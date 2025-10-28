
/******************************************************************************
 * Name: Md Mustafa
 * Batch Id: 25008_052
 
 * Project: MP3 Tag Reader and Editor
 * Last Modified: August 13, 2025
 * Description: A C program to view and edit ID3 tags in MP3 files from the command line.
 * Usage:
 * 1. View Tags:
 *   ./mp3_tag_reader -v <file_name.mp3>
 * 2. Edit Tags:
 *  ./mp3_tag_reader -e <modifier> "new_value" <file_name.mp3>
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "view.h"
#include "types.h"
#include "edit.h"

void print_equalto();
// Function to print a separator line
void Print_view();
// Function to check the operation type based on command line arguments
OperationType check_operation(char *argv[]);
// Function to validate the MP3 header
Status Check_header_file(MP_READ *mp3_reader);
// Function to display tags
Status View_tags(MP_READ *mp3_reader);

/* Main function to handle the MP3 tag reader and editor operations.
 * It checks the command line arguments to determine the operation type
 * (view, edit, or help) and calls the appropriate functions.
 */

int main(int argc, char *argv[])
{
    MP_READ mp3_reader;
    if (argc == 1){
        Print_view();
        return 0;
    }
    OperationType operation = check_operation(argv);
    if (operation == e_view){
        mp3_reader.file_name = argv[2];
        if (open_files(&mp3_reader) == e_success){
            if (Check_header_file(&mp3_reader) == e_success){
                fseek(mp3_reader.fptr_mp3, 10, SEEK_SET);
                if (store_tags(&mp3_reader) == e_success){
                 
                    if (View_tags(&mp3_reader) == e_success){
                        printf("-----------------------Displayed all the tags successfully✅--------------------\n\n");
                    }
                }
            }
            fclose(mp3_reader.fptr_mp3);
        }
    }else if (operation == e_help){
        Print_view();
    }else if (strcmp(argv[1], "-e") == 0){
        if (argc < 5){
            printf("ERROR :: INVALID ARGUMENTS to Edit\nPlease Refer help page once\n");
            return 1;
        }
        const char *modifiers[] = {"-t", "-a", "-A", "-y", "-c", "-G"};
        int valid_modifier = 0;

        for (int i = 0; i < 6; i++){
            if (strcmp(argv[2], modifiers[i]) == 0){
                valid_modifier = 1;
                break;
            }
        }

        if (!valid_modifier){
            printf("ERROR :: Invalid modifier '%s'\n", argv[2]);
            return 1;
        }
        for (int i = 0; i < 6; i++){
            if (strcmp(argv[4], modifiers[i]) == 0){
                printf("ERROR: Filename cannot be a modifier.\n");
                return 1;
            }
        }
        FILE *edit_fp = fopen(argv[4], "r+");
        if (!edit_fp){
            printf("ERROR: Cannot open file %s for editing.\n", argv[4]);
            return 1;
        }
        fclose(edit_fp);

        TagData tagData;
        if (read_and_validate_mp3_file_args(argv, &tagData) == e_success){
            if (edit_tag(argv, &tagData) == e_success){
                print_equalto();
                printf("\t\t   Edit operation completed successfully✅\n");
                print_equalto();
            }else{
                printf("ERROR :: Failed to edit tag\n");
            }
        }else{
            printf("ERROR :: Invalid arguments for editing\n");
        }
    }else{
        printf("ERROR: Unknown operation. Please refer help.\n");
        Print_view();
    }
    return 0;
}

/* Function to check the operation type based on command line arguments.
 * It returns e_view for view operation, e_help for help, and e_edit for edit.
 * It also validates the MP3 file name and extension.
 */

OperationType check_operation(char *argv[])
{
    if (strcmp(argv[1], "--help") == 0){
        return e_help;
    }
    if (strcmp(argv[1], "-v") == 0){
        printf("\n-----------------------------🔓 Selected View operation-------------------------\n\n");

        if (argv[2] == NULL){
            printf("ERROR :: mp3 file name is not present\n");
            exit(1);
        }

        if (strstr(argv[2], ".mp3") == NULL){
            printf("ERROR :: .mp3 extension is not present\n");
            exit(1);
        }
        return e_view;
    }
    return e_edit;
}

/* Function to check the MP3 file header.
 * It reads the first 3 bytes and checks if they match "ID3".
 * Returns e_success if the header is valid, e_failure otherwise.
 */

Status Check_header_file(MP_READ *mp3_reader)
{
    char buffer[3];
    rewind(mp3_reader->fptr_mp3);
    fread(buffer, 3, 1, mp3_reader->fptr_mp3);

    if (strncmp(buffer, "ID3", 3) != 0)
        return e_failure;

    return e_success;
}

/* Function to display tags from the MP3 file.
 * It reads the tags and prints them in a formatted manner.
 * Returns e_success on successful display, e_failure otherwise.
 */

Status View_tags(MP_READ *mp3_reader)
{
    print_equalto();
    printf("\t\t\t    MP3 Tag Reader and Editor\n");
    print_equalto();
    printf("\n");
    printf("%-20s:\t%s\n", "TITLE", mp3_reader->title_name);
    printf("%-20s:\t%s\n", "ARTIST", mp3_reader->artist_name);
    printf("%-20s:\t%s\n", "ALBUM", mp3_reader->album_name);
    printf("%-20s:\t%s\n", "YEAR", mp3_reader->Year);
    printf("%-20s:\t%s\n", "GENRE", mp3_reader->Content_type);
    printf("%-20s:\t%s\n", "LANGUAGE", mp3_reader->Comment);
    print_equalto();
    printf("\n");
    return e_success;
}

void print_equalto()
{
    for (int i = 0; i < 80; i++)
        printf("-");
    printf("\n");
}

/* Function to print help instructions.
 * It displays the usage and available operations for the MP3 tag reader and editor.
 */

void Print_view()
{
    printf("--------------------------------- HELP MENU ---------------------------------\n");

    printf("This program allows you to view and edit ID3 tags in MP3 files.\n");
    printf("USAGE:\n");
    printf("1. View Tags:\n");
    printf("   ./mp3_tag_reader -v <file_name.mp3>\n");
    printf("2. Edit Tags:\n");
    printf("   ./mp3_tag_reader -e <modifier> \"new_value\" <file_name.mp3>\n\n");
    printf("MODIFIERS:\n");
    printf("   -t\tEdit Title\n");
    printf("   -a\tEdit Artist\n");
    printf("   -A\tEdit Album\n");
    printf("   -y\tEdit Year (must be a four-digit number)\n");
    printf("   -c\tEdit Genre (only alphabetic characters allowed)\n");
    printf("   -G\tEdit Language(only alphabetic characters allowed)\n");
    print_equalto();
}