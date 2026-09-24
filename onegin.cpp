#include<stdio.h>
#include<ctype.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000



FILE *open_file_and_get_size(const char *filename, long *size_of_file);
void get_in_buffer(char *buffer, long size, FILE *fp);
int get_count_lines(char * buffer, long file_size);
void split_buffer(char**index, char *buffer, long file_size);
void sort_lines(char **lines, int nlines);
int compare(const void * a, const void * b);
int strcmp_bg(const char *str1, const char *str2);
int strcmp2(char *line1, char *line2);


int main(void) {
    long file_size = 0; 
    FILE *fp = open_file_and_get_size("data.txt", &file_size);
    if (fp == NULL) {
        return 1; 
    }
    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
       printf("Ошибка выделения памяти!\n");
       fclose(fp);
       return 1;
   }
   get_in_buffer(buffer, file_size, fp);
   int nlines = get_count_lines(buffer, file_size);
   printf("найдено строк%d\n", nlines);
    
   char **ptr_to_str;
   ptr_to_str = (char**)calloc(nlines, sizeof(char *));
   if (ptr_to_str == NULL){
    printf("оштбка в выделении динамической памяти для массива указателй\n");
    fclose(fp);
    return 1;
   }
   
   FILE *out_file = fopen("output.txt", "w");
   if (out_file != NULL) {
        split_buffer(ptr_to_str, buffer, file_size); 
        
        for (int i = 0; i < nlines; i++){
            fprintf(out_file, "%s\n" , ptr_to_str[i]);
        }
        sort_lines(ptr_to_str, nlines);
        for (int i = 0; i < nlines; i++) {
            fprintf(out_file, "%s\n", ptr_to_str[i]); 
        }
        fclose(out_file); 

        printf("Результат успешно записан в output.txt\n");
    } else {
        printf("Ошибка: не удалось создать файл output.txt\n");
    }
  

   sort_lines(ptr_to_str, nlines);

  

    if (out_file != NULL) {
        
        for (int i = 0; i < nlines; i++) {
            fprintf(out_file, "%s\n", ptr_to_str[i]); 
        }
        fclose(out_file); 
        printf("Результат успешно записан в output.txt\n");
    } else {
        printf("Ошибка: не удалось создать файл output.txt\n");
    }


    for (int i = 0; i < nlines; i++){
    free(ptr_to_str[i]);
    }
    free(ptr_to_str);

    fclose(fp); 
    free(buffer);
    return 0;
}


FILE *open_file_and_get_size(const char *filename, long *size_of_file) {

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Ошибка: не удалось открыть файл '%s'\n", filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    *size_of_file = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return fp;
}

void get_in_buffer(char *buffer, long size, FILE *fp){
    fread(buffer, 1, size, fp);
    buffer[size] = '\0'; 
}




int get_count_lines(char * buffer, long file_size){
    int count = 0;
    int i;
    for(i = 0; i < file_size; i++){
        if(buffer[i] == '\n')
        count++;
    }
    return count;
}

void split_buffer(char **index, char *buffer, long file_size){
    long bglines = 0;
    int current_line = 0;

    for(int i = 0; i < file_size ; i++){
        if (buffer[i] == '\n'){
        buffer[i] = '\0' ;
        index[current_line] = strdup(&buffer[bglines]);
        bglines = i + 1;
        current_line++ ;
        }
        
    }
}

void sort_lines(char **lines, int nlines) {
   if (nlines > 0 )
   qsort(lines, nlines , sizeof(char*), compare);
}

int compare(const void * a, const void * b){
    const char *str1 = *(const char**)a;
    const char *str2 = *(const char**)b;
    return strcmp_bg(str1, str2);
}

int strcmp_bg(const char *str1, const char *str2){
    int i, j ;
    i = j = 0;
    while (str1[i] != '\0' && ispunct((unsigned char)str1[i])) {
            i++;
        }
    while(str2[j] != '\0' && ispunct((unsigned char)str2[j])){
        j++;
    }
    if(str1[i] == '\0')
        return -1;
    if(str2[j] == '\0')
        return 1;
    if (str1[i] != '\0' && str2[j] != '\0')
    return strcmp2(str1+i , str2+j);
}
int strcmp2(const char *line1, const char *line2){
    int i, j;
    i = 0;
    j = 0;
    while(line1[i] == line2[j] && line1[i] != '\0' && line2[j] != '\0'){
        i++;
        j++;
    }
    int result = 0;
    result = line1[i] - line2[j] ;
    if(result > 0){
        return 1;
    }else if(result < 0){
        return -1;
    }else {
    printf("строки равны\n");
    return 0;
    }
}