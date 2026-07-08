#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
#include "types.h"

/* Open required files */
Status open_decode_files(DecodeInfo *decInfo)
{
    // Open source image in binary read mode
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");

    // Check source image opened or not
    if(decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        printf("ERROR : Unable to open %s\n", decInfo->src_image_fname);
        return e_failure;
    }

    // Open decoded file in binary write mode
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "wb");

    // Check decoded file created or not
    if(decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        printf("ERROR : Unable to create %s\n", decInfo->secret_fname);
        return e_failure;
    }

    // Return success
    return e_success;
}

/* Read and validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *str;

    // Get source image extension
    str = strrchr(argv[2], '.');

    // Check source image is bmp or not
    if(str != NULL && strcmp(str, ".bmp") == 0)
    {
        // Store source image name
        decInfo->src_image_fname = argv[2];
    }
    else
    {
        // Print error message
        printf("ERROR : Source image should be .bmp\n");
        return e_failure;
    }

    // Check output file name is given
    if(argv[3] != NULL)
    {
        // Store output file name
        decInfo->secret_fname = argv[3];
    }
    else
    {
        // Store default file name
        decInfo->secret_fname = "decoded.txt";
    }

    // Return success
    return e_success;
}

/* Perform complete decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    int extn_size;

    // Open all required files
    if(open_decode_files(decInfo) == e_failure)
    {
        printf("ERROR : Unable to open files\n");
        return e_failure;
    }

    // Skip BMP header
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);

    // Decode magic string
    if(decode_magic_string(decInfo) == e_failure)
    {
        printf("ERROR : Magic String Mismatch\n");
        return e_failure;
    }

    // Decode extension size
    if(decode_secret_file_extn_size(&extn_size, decInfo) == e_failure)
    {
        printf("ERROR : Unable to decode extension size\n");
        return e_failure;
    }

    // Decode extension
    if(decode_secret_file_extn(decInfo->extn_secret_file, extn_size, decInfo) == e_failure)
    {
        printf("ERROR : Unable to decode extension\n");
        return e_failure;
    }

    // Decode secret file size
    if(decode_secret_file_size(&decInfo->size_secret_file, decInfo) == e_failure)
    {
        printf("ERROR : Unable to decode file size\n");
        return e_failure;
    }

    // Decode secret file data
    if(decode_secret_file_data(decInfo) == e_failure)
    {
        printf("ERROR : Unable to decode file data\n");
        return e_failure;
    }

    // Close source image
    fclose(decInfo->fptr_src_image);

    // Close decoded file
    fclose(decInfo->fptr_secret);

    // Print success message
    printf("Decoding Completed Successfully\n");

    // Return success
    return e_success;
}
/* Decode one byte from image LSB */
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    // Initialize decoded data
    *data = 0;

    // Run loop for 8 bits
    for(int i = 0; i < 8; i++)
    {
        // Shift decoded data by one bit
        *data = *data << 1;

        // Read LSB from image buffer
        *data = *data | (image_buffer[i] & 1);
    }

    // Return success
    return e_success;
}

/* Decode integer from image LSB */
Status decode_int_from_lsb(int *data, char *image_buffer)
{
    // Initialize integer
    *data = 0;

    // Run loop for 32 bits
    for(int i = 0; i < 32; i++)
    {
        // Shift integer by one bit
        *data = *data << 1;

        // Read LSB from image buffer
        *data = *data | (image_buffer[i] & 1);
    }

    // Return success
    return e_success;
}

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    // Create image buffer
    char image_buffer[8];

    // Create magic string buffer
    char magic[strlen(MAGIC_STRING) + 1];

    // Get magic string length
    int len = strlen(MAGIC_STRING);

    // Decode every character
    for(int i = 0; i < len; i++)
    {
        // Read 8 bytes from source image
        fread(image_buffer, 8, 1, decInfo->fptr_src_image);

        // Decode one character
        decode_byte_from_lsb(&magic[i], image_buffer);
    }

    // Add string terminator
    magic[len] = '\0';

    // Compare magic strings
    if(strcmp(magic, MAGIC_STRING) == 0)
    {
        // Print success message
        printf("Magic String Matched\n");

        // Return success
        return e_success;
    }

    // Print failure message
    printf("Magic String Mismatch\n");

    // Return failure
    return e_failure;
}
/* Decode secret file extension size */
Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo)
{
    // Create image buffer
    char image_buffer[32];

    // Read 32 bytes from source image
    fread(image_buffer, 32, 1, decInfo->fptr_src_image);

    // Decode extension size
    decode_int_from_lsb(size, image_buffer);

    // Print extension size
    printf("Secret File Extension Size = %d\n", *size);

    // Return success
    return e_success;
}

/* Decode secret file extension */
Status decode_secret_file_extn(char *extn, int size, DecodeInfo *decInfo)
{
    // Create image buffer
    char image_buffer[8];

    // Run loop for extension size
    for(int i = 0; i < size; i++)
    {
        // Read 8 bytes from source image
        fread(image_buffer, 8, 1, decInfo->fptr_src_image);

        // Decode one character
        decode_byte_from_lsb(&extn[i], image_buffer);
    }

    // Add string terminator
    extn[size] = '\0';

    // Print decoded extension
    printf("Secret File Extension = %s\n", extn);

    // Return success
    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(long *size, DecodeInfo *decInfo)
{
    // Create image buffer
    char image_buffer[32];

    // Create temporary integer
    int file_size;

    // Read 32 bytes from source image
    fread(image_buffer, 32, 1, decInfo->fptr_src_image);

    // Decode file size
    decode_int_from_lsb(&file_size, image_buffer);

    // Store decoded file size
    *size = file_size;

    // Print decoded file size
    printf("Secret File Size = %ld\n", *size);

    // Return success
    return e_success;
}
/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    // Create image buffer
    char image_buffer[8];

    // Create character variable
    char ch;

    // Run loop till secret file size
    for(long i = 0; i < decInfo->size_secret_file; i++)
    {
        // Read 8 bytes from source image
        if(fread(image_buffer, 8, 1, decInfo->fptr_src_image) != 1)
        {
            // Return failure if read fails
            return e_failure;
        }

        // Decode one character
        decode_byte_from_lsb(&ch, image_buffer);

        // Write decoded character to output file
        fputc(ch, decInfo->fptr_secret);
    }

    // Print success message
    printf("Secret File Data Decoded Successfully\n");

    // Return success
    return e_success;
}