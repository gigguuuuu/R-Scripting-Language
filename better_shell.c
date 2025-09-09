#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define RESET "\033[0m"
#define BLACK       "\033[0;30m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"


int main(){
    char *color = GREEN;
    char *error_color = RED;
    char command[150];
    while(1){
        printf( "%sbsh>%s",color,RESET);
        fflush(stdout);
        if(fgets(command,sizeof(command),stdin)==NULL){
            continue;
        }
        command[strcspn(command,"\n")] = '\0';
        if (strcmp(command,"exit")==0 || strcmp(command,"bye")==0){
            printf("Bye !");
            exit(1);
        }
        char *args[10];
        int count = 0;
        char *token = strtok(command," ");
        while(token!=NULL){
            args[count++] = token;
            token = strtok(NULL," ");
        }
        args[count] = NULL;
        if (strcmp(args[0],"setcolor")==0){
            if (args[1]==NULL){
                printf("enter a color's name bitch !");
            }
            else if (strcmp(args[1],"black")==0 || strcmp(args[1],"BLACK")==0){
                color = BLACK;
                continue;
            }
            else if(strcmp(args[1],"RED")==0 || strcmp(args[1],"red")==0){
                color = RED;
                error_color = GREEN;
                continue;
            }
            else if(strcmp(args[1],"GREEN")==0 || strcmp(args[1],"green")==0){
                color = GREEN;
                error_color = RED;
                continue;
            }
            else if(strcmp(args[1],"YELLOW")==0 || strcmp(args[1],"yellow")==0){
                color = YELLOW;
                continue;
            }
            else if(strcmp(args[1],"blue")==0 || strcmp(args[1],"BLUE")==0){
                color = BLUE;
                continue;
            }
            else{
                printf("%sColor does not exsits !%s",error_color,RESET);
            }
        }

        if (strcmp(args[0],"cd")==0){
            if (args[1]==NULL){
                chdir(getenv("HOME"));
            }
            else if(chdir(args[1])!= 0){
                printf("%sFailed: Dirctory Does Not Exsits !%s",error_color,RESET);
            }
            continue;
        }
        int pid = fork();
        if(pid==0){
            execvp(args[0], args);
            printf("%sThe command does not exsits !%s\n",error_color,RESET);
            exit(1);
        }
        else if (pid > 0){
            wait(NULL);
        }
        else{
            printf("%sSomething went wrong !%s",error_color,RESET);
        }
    }
    printf("EXITING..!");
    return 0;
}