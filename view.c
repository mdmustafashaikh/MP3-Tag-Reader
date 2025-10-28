
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "view.h"
#include "edit.h"

/* Function to display the tags of the MP3 file.
 * It reads the tags from the MP3 file and prints them to the console.
 * Returns e_success on successful display, e_failure otherwise.
 */

Status store_tags(MP_READ *mp3_reader)
{
    // Read the general tag
    mp3_reader->tag_pos[0] = ftell(mp3_reader->fptr_mp3);
    strcpy(mp3_reader->title_tag, tag_read(mp3_reader->fptr_mp3, mp3_reader));
    mp3_reader->title_size = size_read(mp3_reader->fptr_mp3, mp3_reader);
    strcpy(mp3_reader->title_name, name_read(mp3_reader->fptr_mp3, mp3_reader, mp3_reader->title_size));
    

    // Artist
    mp3_reader->tag_pos[1] = ftell(mp3_reader->fptr_mp3);
    strcpy(mp3_reader->artist_tag, tag_read(mp3_reader->fptr_mp3, mp3_reader));
    mp3_reader->artist_size = size_read(mp3_reader->fptr_mp3, mp3_reader);
    strcpy(mp3_reader->artist_name, name_read(mp3_reader->fptr_mp3, mp3_reader, mp3_reader->artist_size));

    // Album
    mp3_reader->tag_pos[2] = ftell(mp3_reader->fptr_mp3);
    strcpy(mp3_reader->album_tag, tag_read(mp3_reader->fptr_mp3, mp3_reader));
    mp3_reader->album_size = size_read(mp3_reader->fptr_mp3, mp3_reader);
    strcpy(mp3_reader->album_name, name_read(mp3_reader->fptr_mp3, mp3_reader, mp3_reader->album_size));

    // Year
    mp3_reader->tag_pos[3] = ftell(mp3_reader->fptr_mp3);
    strcpy(mp3_reader->Year_tag, tag_read(mp3_reader->fptr_mp3, mp3_reader));
    mp3_reader->Year_size = size_read(mp3_reader->fptr_mp3, mp3_reader);
    strcpy(mp3_reader->Year, name_read(mp3_reader->fptr_mp3, mp3_reader, mp3_reader->Year_size));

    // Genre
    mp3_reader->tag_pos[4] = ftell(mp3_reader->fptr_mp3);
    strcpy(mp3_reader->Content_tag, tag_read(mp3_reader->fptr_mp3, mp3_reader));
    mp3_reader->Content_size = size_read(mp3_reader->fptr_mp3, mp3_reader);
    strcpy(mp3_reader->Content_type, name_read(mp3_reader->fptr_mp3, mp3_reader, mp3_reader->Content_size));

    // Comment
    mp3_reader->tag_pos[5] = ftell(mp3_reader->fptr_mp3);
    strcpy(mp3_reader->Comment_tag, tag_read(mp3_reader->fptr_mp3, mp3_reader));
    mp3_reader->Comment_size = size_read(mp3_reader->fptr_mp3, mp3_reader);
    strcpy(mp3_reader->Comment, name_read(mp3_reader->fptr_mp3, mp3_reader, mp3_reader->Comment_size));

    return e_success;
}

/* Function to print help instructions.
 * It displays the usage and available operations for the MP3 tag reader and editor.
 */

void Print() 
{
    printf("ERROR : ./a.out : INVALID ARGUMENTS\n");
    printf("USAGE : \n");
    printf("To view please pass like : ./a.out -v mp3filename\n");
    printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
    printf("To get help pass like : ./a.out --help\n");
}


/* open_files function to open the MP3 file for reading.
 * It initializes the file pointer and checks if the file exists.
 * Returns e_success on successful opening, e_failure otherwise.
 */

Status open_files(MP_READ *mp3_reader)
{
    mp3_reader->fptr_mp3 = fopen(mp3_reader->file_name, "r");
    if (mp3_reader->fptr_mp3 == NULL) {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", mp3_reader->file_name);
        return e_failure;
    }
    return e_success;
}

/* Function to read the general tag from the MP3 file.
 * It reads the tag and returns it as a string.
 * Returns the tag string on success, NULL on failure.
 */

char* tag_read(FILE *fptr_mp3, MP_READ *mp3_reader)
{
    char tag_buffer[5] = {0};
    fread(tag_buffer, 4, 1, fptr_mp3);
    tag_buffer[4] = '\0';
    strcpy(mp3_reader->gen_tag, tag_buffer);
    return mp3_reader->gen_tag;
}


/* Function to read the size of the tag from the MP3 file.
 * It reads 4 bytes and returns the size as an integer.
 * Returns the size on success, -1 on failure.
 */

int size_read(FILE *fptr_mp3, MP_READ *mp3_reader)
{
    int size_buffer = 0;
    fread(&size_buffer, 4, 1, fptr_mp3);
    size_buffer = __builtin_bswap32(size_buffer);
    return size_buffer;
}

/* Function to read the name associated with the tag from the MP3 file.
 * It reads the name based on the size provided and returns it as a string.
 * Returns the name string on success, NULL on failure.
 */

char *name_read(FILE *fptr_mp3, MP_READ *mp3_reader, int size)
{
    char name_buffer[256] = {0};
    fseek(fptr_mp3, 3, SEEK_CUR);
    fread(name_buffer, size - 1, 1, fptr_mp3);
    name_buffer[size - 1] = '\0';
    strcpy(mp3_reader->gen_tag, name_buffer);
    return mp3_reader->gen_tag;
}