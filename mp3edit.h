
#ifndef MP3EDIT_H
#define MP3EDIT_H

#include <stdio.h>
#include "types.h"

typedef struct
{

    FILE *fptr_mp3;
    char *filename;
    char *new_data;
    char frame_id[5];

}MP3EditInfo;

Status edit_mp3(MP3EditInfo *editInfo);

#endif


