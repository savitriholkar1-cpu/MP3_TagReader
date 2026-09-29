#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mp3view.h"

OperationType check_operation_type(char *symbol)
{
    if (strcmp(symbol, "-v") == 0)
    {
        return e_view;
    }
    else if (strcmp(symbol, "-e") == 0)
    {
        return e_edit;
    }
    else if (strcmp(symbol, "--help") == 0)
    {
        return e_help;
    }

    return e_unsupported;
}

Status open_mp3_file(MP3ViewInfo *mp3Info)
{
    mp3Info->fptr_mp3 = fopen(mp3Info->filename, "rb");

    if (mp3Info->fptr_mp3 == NULL)
    {
        printf("ERROR: Unable to open MP3 file\n");
        return e_failure;
    }

    return e_success;
}

Status check_id3_header(MP3ViewInfo *mp3Info)
{
    char header[4];

    rewind(mp3Info->fptr_mp3);

    if (fread(header, 1, 3, mp3Info->fptr_mp3) != 3)
    {
        printf("ERROR: Unable to read ID3 header\n");
        return e_failure;
    }

    header[3] = '\0';

    if (strcmp(header, "ID3") != 0)
    {
        printf("ERROR: Invalid MP3 file\n");
        return e_failure;
    }

    return e_success;
}

Status View_mp3(MP3ViewInfo *mp3Info)
{
    char tag[5];
    unsigned char size_buff[4];
    unsigned int size;

    char *data[6]={NULL};

    char *tag_names[6] =
    {
        "TIT2",
        "TPE1",
        "TALB",
        "TYER",
        "TCON",
        "COMM"
    };

    int i = 0;
    int j;

    printf("\n<-----------------------------Start of view------------------------------->\n");
    printf("-------------------------------------------------------------------------\n");
    printf("SI.No\t|\tTAG\t|\tContent\n");
    printf("-------------------------------------------------------------------------\n");

    /* Move offset to 10th position */
    if (fseek(mp3Info->fptr_mp3, 10, SEEK_SET) != 0)
    {
        return e_failure;
    }

    /* Read all 6 required tags */
    while (i < 6)
    {
        /* Read 4 bytes of tag */
        if (fread(tag, 4, 1, mp3Info->fptr_mp3) != 1)
        {
            return e_failure;
        }

        tag[4] = '\0';

        /* Read 4 bytes of size */
        if (fread(size_buff, 4, 1, mp3Info->fptr_mp3) != 1)
        {
            return e_failure;
        }

        /* Convert endianess */
        size = ((size_buff[0] << 24) |
                (size_buff[1] << 16) |
                (size_buff[2] << 8) |
                size_buff[3]);

        /* Skip 3 bytes
           2 bytes flag + 1 byte '\0' */
        if (fseek(mp3Info->fptr_mp3, 3, SEEK_CUR) != 0)
        {
            return e_failure;
        }

        /* Check tag */
        for (j = 0; j < 6; j++)
        {
            if (strcmp(tag, tag_names[j]) == 0)
            {
                data[j] = malloc(size);

                if (data[j] == NULL)
                {
                    return e_failure;
                }

                /* Read size - 1 bytes of metadata */
                if (fread(data[j], 1, size - 1,
                          mp3Info->fptr_mp3) != size - 1)
                {
                    free(data[j]);
                    return e_failure;
                }

                data[j][size - 1] = '\0';

                i++;

                break;
            }
        }

        /* If tag is not required, skip metadata */
        if (j == 6)
        {
            if (fseek(mp3Info->fptr_mp3, size - 1, SEEK_CUR) != 0)
            {
                return e_failure;
            }
        }
    }

    /* Print all 6 tags */
    for (int k = 0; k < 6; k++)
    {
        printf("%d\t|\t%s\t|\t%s\n",
               k + 1,
               tag_names[k],
               data[k]);
    }

    printf("-------------------------------------------------------------------------\n");
    printf("\n<-----------------------------End of view--------------------------------->\n");

    /* Free allocated memory */
    for (int k = 0; k < 6; k++)
    {
        free(data[k]);
    }

    return e_success;
}