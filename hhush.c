#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

char *trimWS(char *raw_input) {
    char *end = raw_input + strlen(raw_input) - 1;
    
    while (isspace(*raw_input)) raw_input++;
    
    if (*raw_input == 0)
        return raw_input;
    
    while (end > raw_input && isspace(*end)) end--;
    
    *(end + 1) = 0;
    
    return raw_input;
}

char *extractCommand(char *cmd, char *input) {
    memcpy(cmd, input, strlen(input) + 1);
    int i = 0;
    
    while (!isspace(*(input + i)) && *(input + i) != 124 && *(input + i) != 0) i++;
    
    *(cmd + i) = 0;
    
    if (strlen(cmd) == strlen(input)) {
        *input = 0;
    } else {
        input += ++i;
    }

    return input;
}

char *dateCMD() {  
    time_t rawtime;
    struct tm *timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    char temp[30];
    strftime(temp, 30, "%c", timeinfo);
    strcat(temp, "\n");
    
    char *time_string = (char*) malloc(sizeof(char) * (strlen(temp) + 1));
    strcpy(time_string, temp);
    
    return time_string;
}

char *lsCMD() {
    char *ls_string = (char*) malloc(sizeof(char));
    strcpy(ls_string, "");
    
    DIR *dirp = opendir(".");
    struct dirent *file;
    
    while ((file = readdir(dirp))!= NULL) {
        if (file->d_name[0] == 46)
            continue;
        
        ls_string = (char*) realloc(ls_string, sizeof(file->d_name));
        strcat(ls_string, file->d_name);
        strcat(ls_string, "\n");
    }
    
    closedir(dirp);

    return ls_string;
}

char *grepCMD(char *input, char *pipe_input) {
    char *grep_string = (char*) malloc(sizeof(char));
    strcpy(grep_string, "");
    
    if (strlen(input)) {
        char *pattern = strtok(input, " ");
        char *file = strtok(NULL, " ");
        
        if (pipe_input) {
            if (pattern && file) {
                grep_string = (char*) realloc(grep_string, 19);
                strcpy(grep_string, "invalid arguments\n");
            } else {
                char temp[512];
                char *ptr = pipe_input;
                
                while (*ptr != 0) {
                    sscanf (ptr, "%s\n", temp);
                    strcat(temp, "\n");
                    if (strstr(temp, input)) {
                        grep_string = (char*) realloc(grep_string, sizeof(char) * strlen(temp));
                        strcat(grep_string, temp);
                    }
                    ptr += strlen(temp);
                }
            }
        } else {
            if (pattern && file && !strtok(NULL, " ")) {
                FILE *filep = fopen(file, "r");
                
                if (filep == NULL) {
                    grep_string = (char*) realloc(grep_string, 27);
                    strcpy(grep_string, "no such file or directory\n");
                } else {
                    char temp[512];
                    
                    while (fgets(temp, 512, filep)) {
                        if (strstr(temp, pattern)) {
                            grep_string = (char*) realloc(grep_string, sizeof(temp));
                            strcat(grep_string, temp);
                        }
                    }
                    
                    fclose(filep);
                }
            } else {
                grep_string = (char*) realloc(grep_string, 19);
                strcpy(grep_string, "invalid arguments\n");
            }
        }
    } else {
        grep_string = (char*) realloc(grep_string, 19);
        strcpy(grep_string, "invalid arguments\n");
    }
    
    return grep_string;
}

char *setOutput(char *string, int free_flag) {
    char *output = (char*) malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(output, string);
    
    if (free_flag)
        free(string);
    
    return output;
}

void parseInput(char *raw_input, char *pipe_input) {
    char *input;
    char *output = NULL;
    char cmd[256];
    
    input = trimWS(raw_input);
    input = extractCommand(cmd, input);
    
    if (!strcmp(cmd, "date")) {
        if (strlen(input) && input[0] != 124) {
            output = setOutput("invalid arguments\n", 0);
        } else {
            output = setOutput(dateCMD(), 1);
        }
    } else if (!strcmp(cmd, "echo")) {
        output = setOutput(strcat(input, "\n"), 0);
    } else if (!strcmp(cmd, "ls")) {
        if (strlen(input) && input[0] != 124) {
            output = setOutput("invalid arguments\n", 0);
        } else {
            output = setOutput(lsCMD(), 1);
        }
    } else if (!strcmp(cmd, "cd")) {
        chdir(input);
    } else if (!strcmp(cmd, "grep")) {
        output = setOutput(grepCMD(input, pipe_input), 1);
    } else if (!strcmp(cmd, "exit")) {
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
        exit(0);
    } else {
        if (strlen(cmd))
            puts("command not found");
    }
    
    if (input[0] == 124) {
        parseInput(++input, output);
    } else {
        printf("%s", output);
    }
    
    free(output);
}

int main() {
    char raw_input[256];
    char cwd[1024];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s $ ", cwd);
    
    while (1) {
        fgets(raw_input, sizeof(raw_input), stdin);
        parseInput(raw_input, NULL);
        
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s $ ", cwd);
    }
    
    return 0;
}