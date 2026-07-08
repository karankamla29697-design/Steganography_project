#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }


    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)

{

    // 1st check whether the argv[2] contains '.' if it contains using strchr return the add to str
    char *str = strchr(argv[2],'.');
  
    //by using strcmp check whether .bmp is present or not 
    if(strcmp(str,".bmp") == 0)
    {
        // update the source image name in the structure 
        encInfo->src_image_fname = argv[2];
    }

    else
    {
        printf("ERROR : argv[2] contains wrong file name\n");
        return e_failure;
    }

    // check whether the aqrgv[3] contains '.' or not
    if(strchr(argv[3],'.') != NULL)
    {
        //update the secret file name in the structure 
        encInfo->secret_fname = argv[3];
    }

    else
    {
        printf("ERROR : argv[3] contains wrong file name\n");
        return e_failure;
    }

    // check whether the argv[4] is not equal to NULL or not
    if(argv[4] != NULL)
    {
        // using strchr check whether the argv[4] contains '.' or not , if it is present store the add to str
        str = strchr(argv[4],'.');

        // using strcmp check whether .bmp is present or not , compare str and ".bmp"
        if(strcmp(str,".bmp") == 0)
        {
            // upadte the stego file name into the structure
            encInfo->stego_image_fname = argv[4];
        }

        else
        {
            printf("ERROR : argv[4] contains wrong file name\n");
            return e_failure; 
        }
    }

    // if argv[4] is NULL then store the default name into the structure
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)

{
    //1st open the files
    if(open_files(encInfo) == e_failure)
    {
        printf("ERROR : In opening files\n");
        return e_failure;
    }

    if(check_capacity(encInfo) == e_failure)
    {
        printf("ERROR : Image capacity is Small\n");
        return e_failure;
    }

    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR : While coping Bmp header\n");
        return e_failure;
    }

    if(encode_magic_string(MAGIC_STRING,encInfo) == e_failure)
    {
        printf("EEROR : While encoding Magic String\n");
        return e_failure;
    }

    // here we are taking the secret file extension may be .txt / .c or any
    char *str = strrchr(encInfo->secret_fname, '.');
    // here by using strcpy we copying the extension to the structure
    strcpy(encInfo->extn_secret_file,str);

    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_failure)
    {
        printf("ERROR : while encoding secret file extension size\n");
        return e_failure;
    }

    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_failure)
    {
        printf("ERROR : while encoding the secret file extension\n");
        return e_failure;
    }

    if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_failure)
    {
        printf("ERROR : while encoding Secret file size\n");
        return e_failure;
    }

    if(encode_secret_file_data(encInfo) == e_failure)
    {
        printf("ERROR : while encoding secret file data\n");
        return e_failure;
    }

    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR : while encoding the remaining data to the destination image\n");
        return e_failure;
    }

    return e_success;
}



Status check_capacity(EncodeInfo *encInfo)
{
    // get the size of the source image and store it in the structure varaible image_capacity
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    // get the size of the secret file
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    if(encInfo->image_capacity > ((2 + 4 + 4 + 4 + encInfo->size_secret_file)*8))
    {
        printf("File capacity check done\n");
        return e_success;
    }

    else
    {
        printf("ERRPR : Insufficient image capacity\n");
        return e_failure;
    }
}

uint get_file_size(FILE *fptr_secret)
{
    long size;

    // by using fseek() make the pointer to point last 0th position of the secret file
    fseek(fptr_secret , 0 , SEEK_END);

    // by using ftell() get the position of the file
    size = ftell(fptr_secret);

    // make the pointer to point again to the the 1st position of the file
    rewind(fptr_secret);

    return size;
}

