#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    // Check minimum arguments
    if(argc < 3)
    {
        printf("Usage:\n");
        printf("./a.out -e <source.bmp> <secret.txt> <stego.bmp>\n");
        printf("./a.out -d <stego.bmp> <output.txt>\n");
        return 1;
    }

    // Check encode option
    if(strcmp(argv[1], "-e") == 0)
    {
        printf("INFO : Encoding Selected\n");

        // Validate encode arguments
        if(read_and_validate_encode_args(argc, argv, &encInfo) == e_failure)
        {
            printf("ERROR : Invalid Encode Arguments\n");
            return 1;
        }

        // Start encoding
        if(do_encoding(&encInfo) == e_success)
        {
            printf("Encoding Completed Successfully\n");
        }
        else
        {
            printf("Encoding Failed\n");
        }
    }

    // Check decode option
    else if(strcmp(argv[1], "-d") == 0)
    {
        printf("INFO : Decoding Selected\n");

        // Validate decode arguments
        if(read_and_validate_decode_args(argv, &decInfo) == e_failure)
        {
            printf("ERROR : Invalid Decode Arguments\n");
            return 1;
        }

        // Start decoding
        if(do_decoding(&decInfo) == e_success)
        {
            printf("Decoding Completed Successfully\n");
        }
        else
        {
            printf("Decoding Failed\n");
        }
    }

    // Invalid option
    else
    {
        printf("Invalid Option\n");
        printf("Use -e for Encode\n");
        printf("Use -d for Decode\n");
    }

    return 0;
}