#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

char *input = NULL;
char *cmd = NULL;
char *params = NULL;
char *output = NULL;
char **history = NULL;
int hist_length = 0;

char *trimWS(char *ptr) {
    char *end = ptr + strlen(ptr) - 1;
    
    while (isspace(*ptr)) ptr++;
    
    if (*ptr != 0) {
        while (end > ptr && isspace(*end)) end--;
        *(end + 1) = 0;
    }
    
    return ptr;
}

void extractCommand() {
    memcpy(cmd, input, strlen(input) + 1);
    memcpy(params, input, strlen(input) + 1);
    int i = 0;
    
    while (!isspace(*(cmd + i)) && *(cmd + i) != 124 && *(cmd + i) != 0) i++;
    
    *(cmd + i) = 0;
    
    if (strlen(cmd) == strlen(input)) {
        *params = 0;
    } else {
        params += ++i;
    }
}

void pushHistory(char *input) {
    char *ptr = malloc(strlen(input) + 1);
    strcpy(ptr, input);
    
    history = realloc(history, (hist_length + 1) * sizeof(char *));
    history[hist_length] = ptr;
    
    hist_length++;
}

void clearHistory() {
    for (int i = 0; i < hist_length; i++) {
        free(history[i]);
    }
    
    free(history);
    hist_length = 0;
}

void invalidArguments() {
    output = realloc(output, 19);
    strcpy(output, "invalid arguments\n");
}

void dateCMD() {
    time_t rawtime;
    struct tm *timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    char temp[30];
    strftime(temp, 30, "%c", timeinfo);
    strcat(temp, "\n");
    
    output = realloc(output, strlen(output) + strlen(temp) + 1);
    strcat(output, temp);
}

void lsCMD() {
    DIR *dirp = opendir(".");
    struct dirent *file;
    
    while ((file = readdir(dirp))!= NULL) {
        if (file->d_name[0] == 46)
            continue;
        
        output = realloc(output, strlen(output) + strlen(file->d_name) + 2);
        //ls_string = realloc(ls_string, strlen(output) + sizeof(file->d_name));
        strcat(output, file->d_name);
        strcat(output, "\n");
    }
    
    closedir(dirp);
}

void grepCMD(char *pipe_input) {
    if (strlen(params)) {
        char *pattern = strtok(params, " ");
        char *file = strtok(NULL, " ");
        
        if (pipe_input) {
            if (pattern && file) {
                invalidArguments();
            } else {
                char temp[512];
                char *ptr = pipe_input;
                
                while (*ptr != 0) {
                    sscanf(ptr, "%[^\n]\n", temp);
                    strcat(temp, "\n");
                    
                    if (strstr(temp, params)) {
                        output = realloc(output, strlen(output) + strlen(temp) + 1);
                        strcat(output, temp);
                    }
                    ptr += strlen(temp);
                }
            }
        } else {
            if (pattern && file && !strtok(NULL, " ")) {
                FILE *filep = fopen(file, "r");
                
                if (filep == NULL) {
                    output = realloc(output, 27);
                    strcpy(output, "no such file or directory\n");
                } else {
                    char temp[512];
                    
                    while (fgets(temp, 512, filep)) {
                        if (strstr(temp, pattern)) {
                            output = realloc(output, strlen(output) + strlen(temp) + 1);
                            strcat(output, temp);
                        }
                    }
                    
                    fclose(filep);
                }
            } else {
                invalidArguments();
            }
        }
    } else {
        invalidArguments();
    }
}

void historyCMD() {
    for (int i = 0; i < hist_length; i++) {
        printf("%i %s\n", i, history[i]);
    }
}

void parseInput(char *pipe_input) {
    char *pipe = NULL;
    
    output = malloc(1);
    *output = 0;
    
    extractCommand();
    params = trimWS(params);
    
    if ((pipe = strstr(params, "|"))) {
        *pipe = 0;
        pipe++;
        pipe = trimWS(pipe);
    }
    
    if (!strcmp(cmd, "date")) {
        if (strlen(params) && !pipe) {
            invalidArguments();
        } else {
            dateCMD();
        }
    } else if (!strcmp(cmd, "echo")) {
        output = realloc(output, strlen(output) + strlen(params) + 2);
        strcat(output, params);
        strcat(output, "\n");
    } else if (!strcmp(cmd, "ls")) {
        if (strlen(params) && !pipe) {
            invalidArguments();
        } else {
            lsCMD();
        }
    } else if (!strcmp(cmd, "cd")) {
        chdir(params);
    } else if (!strcmp(cmd, "grep")) {
        grepCMD(pipe_input);
    } else if (!strcmp(cmd, "exit")) {
        clearHistory();
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
        exit(0);
    } else {
        if (strlen(cmd))
            puts("command not found");
        pipe = NULL;
    }
    
    if (pipe) {
        input = pipe;
        parseInput(output);
    } else if (output) {
        printf("%s", output);
        free(output);
    }
    
    free(pipe_input);
}

int main() {
    char raw_input[256];
    char raw_cmd[256];
    char raw_params[256];
    char cwd[1024];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s $ ", cwd);
    
    while (1) {
        input = raw_input;
        cmd = raw_cmd;
        params = raw_params;
        
        fgets(raw_input, sizeof(raw_input), stdin);
        input = trimWS(input);
        
        pushHistory(input);
        
        parseInput(NULL);
        
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s $ ", cwd);
    }
    
    return 0;
}
