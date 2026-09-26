#include<stdio.h>
#include<ctype.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>




FILE *open_file_and_get_size(const char *filename, long *size_of_file);
void get_in_buffer(char *buffer, long size, FILE *fp);
int get_count_lines(char * buffer, long file_size);
void split_buffer(char**index, char *buffer, long file_size);
void sort_lines_bg(char **lines, int nlines);
void sort_lines_end(char** lines, int nlines);
int compare1(const void * a, const void * b);
void copy_orig_strings(char** copy, char** orig, int count_lines);
char* clean_string(const char* str);
int strcmp_from_end(const char *s1, const char *s2);
int compare2(const void* a, const void* b);
void write_section_to_file(FILE *out_file, const char *title, char **lines, int nlines);




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
   split_buffer(ptr_to_str, buffer, file_size);
   char **orig_ptr;
   orig_ptr = (char**)calloc(nlines, sizeof(char*));
   copy_orig_strings(ptr_to_str, orig_ptr, nlines);
   
   FILE *out_file = fopen("my_values.txt", "w");
    if (out_file == NULL) {
        printf("Ошибка: не удалось создать файл!\n");
        return 1; 
    }else{
        write_section_to_file(out_file, "обычный текст", orig_ptr, nlines);
        sort_lines_bg(ptr_to_str, nlines);
        sort_lines_end(orig_ptr, nlines);
        write_section_to_file(out_file, "отсортированный с начала строк", ptr_to_str, nlines);
        write_section_to_file(out_file, "остортированный с конца строк", orig_ptr, nlines);

    }
  

   

    for (int i = 0; i < nlines; i++){
        free(ptr_to_str[i]);
    }
    for(int i = 0; i < nlines; i++){
        free(orig_ptr[i]);
    }
    free(ptr_to_str);
    free(orig_ptr);
    free(buffer);

    fclose(fp); 
    fclose(out_file);
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
    if (file_size > 0 && buffer[file_size - 1] != '\n') {
        count++ ;
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
    if(bglines < file_size)
        index[current_line] = strdup(&buffer[bglines]);
}

void sort_lines_bg(char **lines, int nlines) {
   if (nlines > 0 )
        qsort(lines, nlines , sizeof(char*), compare1);
}

int compare1(const void *a, const void *b) {
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;

    char *clean1 = clean_string(str1);
    char *clean2 = clean_string(str2);


    if (clean1 == NULL || clean2 == NULL) {
        free(clean1);
        free(clean2);
        return 0; 
    }


    int result = strcmp(clean1, clean2);

  
    free(clean1);
    free(clean2);

    return result;
}

char* clean_string(const char *original) {
    
    char *clean = (char *)malloc(strlen(original) + 1);
    if (clean == NULL) {
        return NULL; 
    }

    int j = 0; 

    
    for (int i = 0; original[i] != '\0'; i++) {
       
        if (isalnum((unsigned char)original[i])) {
            clean[j] = tolower((unsigned char)original[i]);
            j++;
        }
        
    }

    clean[j] = '\0';
    return clean; 
}

int compare2(const void *a, const void *b) {
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;

    char *clean1 = clean_string(str1);
    char *clean2 = clean_string(str2);

    if (clean1 == NULL || clean2 == NULL) {
        free(clean1);
        free(clean2);
        return 0; 
    }

    int result = strcmp_from_end(clean1, clean2);

    free(clean1);
    free(clean2);

    return result;
}

int strcmp_from_end(const char *s1, const char *s2) {
   
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    
    
    int i = len1 - 1;  
    int j = len2 - 1;  
    
    while (i >= 0 && j >= 0) {
        if (s1[i] != s2[j]) {
       
            return (unsigned char)s1[i] - (unsigned char)s2[j];
        }
        i--;  
        j--;  
    }
    
    
    if (i >= 0) return 1;   
    if (j >= 0) return -1;  
    
    return 0;  
}


void write_section_to_file(FILE *out_file, const char *title, char **lines, int nlines) {
    fprintf(out_file, "=== %s ===\n", title);
    for (int i = 0; i < nlines; i++) {
        fprintf(out_file, "%s\n", lines[i]);
    }
    fprintf(out_file, "\n");
}
void copy_orig_strings(char **copy, char **orig, int nlines) {
    for (int i = 0; i < nlines; i++) {
        orig[i] = strdup(copy[i]);
    }
}


void sort_lines_end(char** lines, int nlines){
    if (nlines > 0 )
        qsort(lines, nlines , sizeof(char*), compare2);
}
