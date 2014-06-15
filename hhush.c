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
    while (!isspace(*(input + i)) && *(input + i) != 0) i++;
    
    *(cmd + i) = 0;
    
    if (strlen(cmd) == strlen(input)) {
        *input = 0;
    } else {
        input += ++i;
    }

    return input;
}

char *dateCMD(char *input) {
    char *time_string = (char*) malloc(sizeof(char) * 30);
    
    if (strlen(input) == 0) {
        time_t rawtime;
        struct tm *timeinfo;
        
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        
        //if (strftime(time_string, sizeof(time_string), "%c", timeinfo) != 0)
        strftime(time_string, 30, "%c", timeinfo);
    } else {
        strcpy(time_string, "invalid arguments");
    }
    
    return time_string;
}

char *lsCMD(char *input) {
    char *ls_string = (char*) malloc(sizeof(char));
    strcpy(ls_string, "");
    
    if (!strlen(input)) {
        DIR *dirp = opendir(".");
        struct dirent *file;
        
        while((file = readdir(dirp))!= NULL) {
            if(file->d_name[0] == 46)
                continue;
            
            ls_string = (char*) realloc(ls_string, sizeof(file->d_name));
            strcat(ls_string, file->d_name);
            strcat(ls_string, "\n");
        }
        
        closedir(dirp);
    } else {
        ls_string = (char*) realloc(ls_string, 19);
        strcpy(ls_string, "invalid arguments\n");
    }

    return ls_string;
}

int main() {
    char raw_input[256];
    char *input;
    char cmd[256];
    
    char cwd[1024];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s $ ", cwd);
    
    while (1) {
        fgets(raw_input, sizeof(raw_input), stdin);
        input = trimWS(raw_input);
        input = extractCommand(cmd, input);
        
        if (!strcmp(cmd, "date")) {
            char *date_string = dateCMD(input);
            puts(date_string);
            free(date_string);
        } else if (!strcmp(cmd, "echo")) {
            puts(input);
        } else if (!strcmp(cmd, "ls")) {
            char *ls_string = lsCMD(input);
            printf("%s", ls_string);
            free(ls_string);
        } else if (!strcmp(cmd, "cd")) {
            chdir(input);
        } else if (!strcmp(cmd, "exit")) {
            fclose(stdin);
            fclose(stdout);
            fclose(stderr);
            exit(0);
        } else {
            if (strlen(cmd))
                puts("command not found");
        }
        
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s $ ", cwd);
    }
    
    return 0;
}