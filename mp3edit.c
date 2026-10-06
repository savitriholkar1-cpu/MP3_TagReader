
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "mp3edit.h"

Status edit_mp3(MP3EditInfo *editInfo)
{
    if(validate_edit_input(editInfo) == e_failure)
    {
        return e_failure;
    }

    if(copy_mp3_header(editInfo) == e_failure)
    {
        return e_failure;
    }

    if(edit_frame(editInfo) == e_failure)
    {
        return e_failure;
    }

    if(save_edited_mp3(editInfo) == e_failure)
    {
        return e_failure;
    }

    return e_success;
}

Status validate_edit_input(MP3EditInfo *editInfo)
{
    char header[10];
    char *extension;

    /*Open source file*/
    editInfo->fptr_mp3 = fopen(editInfo->filename, "rb");

    if(editInfo->fptr_mp3 == NULL)
    {
        printf("ERROR: Unable to open MP3 file\n");
        return e_failure;
    }

    /*Check .mp3 extension*/
    extension = strrchr(editInfo->filename, '.');

    if(extension == NULL || strcmp(extension, ".mp3") != 0)
    {
        printf("ERROR: Input file should be .mp3 file\n");
        fclose(editInfo->fptr_mp3);
        return e_failure;
    }

    /*Check ID3 header*/
    if(fread(header, 1, 10, editInfo->fptr_mp3) != 10)
    {
        printf("ERROR: Unable to read MP3 header\n");
        fclose(editInfo->fptr_mp3);
        return e_failure;
    }

    if(header[0] != 'I' || header[1] != 'D' || header[2] != '3')
    {
        printf("ERROR: Invalid MP3 file\n");
        fclose(editInfo->fptr_mp3);
        return e_failure;
    }

    rewind(editInfo->fptr_mp3);

    return e_success;
}

Status copy_mp3_header(MP3EditInfo *editInfo)
{
    char header[10];

    /*Open destination file*/
    editInfo->fptr_dest = fopen("temp.mp3", "wb");

    if(editInfo->fptr_dest == NULL)
    {
        printf("ERROR: Unable to create destination file\n");
        fclose(editInfo->fptr_mp3);
        return e_failure;
    }

    /*Move to beginning*/
    rewind(editInfo->fptr_mp3);

    /*Read header*/
    if(fread(header, 1, 10, editInfo->fptr_mp3) != 10)
    {
        printf("ERROR: Unable to read header\n");
        return e_failure;
    }

    /*Copy header to destination*/
    fwrite(header, 1, 10, editInfo->fptr_dest);

    return e_success;
}

Status edit_frame(MP3EditInfo *editInfo)
{
    char tag[5];
    unsigned char size_buff[4];
    unsigned int size;
    char extra[3];
    char data[1024];

    /*Move to 10th offset*/
    fseek(editInfo->fptr_mp3, 10, SEEK_SET);

    while(1)
    {

        /*Read 4 bytes tag*/
        if(fread(tag, 1, 4, editInfo->fptr_mp3) != 4)
        {
            break;
        }

        tag[4]='\0';

        /*Read 4 bytes size*/
        if(fread(size_buff, 1, 4, editInfo->fptr_mp3) != 4)
        {
            printf("ERROR: Unable to read size\n");
            return e_failure;
        }

        /*Convert big endian size*/
        size = ((size_buff[0]<<24) | (size_buff[1]<<16) | (size_buff[2]<<8) | size_buff[3]);

        /*Read next 3 bytes*/
        if(fread(extra, 1, 3, editInfo->fptr_mp3) != 3)
        {
            printf("ERROR: Unable to read frame information\n");
            return e_failure;
        }

        /*Read size-1 bytes of data*/
        if(fread(data, 1, size-1, editInfo->fptr_mp3) != size-1)
        {
            printf("ERROR: Unable to read frame data\n");
            return e_failure;
        }

        /*Check required tag*/
        if(strcmp(tag, editInfo->frame_id) == 0)
        {
            unsigned int new_size;
            unsigned char new_size_buff[4];

            /*Calculate new data size*/
            new_size = strlen(editInfo->new_data) + 1;

                /*Convert new size to big endian*/
            new_size_buff[0] = (new_size >> 24) & 0xFF;
            new_size_buff[1] = (new_size >> 16) & 0xFF;
            new_size_buff[2] = (new_size >> 8) & 0xFF;
            new_size_buff[3] = new_size & 0xFF;

            /*Copy tag*/
            fwrite(tag, 1, 4, editInfo->fptr_dest);

            /*Write new size*/
            fwrite(new_size_buff, 1, 4, editInfo->fptr_dest);

            /*Copy 3 bytes*/
            fwrite(extra, 1, 3, editInfo->fptr_dest);

            /*Write new data*/
            fwrite(editInfo->new_data, 1, strlen(editInfo->new_data),editInfo->fptr_dest);

            break;

        }
        else
        {
            /*Copy tag*/
            fwrite(tag, 1, 4, editInfo->fptr_dest);

            /*Copy size*/
            fwrite(size_buff, 1, 4, editInfo->fptr_dest);

            /*Copy next 3 bytes*/
            fwrite(extra, 1, 3, editInfo->fptr_dest);

            /*Copy data*/
            fwrite(data, 1, size-1, editInfo->fptr_dest);
        }
    }

    /*Copy remaining data*/
    {
        char buffer[1024];
        size_t bytes;

        while((bytes = fread(buffer, 1, sizeof(buffer), editInfo->fptr_mp3)) > 0)
        {
            fwrite(buffer, 1, bytes, editInfo->fptr_dest);
        }
    }

    return e_success;
}

Status save_edited_mp3(MP3EditInfo *editInfo)
{
    /*Close files*/
    fclose(editInfo->fptr_mp3);
    fclose(editInfo->fptr_dest);

    /*Remove original file*/
    if(remove(editInfo->filename) != 0)
    {
        printf("ERROR: Unable to remove original file\n");
        return e_failure;
    }

    /*Rename temp file*/
    if(rename("temp.mp3", editInfo->filename) != 0)
    {
        printf("ERROR: Unable to rename temp file\n");
        return e_failure;
    }

    return e_success;
}