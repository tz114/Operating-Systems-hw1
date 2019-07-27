#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h> 

//hash function
int hash(char *word, int size){
    int len,sum=0,i;
    
    len=strlen(word);
    for(i=0;i<len;i++){
        sum=sum + *(word+i);
    }
    return sum % size;
}

//function used to check if the 2nd argument is a number
bool isNumber(char *number)
{
    int i = 0;
    if (*number == '-'){
        i = 1;
    }
    for(; (*(number+i)) != 0; i++){
        if (!isdigit((*(number+i)))){
            return false;
        }
    }
    return true;
}
int main(int argc, char **argv){
    char **cache;
    //allocate a big number so that i can't possibly run out of room for file
    char *line = calloc(1000000,sizeof(char));
    
    int hashnum;
    if(argc<3){
        fprintf(stderr,"Invalid usage - not enough args\n");
        return EXIT_FAILURE;
    }
    if(!isNumber(*(argv+1))){
        fprintf(stderr,"Invalid usage - invalid cache size\n");
        return EXIT_FAILURE;
    }
    
    cache = calloc(atoi(*(argv+1)),sizeof(char*));
    if(cache==NULL){
        fprintf(stderr,"too big!\n");
        return EXIT_FAILURE;
    }

    FILE* file = fopen(*(argv+2),"r");
    if(file==NULL){
        fprintf(stderr,"Invalid usage - cannot open file\n");
        return EXIT_FAILURE;
    }
    char *ptr = calloc(1000000,sizeof(char));
    free(ptr);

    char *alloc="calloc";
    //read each line in file
    while (fgets(line,1000000,file)!=NULL){
        
        //replace all delimiters with space
        for(int i=0;i<strlen(line);i++){
            if ( !( (isalpha(*(line+i)) || *(line+i)==' ' || isdigit(*(line+i))) || *(line+i) == '\0' )){
                (*(line+i))=' ';
            }
        }
        char* dup = strdup(line);
        ptr=strtok(dup," ");
        //process each word in line
        while(ptr!=NULL){
            if(strlen(ptr)>2){
                
                hashnum = hash(ptr,atoi(*(argv+1)));
                if(*(cache+hashnum)==NULL){
                    alloc="calloc";
                    *(cache+hashnum)= calloc(strlen(ptr)+1,sizeof(char));
                    strcpy(*(cache+hashnum),ptr);
                }
                else{
                    alloc="realloc";
                  
                    *(cache+hashnum)=realloc(*(cache+hashnum),strlen(ptr)*sizeof(char)+1);
                    strcpy(*(cache+hashnum),ptr);
                }
                    
                    
                printf("Word \"%s\" ==> %d (%s)\n",ptr,hashnum,alloc);
                
            }
            ptr=strtok(NULL," ");
     
        }
        free(dup);
        //
       
        
    }
    
    fclose(file);
    //free memory
    for(int i=0;i<atoi(*(argv+1));i++){
        if(*(cache+i)!=NULL){
            printf("Cache index %d ==> \"%s\"\n",i,*(cache+i));
        }
        free(*(cache+i));
    }
    
   free(cache);
   
    free(line);
    
    return 0;
}