Status copy_bmp_header(FILE *fptr_src, FILE *fptr_dest)
{
    //create image_buffer for 54 bytes to store and write 
    char image_buffer[54];

    //rewind both the source source image file and dest image file
    rewind(fptr_src);
    rewind(fptr_dest);

    //read 54 bytes from the source image file and store it into the image buffer
    fread(image_buffer,54,1,fptr_src);

    //Write 54 bytes to the stego.bmp from the buffer
    fwrite(image_buffer,54,1,fptr_dest);

    printf("Copy bmp to header Done\n");

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char image_buffer[8];

    //run a loop according to the magic string
    for(int i = 0 ; magic_string[i] != '\0' ; i++)
    {
        //read 8 bytes from the source image and store it to the image buffer
        fread(image_buffer,8,1,encInfo->fptr_src_image);

        //encode the magic string one by one to the lsb of the image buffer that we get it from the source image
        encode_byte_to_lsb(magic_string[i],image_buffer);

        //write 8 bytes of encoded data to the stego file
        fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
    }

    printf("Magic String Encoded\n");

    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int bit ;
    //run loop for 8 time to encode 
    for(int i = 0; i < 8 ; i++)
    {
        //clear the lsb of the image_buffer[i] NOTE: here image_buffer[i] have 8 bits that last lsb we are clearing
        image_buffer[i] = image_buffer[i] & 0xFE;

        // moving the msb of the the magic_string towards to the lsb and perform & with 1 to get the lsb bit
        bit = (data >> (7 - i)) & 1;

        //After the data'msb moved to the lsb side , we add the bit to the msb of the image_buffer[i]
        image_buffer[i] = image_buffer[i] | bit;
    }

    return e_success;
}

Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char image_buffer[32];

    // store 32 bytes from source image to the image_buffer
    fread(image_buffer,32,1,encInfo->fptr_src_image);

    //encode size value into the lsb of 32 bytes of image buffer
    encode_int_to_lsb(size,image_buffer);

    //write 32 bytes of encoded data to the stego.bmp image
    fwrite(image_buffer,32,1,encInfo->fptr_stego_image);

    printf("Successfully Encoded Secret file extension size \n");

    return e_success;

}

Status encode_int_to_lsb(int size, char *image_buffer)
{
    int bit; 

    //run loop for 32 times
    for(int i = 0 ; i < 32 ; i++)
    {
        //clear lsb of the image_buffer[i]
        image_buffer[i] = image_buffer[i] & 0xFE;

        //move the msb of the size to the lsb and perform & opertaion to get the bit
        bit = (size >> 31 - i) & 1;

        //add the bit to the lsb of the image_buffer[i]
        image_buffer[i] = image_buffer[i] | bit;
    }

    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char image_buffer[8];

    //run loop according the string file_extn
    for(int i = 0 ; file_extn[i] != '\0' ; i++)
    {
        //read 8 bytes from the source image and store it in the image_buffer
        fread(image_buffer,8,1,encInfo->fptr_src_image);

        //call encode_byte_to_lsb() to encode .txt extension one by one to the lsb of the image_buffer[i]
        encode_byte_to_lsb(file_extn[i],image_buffer);

        //write the 8 bytes of encoded bytes to the stego.bmp image
        fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
    }

    printf("Successfully Encoded Secret file extension \n");
    return e_success;
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];

    //read 32 bytes from the source image and store it into the image buffer
    fread(image_buffer,32,1,encInfo->fptr_src_image);

    // call encode_int_to_lsb() encode the bits to the lsb
    encode_int_to_lsb(file_size,image_buffer);

    //write the encoded bytes from the image_buffer to the stego.bmp
    fwrite(image_buffer,32,1,encInfo->fptr_stego_image);

    printf("Successfully Encoded secret file size\n");

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char image_buffer[8];
    char ch;
    rewind(encInfo->fptr_secret);

    //run while till the EOF of secret file and one by one character store in ch
   while ((ch = fgetc(encInfo->fptr_secret)) != EOF)
    {
        //read 8 bytes from the source image and store in the image_buffer
        fread(image_buffer,8,1,encInfo->fptr_src_image);

        //encode the character to the lsb of image_buffer
        encode_byte_to_lsb(ch,image_buffer);

        //write the encoded bytes to the stego.bmp
        fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
    }

    printf("Successfully Encoded secret file data\n");

    return e_success;
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    
    // here we run loop , for copying 1 byte from the source file till the EOF
    while(fread(&ch,1,1,fptr_src))
    {
        //store the one byte of data to the stego.bmp
        fwrite(&ch,1,1,fptr_dest);
    }

    printf("Successfully copied the remaing data to the destination image\n");
    return e_success;
}