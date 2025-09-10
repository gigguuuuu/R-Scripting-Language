#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    // var storage & var name storage for "for loop !";
    char *var_name;
    int var_value;
    char *var_char_value;
    // for char var storage;
}variables;

char** lexer(char *code){
    int count = 0;
    int cnt_args = 0;
    int store_size = 3;
    int i = 0;
    char **args = malloc(store_size * sizeof(char *));
    if (!args){
        printf("Memory Allocation failed !");
        exit(1);
    }
    while(code[i]!='\0'){
        if (code[i]==' ' || code[i] == ';'|| code[i]=='\n'){
            i++;
            continue;
        }
        else if (code[i]=='"'){
            i++;
            int j = 0;
            char buffer[256];
            while(code[i]!='"' && code[i]!='\0'){
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';
            if(code[i]==' '){
                i++;
            }
            if (cnt_args >= store_size) {
                store_size *= 2;
                char **temp = realloc(args, store_size * sizeof(char *));
                if (!temp){
                    printf("memory allocation failed !");
                    for (int i = 0; i < cnt_args; i++) {
                        free(args[i]);
                    }
                    free(args);
                    exit(1);
                }
                args = temp;
            }
            args[cnt_args++] = strdup(buffer); // store token
            continue;
        }
        else if (code[i]!=' ' && code[i] != ';' && code[i] != '\n'){
            int j = 0;
            char buffer[256];
            while(code[i] != ' ' && code[i] != ';' && code[i] != '\n'){
                buffer[j++] = code[i++];
            }
            if  (cnt_args >= store_size) {
                store_size *= 2;
                args = realloc(args, store_size * sizeof(char *));
                if (!args){
                    printf("Memory Allocation Failed !");
                    exit(1);
                }
            }
            buffer[j] = '\0';
            args[cnt_args++] = strdup(buffer); // store token
            continue;
        }
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
        variables vars[100];
        int var_count = 0;

        if (read){
            char **lex = lexer(read);
            int i = 0;
            while (lex[i] != NULL && strcmp(lex[i], "stop:") != 0){
                if (strcmp(lex[i],"let")==0){
                    if (lex[i + 1] && lex[i + 3] && strcmp(lex[i + 2],"=")==0){
                        if (lex[i + 3][0] == '"'){
                            char *str_val = lex[i + 3];
                            size_t len = strlen(str_val);

                            if (len > 1 && str_val[len - 1] == '"') {
                                str_val[len - 1] = '\0';
                            }
                            vars[var_count].var_name = strdup(lex[i + 1]);
                            vars[var_count].var_char_value = lex[i + 3];
                            var_count++;
                        }
                        else{
                            vars[var_count].var_name = strdup(lex[i + 1]);
                            vars[var_count].var_value = atoi(lex[i + 3]);
                            var_count++;
                        }

                    }
                }
                else if (strcmp(lex[i],"print")==0){
                    printf("print is working on a point !\n");
                    for (int k = 0;k < var_count;k++){
                        if (strcmp(lex[i + 1],vars[k].var_name)==0){
                            if (vars[k].var_char_value){
                                printf("%s\n",vars[k].var_char_value);
                            }
                            else{
                                printf("%d",vars[k].var_value);
                                }
                            }
                        }
                    }
                i++;
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