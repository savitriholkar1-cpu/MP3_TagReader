#include <stdio.h>
#include <string.h>

#include "types.h"
#include "mp3view.h"
#include "mp3edit.h"

void print_help(void)
{
    printf("\n--------------- MP3 TAG READER ---------------\n");

    printf("\nVIEW OPERATION:\n");
    printf("./a.out -v song.mp3\n");

    printf("\nEDIT OPERATION:\n");
    printf("./a.out -e -t \"New Title\" song.mp3\n");
    printf("./a.out -e -a \"New Artist\" song.mp3\n");
    printf("./a.out -e -A \"New Album\" song.mp3\n");
    printf("./a.out -e -y \"2026\" song.mp3\n");
    printf("./a.out -e -g \"Rock\" song.mp3\n");
    printf("./a.out -e -c \"New Comment\" song.mp3\n");

    printf("\nHELP:\n");
    printf("./a.out --help\n");

    printf("\n------------------------------------------------\n");
}

int main(int argc, char *argv[])
{
    OperationType operation;

    MP3ViewInfo viewInfo;
    MP3EditInfo editInfo;

    if (argc < 2)
    {
        printf("ERROR: Invalid arguments\n");
        print_help();
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
            printf("ERROR: Usage: ./a.out -v song.mp3\n");
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
        }
        else if (strcmp(argv[2], "-a") == 0)
        {
            strcpy(editInfo.frame_id, "TPE1");
        }
        else if (strcmp(argv[2], "-A") == 0)
        {
            strcpy(editInfo.frame_id, "TALB");
        }
        else if (strcmp(argv[2], "-y") == 0)
        {
            strcpy(editInfo.frame_id, "TYER");
        }
        else if (strcmp(argv[2], "-g") == 0)
        {
            strcpy(editInfo.frame_id, "TCON");
        }
        else if (strcmp(argv[2], "-c") == 0)
        {
            strcpy(editInfo.frame_id, "COMM");
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

        return e_success;
    }

    printf("ERROR: Unsupported operation\n");
    print_help();

    return e_failure;
}