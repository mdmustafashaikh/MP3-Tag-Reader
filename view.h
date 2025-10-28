#ifndef MP_READER_H
#define MP_READER_H

#include<stdio.h>
#include<stdlib.h>
#include "types.h"

/* Structure to hold MP3 file information and tags.
 * It includes file pointers, tag positions, and tag details.
 */


typedef struct{
    // MP3 file name
    char *file_name;
    FILE * fptr_mp3;
    
    // Positions of tags in the MP3 file
    int  tag_pos[6];
    // General tag information
    char gen_tag[4];
    char gen_name[50];

    char * def_file;
    FILE * fptr_def;

    //title details
    char title_tag[4];
    int title_size;
    char title_name[50];

    //artist details
    char artist_tag[4];
    int artist_size;
    char artist_name[50];

    //Album details
    char album_tag[4];
    int album_size;
    char album_name[50];

    //Year details
    char Year_tag[4];
    int Year_size;
    char Year[50];

    //Content_type details (Bollywood)
    char Content_tag[4];
    int Content_size;
    char Content_type[50];

    //Comments
    char Comment_tag[4];
    int Comment_size;
    char Comment[50];


}MP_READ;

/* * Function prototypes for MP3 tag reader and editor operations.
 * These functions handle file operations, tag reading, and displaying tags.
 */

OperationType check_operation(char*argv[]);

Status open_files(MP_READ * mp3_reader);

char* tag_read( FILE * fptr_mp3,MP_READ *mp3_reader);

int size_read(FILE*fptr_mp3,MP_READ *mp3_reader);

char *name_read(FILE *fptr_mp3,MP_READ *mp3_reader,int size);

Status store_tags(MP_READ * mp3_reader);

Status Check_header_file(MP_READ * mp3_reader);

Status View_tags(MP_READ * mp3_reader);

void print_equalto();

void Print_view();

void Print();

#endif