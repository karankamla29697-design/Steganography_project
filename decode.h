#ifndef DECODE_H
#define DECODE_H

// Include standard input output header
#include <stdio.h>

// Include user defined types
#include "types.h"

// Maximum secret file extension size
#define MAX_FILE_SUFFIX 20

// Structure to store decoding information
typedef struct _DecodeInfo
{
    // Source image file name
    char *src_image_fname;

    // Source image file pointer
    FILE *fptr_src_image;

    // Output secret file name
    char *secret_fname;

    // Output secret file pointer
    FILE *fptr_secret;

    // Secret file extension
    char extn_secret_file[MAX_FILE_SUFFIX];

    // Secret file size
    long size_secret_file;

} DecodeInfo;

/* Function Prototypes */

// Read and validate decode arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

// Perform complete decoding
Status do_decoding(DecodeInfo *decInfo);

// Open required files
Status open_decode_files(DecodeInfo *decInfo);

// Decode magic string
Status decode_magic_string(DecodeInfo *decInfo);

// Decode secret file extension size
Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo);

// Decode secret file extension
Status decode_secret_file_extn(char *extn, int size, DecodeInfo *decInfo);

// Decode secret file size
Status decode_secret_file_size(long *size, DecodeInfo *decInfo);

// Decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo);

// Decode one byte from image LSB
Status decode_byte_from_lsb(char *data, char *image_buffer);

// Decode integer from image LSB
Status decode_int_from_lsb(int *data, char *image_buffer);

#endif