#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

char *trimWS(char *string) {
    char *end;
    end = string + strlen(string) - 1;
    
    while (isspace(*string)) string++;
    
    if (*string == 0)
        return string;
    
    while (end > string && isspace(*end)) end--;
    
    *(end + 1) = 0;
    
    return string;
}

char *extractCommand(char *cmd, char *input) {
    memcpy(cmd, input, strlen(input)+1);
    int i;
    
    i = 0;
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
    char *time_string = (char*) malloc(sizeof(char) * 30);
    
    time_t rawtime;
    struct tm *timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    //if (strftime(time_string, sizeof(time_string), "%c", timeinfo) != 0)
    strftime(time_string, 30, "%c", timeinfo);
    
    strcat(time_string, "\n");
    
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
                if (strstr(pipe_input, input)) {
                    grep_string = (char*) realloc(grep_string, sizeof(char) * strlen(pipe_input));
                    strcat(grep_string, pipe_input);
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

void parseInput(char *raw_input, char *pipe_input) {
    char *input;
    char cmd[256];
    
    char *output = (char*) malloc(sizeof(char));
    strcpy(output, "");
    
    input = trimWS(raw_input);
    input = extractCommand(cmd, input);
    
    if (!strcmp(cmd, "date")) {
        if (strlen(input) && input[0] != 124) {
            output = (char*) realloc(output, sizeof(char) * 19);
            strcat(output, "invalid arguments");
            //strcpy(time_string, "invalid arguments");
        } else {
            char *date_string = dateCMD();
            output = (char*) realloc(output, sizeof(char) * strlen(date_string));
            strcat(output, date_string);
            free(date_string);
        }
    } else if (!strcmp(cmd, "echo")) {
        puts(input);
    } else if (!strcmp(cmd, "ls")) {
        if (strlen(input) && input[0] != 124) {
            output = (char*) realloc(output, sizeof(char) * 19);
            strcat(output, "invalid arguments");
            //strcpy(time_string, "invalid arguments");
        } else {
            char *ls_string = lsCMD();
            output = (char*) realloc(output, sizeof(char) * strlen(ls_string));
            strcat(output, ls_string);
            free(ls_string);
        }
    } else if (!strcmp(cmd, "cd")) {
        chdir(input);
    } else if (!strcmp(cmd, "grep")) {
        char *grep_string = grepCMD(input, pipe_input);
        output = (char*) realloc(output, sizeof(char) * strlen(grep_string));
        strcat(output, grep_string);
        free(grep_string);
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