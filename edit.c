#include <stdio.h>
#include <string.h>
#include "types.h"
#include "edit.h"
#include "view.h"

/* Function to read and validate the arguments passed for editing MP3 tags.
 * It populates the TagData structure with the necessary information.
 * Returns e_success on successful validation, e_failure otherwise.
 */

Status read_and_validate_mp3_file_args(char *argv[], TagData *mp3tagData) {
    print_equalto();
    printf("\t\t\t🔓 You Choose Edit Operation\n");
    print_equalto();
    if (argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
        printf("INFO: Usage -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
        printf("Modifiers:\n");
        printf("-t\tTitle\n-A\tArtist\n-a\tAlbum\n-y\tYear\n-G\tGenre\n-c\tComments\n");
        return e_failure;
    }
    if (strcmp(argv[2], "-t") == 0){
       
        int valid = 1;
        for (int i = 0; argv[3][i] != '\0'; i++) {
            if (!((argv[3][i] >= 'A' && argv[3][i] <= 'Z') || (argv[3][i] >= 'a' && argv[3][i] <= 'z') || argv[3][i] == ' ')) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("ERROR: Title must contain only alphabetic characters and spaces.\n");
            return e_failure;
        }
        strcpy(mp3tagData->frame_Id, "TIT2");
    }
    else if (strcmp(argv[2], "-a") == 0){
        int valid = 1;
        for (int i = 0; argv[3][i] != '\0'; i++) {
            if (!((argv[3][i] >= 'A' && argv[3][i] <= 'Z') || (argv[3][i] >= 'a' && argv[3][i] <= 'z') || argv[3][i] == ' ')) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("ERROR: Artist must contain only alphabetic characters and spaces.\n");
            return e_failure;
        }
        strcpy(mp3tagData->frame_Id, "TPE1");
    }
    else if (strcmp(argv[2], "-A") == 0){
        strcpy(mp3tagData->frame_Id, "TALB");
    }
    else if (strcmp(argv[2], "-y") == 0) {
        int valid = 1;
        if (strlen(argv[3]) != 4) {
            valid = 0;
        } else {
            for (int i = 0; i < 4; i++) {
                if (argv[3][i] < '0' || argv[3][i] > '9') {
                    valid = 0;
                    break;
                }
            }
        }
        if (!valid) {
            printf("ERROR: Year must be a four-digit number.\n");
            return e_failure;
        }
        strcpy(mp3tagData->frame_Id, "TYER");
    }
    else if (strcmp(argv[2], "-G") == 0) {
        
        int valid = 1;
        for (int i = 0; argv[3][i] != '\0'; i++) {
            if ((argv[3][i] < 'A' || (argv[3][i] > 'Z' && argv[3][i] < 'a') || argv[3][i] > 'z')) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("ERROR: language must contain only alphabetic characters.\n");
            return e_failure;
        }
        strcpy(mp3tagData->frame_Id, "COMM");
    }
    else if (strcmp(argv[2], "-c") == 0) {
        int valid = 1;
        for (int i = 0; argv[3][i] != '\0'; i++) {
            if ((argv[3][i] < 'A' || (argv[3][i] > 'Z' && argv[3][i] < 'a') || argv[3][i] > 'z')) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("ERROR: Genre must contain only alphabetic characters.\n");
            return e_failure;
        }
        strcpy(mp3tagData->frame_Id, "TCON");
    }
    else {
        printf("ERROR: Invalid modifier '%s'\n", argv[2]);
        return e_failure;
    }
    strncpy(mp3tagData->frame_Id_value, argv[3], sizeof(mp3tagData->frame_Id_value));
    mp3tagData->frame_Id_size = strlen(mp3tagData->frame_Id_value) + 1;

    if (mp3tagData->frame_Id_size >= 100) {
        printf("ERROR: New value too long.\n");
        return e_failure;
    }
    if (strstr(argv[4], ".mp3") == NULL) {
        printf("ERROR: Invalid file type. Must be .mp3\n");
        return e_failure;
    }

    mp3tagData->fptr_mp3 = fopen(argv[4], "r");
    if (mp3tagData->fptr_mp3 == NULL) {
        perror("fopen");
        printf("ERROR: Unable to open file %s\n", argv[4]);
        return e_failure;
    }
    char id3[3];
    fread(id3, 1, 3, mp3tagData->fptr_mp3);
    if (strncmp(id3, "ID3", 3) != 0) {
        printf("ERROR: File does not contain valid ID3 header.\n");
        fclose(mp3tagData->fptr_mp3);
        return e_failure;
    }

    fclose(mp3tagData->fptr_mp3);
    return e_success;
}

/*Function to edit the MP3 tag based on the provided arguments.
 * It opens the source MP3 file, creates a temporary file, and performs the editing operation.
 * Returns e_success on successful editing, e_failure otherwise.
 */

Status edit_tag(char *argv[], TagData *mp3tagData) {
    mp3tagData->fptr_mp3 = fopen(argv[4], "r");
    if (!mp3tagData->fptr_mp3) {
        perror("fopen");
        return e_failure;
    }

    mp3tagData->fptr_temp = fopen("temp.mp3", "w");
    if (!mp3tagData->fptr_temp) {
        perror("fopen temp");
        fclose(mp3tagData->fptr_mp3);
        return e_failure;
    }
    char buffer[10];
    if (fread(buffer, 1, 10, mp3tagData->fptr_mp3) < 10) {
        printf("ERROR: Failed to read MP3 header.\n");
        return e_failure;
    }
    fwrite(buffer, 1, 10, mp3tagData->fptr_temp);

    char *frame_tags[] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    Status ret;

    for (int i = 0; i < 6; i++) {
        fread(buffer, 1, 4, mp3tagData->fptr_mp3);
        fwrite(buffer, 1, 4, mp3tagData->fptr_temp);

        if (strncmp(mp3tagData->frame_Id, frame_tags[i], 4) == 0){
            ret = copy_data(mp3tagData, 1);
        }else{
            ret = copy_data(mp3tagData, 0);
        }
        if (ret == e_failure){
        return e_failure;
        }
    }

    if (copy_remaining_data(mp3tagData) == e_failure) return e_failure;

    fclose(mp3tagData->fptr_mp3);
    fclose(mp3tagData->fptr_temp);

    mp3tagData->fptr_mp3 = fopen(argv[4], "w");
    mp3tagData->fptr_temp = fopen("temp.mp3", "r");

    if (!mp3tagData->fptr_mp3 || !mp3tagData->fptr_temp) {
        printf("ERROR: Failed to reopen files for writing final output.\n");
        return e_failure;
    }

    if (copy_back_to_src(mp3tagData) == e_failure){ 
    return e_failure;
    }

    fclose(mp3tagData->fptr_mp3);
    fclose(mp3tagData->fptr_temp);

 
    const char *tag_name = "Tag";
    if (strcmp(mp3tagData->frame_Id, "TIT2") == 0){ 
    tag_name = "Title";
    }
    else if (strcmp(mp3tagData->frame_Id, "TPE1") == 0){
    tag_name = "Artist";
    }
    else if (strcmp(mp3tagData->frame_Id, "TALB") == 0){ 
    tag_name = "Album";
    }
    else if (strcmp(mp3tagData->frame_Id, "TYER") == 0) {
        tag_name = "Year";
    }
    else if (strcmp(mp3tagData->frame_Id, "TCON") == 0) {
        tag_name = "Genre";
    }
    else if (strcmp(mp3tagData->frame_Id, "COMM") == 0) {
        tag_name = "Comment";
    }else {
        printf("Edit operation completed, but no tag was updated.\n");
    }
    printf("%s updated successfully.\nNew %s: %s\n", tag_name, tag_name, mp3tagData->frame_Id_value);
    return e_success;
}

/* * Function to copy data from the source MP3 file to the temporary file.
 * It handles both the case of updating a specific frame and copying remaining data.
 * Returns e_success on successful copy, e_failure otherwise.
 */

Status copy_data (TagData* mp3tagData, char flag)
{
    if (flag == 1)
    {
        uint old_size, new_size;
        int count;
        count = fread (&old_size, 4, 1, mp3tagData->fptr_mp3);
        if (count < 1)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return e_failure;
        }
        else{
            char* pos = (char*) &old_size;
            for (int i = 0; i < ((sizeof (uint)) / 2); i++)    
            {
                pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            }
        }
        new_size = mp3tagData->frame_Id_size;
        char* pos = (char*) &mp3tagData->frame_Id_size;
        for (int i = 0; i < ((sizeof (uint)) / 2); i++)     
        {
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
        }

        count = fwrite (&mp3tagData->frame_Id_size, 4, 1, mp3tagData->fptr_temp);
        if (count < 1)
        {
            printf ("ERROR: Unable to write on the MP3 file.\n");
            return e_failure;
        }

        char f [3];
        count = fread (f, 1, 3, mp3tagData->fptr_mp3);
        if (count < 3)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return e_failure;
        }
        else{
            count = fwrite (f, 1, 3, mp3tagData->fptr_temp);
            if (count < 3)
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return e_failure;
            }
        }

        count = fwrite (mp3tagData->frame_Id_value, 1, (new_size - 1), mp3tagData->fptr_temp);
        if (count < (new_size - 1))
        {
            printf ("ERROR: Unable to write on the MP3 file.\n");
            return e_failure;
        }
        fseek (mp3tagData->fptr_mp3, (long) (old_size - 1), SEEK_CUR);
    }
    else{
        uint size;
        int count;
        count = fread (&size, 4, 1, mp3tagData->fptr_mp3);
        if (count < 1)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return e_failure;
        }
        else
        {
            count = fwrite (&size, 4, 1, mp3tagData->fptr_temp);
            if (count < 1)
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return e_failure;
            }

        }
        char* pos = (char*) &size;
        for (int i = 0; i < ((sizeof (uint)) / 2); i++)     
        {
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
        }
        char f [3];
        count = fread (f, 1, 3, mp3tagData->fptr_mp3);
        if (count < 3)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return e_failure;
        }
        else
        {
            count = fwrite (f, 1, 3, mp3tagData->fptr_temp);
            if (count < 3)
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return e_failure;
            }
        }

        char str [size-1];
        count = fread (str, 1, (size - 1), mp3tagData->fptr_mp3);
        if (count < (size - 1))
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return e_failure;
        }
        else
        {
            str [size - 1] = '\0';
            count = fwrite (str, 1, (size - 1), mp3tagData->fptr_temp);
            if (count < (size - 1))
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return e_failure;
            }
        }
    }
