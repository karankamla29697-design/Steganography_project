#ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h>
#include "types.h"

/* Macro Definitions */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 20

/* Structure to store encoding information */

typedef struct _EncodeInfo
{
    /* Source Image Info */

    char *src_image_fname;
    FILE *fptr_src_image;

    uint image_capacity;
    uint bits_per_pixel;

    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */

    char *secret_fname;
    FILE *fptr_secret;

    char extn_secret_file[MAX_FILE_SUFFIX];

    char secret_data[MAX_SECRET_BUF_SIZE];

    long size_secret_file;

    /* Stego Image Info */

    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;


/* Function Prototypes */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate encode arguments */
Status read_and_validate_encode_args(int argc,
                                     char *argv[],
                                     EncodeInfo *encInfo);

/* Perform complete encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Open required files */
Status open_files(EncodeInfo *encInfo);

/* Check image capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get secret file size */
uint get_file_size(FILE *fptr);

/* Copy BMP header */
Status copy_bmp_header(FILE *fptr_src_image,
                       FILE *fptr_dest_image);

/* Encode magic string */
Status encode_magic_string(const char *magic_string,
                           EncodeInfo *encInfo);

/* Encode extension size */
Status encode_secret_file_extn_size(int size,
                                    EncodeInfo *encInfo);

/* Encode secret file extension */
Status encode_secret_file_extn(const char *file_extn,
                               EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size,
                               EncodeInfo *encInfo);

/* Encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode data into image */
Status encode_data_to_image(char *data,
                            int size,
                            FILE *fptr_src_image,
                            FILE *fptr_stego_image);

/* Encode one byte */
Status encode_byte_to_lsb(char data,
                          char *image_buffer);

/* Encode integer */
Status encode_int_to_lsb(int size,
                         char *image_buffer);

/* Copy remaining image bytes */
Status copy_remaining_img_data(FILE *fptr_src,
                               FILE *fptr_dest);

#endif