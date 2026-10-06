#include <stdio.h>
#include <string.h>

#include "types.h"
#include "mp3view.h"
#include "mp3edit.h"

void print_invalid_arguments(void)
{
    printf("\n------------------------------------------------\n");
    
    printf("\nERROR: ./a.out : INVALID ARGUMENTS\n");
    printf("USAGE : \n");
    printf("To view please pass like: ./a.out -v mp3filename\n");
    printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
    printf("To get help pass like: ./a.out --help\n");
    printf("\n------------------------------------------------\n");
}

void print_help(void)
{
    printf("\n-------------------------HELP MENU-------------------------\n");

    printf("\n1. -v -> to view mp3 file contents\n");

    printf("2. -e -> to edit mp3 file contents\n");

    printf("\t2.1. -t -> to edit song title\n");
    printf("\t2.2. -a -> to edit artist name\n");
    printf("\t2.3. -A -> to edit album name\n");
    printf("\t2.4. -y -> to edit year\n");
    printf("\t2.5. -m -> to edit music\n");
    printf("\t2.6. -c -> to edit comment\n");

    printf("\n----------------------------------------------------------\n");

}

int main(int argc, char *argv[])
{
    OperationType operation;

    MP3ViewInfo viewInfo;
    MP3EditInfo editInfo;

    if (argc < 2)
    {
        print_invalid_arguments();
        return e_failure;
    }

    operation = check_operation_type(argv[1]);

    if (operation == e_help)
    {
        print_help();
        return e_success;
    }

    if (operation == e_view)
    {
        if (argc != 3)
        {
            printf("ERROR: Usage: ./a.out -v sample.mp3\n");
            return e_failure;
        }

        viewInfo.filename = argv[2];

        if (open_mp3_file(&viewInfo) == e_failure)
        {
            return e_failure;
        }

        if (check_id3_header(&viewInfo) == e_failure)
        {
            fclose(viewInfo.fptr_mp3);
            return e_failure;
        }

        if (View_mp3(&viewInfo) == e_failure)
        {
            fclose(viewInfo.fptr_mp3);
            return e_failure;
        }

        fclose(viewInfo.fptr_mp3);

        return e_success;
    }

    if (operation == e_edit)
    {
        if (argc != 5)
        {
            printf("ERROR: Invalid edit arguments\n");
            print_help();
            return e_failure;
        }

        editInfo.filename = argv[4];
        editInfo.new_data = argv[3];

        if (strcmp(argv[2], "-t") == 0)
        {
            strcpy(editInfo.frame_id, "TIT2");

            printf("\n------------------------SELECTED EDIT DETAILS------------------------\n");
            printf("\n-----------SELECTED EDIT OPTION------\n");
            printf("\n-----------CHANGE THE TITLE----------\n");
            printf("\nTITLE     : %s\n",argv[3]);
        }
        else if (strcmp(argv[2], "-a") == 0)
        {
            strcpy(editInfo.frame_id, "TPE1");

            printf("\n------------------------SELECTED EDIT DETAILS------------------------\n");
            printf("\n-----------SELECTED EDIT OPTION------\n");
            printf("\n-----------CHANGE THE ARTIST---------\n");
            printf("\nARTIST     : %s\n",argv[3]);
        }
        else if (strcmp(argv[2], "-A") == 0)
        {
            strcpy(editInfo.frame_id, "TALB");

            printf("\n------------------------SELECTED EDIT DETAILS------------------------\n");
            printf("\n-----------SELECTED EDIT OPTION------\n");
            printf("\n-----------CHANGE THE ALBUM----------\n");
            printf("\nALBUM     : %s\n",argv[3]);
        }
        else if (strcmp(argv[2], "-y") == 0)
        {
            strcpy(editInfo.frame_id, "TYER");

            printf("\n------------------------SELECTED EDIT DETAILS------------------------\n");
            printf("\n-----------SELECTED EDIT OPTION------\n");
            printf("\n-----------CHANGE THE YEAR-----------\n");
            printf("\nYEAR     : %s\n",argv[3]);
        }
        else if (strcmp(argv[2], "-m") == 0)
        {
            strcpy(editInfo.frame_id, "TCON");

            printf("\n------------------------SELECTED EDIT DETAILS------------------------\n");
            printf("\n-----------SELECTED EDIT OPTION------\n");
            printf("\n-----------CHANGE THE MUSIC----------\n");
            printf("\nMUSIC     : %s\n",argv[3]);
        }
        else if (strcmp(argv[2], "-c") == 0)
        {
            strcpy(editInfo.frame_id, "COMM");

            printf("\n------------------------SELECTED EDIT DETAILS------------------------\n");
            printf("\n-----------SELECTED EDIT OPTION------\n");
            printf("\n-----------CHANGE THE COMMENT--------\n");
            printf("\nCOMMENT     : %s\n",argv[3]);
        }
        else
        {
            printf("ERROR: Invalid edit option\n");
            print_help();
            return e_failure;
        }

        if (edit_mp3(&editInfo) == e_failure)
        {
            return e_failure;
        }

        if (strcmp(argv[2], "-t") == 0)
            printf("\n----------TITLE CHANGED SUCCESSFULLY----------\n");
        else if (strcmp(argv[2], "-a") == 0)
            printf("\n----------ARTIST CHANGED SUCCESSFULLY---------\n");
        else if (strcmp(argv[2], "-A") == 0)
            printf("\n----------ALBUM CHANGED SUCCESSFULLY----------\n");
        else if (strcmp(argv[2], "-y") == 0)
            printf("\n----------YEAR CHANGED SUCCESSFULLY-----------\n");
        else if (strcmp(argv[2], "-m") == 0)
            printf("\n----------MUSIC CHANGED SUCCESSFULLY----------\n");
        else if (strcmp(argv[2], "-c") == 0)
            printf("\n----------COMMENT CHANGED SUCCESSFULLY---------\n");

        return e_success;
    }

    printf("ERROR: Unsupported operation\n");
    print_help();

    return e_failure;
}