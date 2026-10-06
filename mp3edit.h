
#ifndef MP3EDIT_H
#define MP3EDIT_H

#include <stdio.h>
#include "types.h"

typedef struct
{
    FILE *fptr_mp3;
    FILE *fptr_dest;

    char *filename;
    char *new_data;
    char frame_id[5];

}MP3EditInfo;

/*Check and validate edit inputs*/
Status validate_edit_input(MP3EditInfo *editInfo);

/*Do editing*/
Status edit_mp3(MP3EditInfo *editInfo);

/*Copy header*/
Status copy_mp3_header(MP3EditInfo *editInfo);

Status edit_frame(MP3EditInfo *editInfo);

Status save_edited_mp3(MP3EditInfo *editInfo);

#endif


