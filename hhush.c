#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

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

char *extractCMD(char *cmd, char *input) {
    memcpy(cmd, input, strlen(input)+1);
    int i;
    
    i = 0;
    while (!isspace(*(input + i))) i++;
    
    *(cmd + i) = 0;
    
    if (strlen(cmd) == strlen(input)) {
        *input = 0;
    } else {
        input += ++i;
    }

    return input;
}

int main() {
    char raw_input[256];
    char *input;
    char cmd[256];
    
    char cwd[1024];
    
    time_t rawtime;
    struct tm *timeinfo;
    char time_string[80];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s $ ", cwd);
    
    while (1) {
        fgets(raw_input, sizeof(raw_input), stdin);
        input = trimWS(raw_input);
        input = extractCMD(cmd, input);
        
        if (strcmp(cmd, "date") == 0) {
            if (strlen(input) == 0) {
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                
                if (strftime(time_string, sizeof(time_string), "%c", timeinfo) != 0)
                    puts(time_string);
            } else {
                puts("invalid arguments");
            }
        } else if (strcmp(cmd, "echo") == 0) {
            puts(input);
        } else if (strcmp(cmd, "exit") == 0) {
            exit(0);
        } else {
            if (strlen(cmd) != 0)
                puts("command not found");
        }
        
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s $ ", cwd);
    }
    
    return 0;
}