return e_success;
}

/* * Function to copy remaining data from the source MP3 file to the temporary file.
 * It reads the remaining content after the frames have been copied.
 * Returns e_success on successful copy, e_failure otherwise.
 */

Status copy_remaining_data (TagData* mp3tagData)
{
    char ch [1024];
    int count;
    long start = ftell (mp3tagData->fptr_mp3);
    fseek (mp3tagData->fptr_mp3, 0L, SEEK_END);
    long end = ftell (mp3tagData->fptr_mp3);
    fseek (mp3tagData->fptr_mp3, start, SEEK_SET);
    while ((ftell (mp3tagData->fptr_mp3)) != end)
    {
        count = fread (ch, 1, 1024, mp3tagData->fptr_mp3);
        if (count < 0)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return e_failure;
        }
        else
        {
            fwrite (ch, 1, count, mp3tagData->fptr_temp);
        }
    }
    return e_success;
}


/* * Function to copy the complete data from the temporary file back to the source MP3 file.
 * It ensures that all changes made in the temporary file are reflected in the source file.
 * Returns e_success on successful copy, e_failure otherwise.
 */

Status copy_back_to_src (TagData* mp3tagData)
{
    fseek (mp3tagData->fptr_mp3, 0L, SEEK_SET);
    fseek (mp3tagData->fptr_temp, 0L, SEEK_SET);
    int count;
    char ch [1024];
    long start = ftell (mp3tagData->fptr_temp);
    fseek (mp3tagData->fptr_temp, 0L, SEEK_END);
    long end = ftell (mp3tagData->fptr_temp);
    fseek (mp3tagData->fptr_temp, 0L, SEEK_SET);
    while ((ftell (mp3tagData->fptr_temp)) != end)
    {
        count = fread (ch, 1, 1024, mp3tagData->fptr_temp);
        if (count < 0)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return e_failure;
        }
        else
        {
            fwrite (ch, 1, count, mp3tagData->fptr_mp3);
        }
    }
    return e_success;
}

/* Function to open the source MP3 file for editing.
 * It checks if the file exists and is accessible.
 * Returns e_success on successful opening, e_failure otherwise.
 */

Status open_file_for_edit(const char *filename, FILE **fptr) {
    if (filename == NULL) {
        printf("ERROR: No filename provided for editing.\n");
        return e_failure;
    }
    *fptr = fopen(filename, "r+"); 
        if (*fptr == NULL) {
        printf("ERROR: Unable to open file %s for editing.\n", filename);
        return e_failure;
    }
    return e_success;
}