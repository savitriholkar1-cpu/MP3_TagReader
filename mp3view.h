#ifndef MP3VIEW_H
#define MP3VIEW_H

#include<stdio.h>
#include "types.h"

typedef struct
{
    FILE *fptr_mp3;
    char *filename;
}MP3ViewInfo;

//Function to check the operation type
OperationType check_operation_type(char *symbol);

//Function to open the MP3 file
Status open_mp3_file(MP3ViewInfo *mp3Info);

//Function to check ID3 header
Status check_id3_header(MP3ViewInfo *mp3Info);

//Function to view MP3 tags
Status View_mp3(MP3ViewInfo *mp3Info);

#endif