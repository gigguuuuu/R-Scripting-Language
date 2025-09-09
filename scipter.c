#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** lexer(char *code){
    int count = 0;
    int cnt_args = 0;
    int store_size = 3;
    char *token = strtok(code," ;\n");
    char **args = malloc(store_size * sizeof(char *));
    while (token!=NULL){
        if (count >= store_size){
            store_size *= 2;
            char **temp_mem = realloc(args,store_size * sizeof(char *));
            if (!temp_mem){
                printf("Memory Allocation Failed !");
                free(args);
                exit(1);
            }
            args = temp_mem; 
        }
        args[cnt_args] = strdup(token);
        if (!args[cnt_args]){
            printf("Token Copying Has Failed !");
            for (int i = 0; i < count; i++) free(args[i]);
            free(args);
            return NULL;
        }
        token = strtok(NULL," ;\n");
        cnt_args++;
        count++;
    }
    args[cnt_args] = NULL;
    return args;
}
char* open_r(char *filename){
    char *ex = strrchr(filename,'.');
    if (ex && strcmp(ex,".r")==0){
        FILE *pFilename = fopen(filename,"r");
        if (!pFilename){
            printf("Cant Read The File !");
            char error1[10] = "wrong";
            return "wrong";
        }
        fseek(pFilename,0,SEEK_END);
        long size = ftell(pFilename);
        rewind(pFilename);
        char *by_code = malloc(size + 2);
        if (!by_code){
            printf("Memory Allocation Failed !");
            return NULL;
        }
        fread(by_code,1,size,pFilename);
        by_code[size] = '\0';
        fclose(pFilename);
        return by_code;
    }
    else{
        char *err_code = "error";
        return err_code;
    }
}
int main(int argc,char *argv[]){
    if (argc==2){
        char *filename = argv[1];
        char *read = open_r(filename);
        if (read){
            char **lex = lexer(read);
            printf("%s",lex[1]);
            if (strcmp(lex[0],"let")==0){
                printf("it's fucking work ! ");
            }
        }
        else if (strcmp(read,"error")==0){
            printf("RCC Got An Error: Can Not Read File OR  File Does Not Exsits !");
        }
        else{
            printf("RCC Has Failed !");
        }
        return 0;
    }
    else if (argc > 2){
        printf("Only One Input Needed !");
        return 1;
    }
    else if (argc == 1){
        printf("Need File Name !");
        return 1;
    }
    
}