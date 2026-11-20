#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

typedef struct _DecodeInfo{
    /*For Stego Image*/
    char *stego_image_fname;
    FILE *fptr_stego_image;
    /*Magic String*/
    char magic[50];
    int magic_size;


    int extn_size;
    char extn_secret_file[50];

    long size_secret_file;

    char *secret_fname;
    FILE *fptr_secret;

} DecodeInfo;


/* Function prototypes */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status open_decode_files(DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string_size(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

Status open_secret_file(DecodeInfo *decInfo);

char decode_byte_from_lsb(char *image_buffer);

#endif