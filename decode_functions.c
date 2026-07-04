#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

Status do_decoding(EncodeInfo *encInfo)
{
    printf("\nEntered to Decoding Process,,\n");
    
    char *magic_string = MAGIC_STRING;
    if( decode_magic_string(magic_string, encInfo) == e_success)
    {
        printf("\ndecode_magic_string completed..\n");
    }
    else
    {
        printf("\ndecode_magic_string has failed..\n");
        return e_failure;
    }

    char *ext = ".txt" ;
    if( decode_secret_file_extn(ext, encInfo) == e_success)
    {
        printf("\ndecode_secret_file_extn completed..\n");
    }
    else
    {
        printf("\ndecode_secret_file_extn has failed..\n");
        return e_failure;
    }
    
    if( decode_secret_file_size(encInfo) == e_success)
    {
        printf("\ndecode_secret_file_size completed..\n");
    }
    else
    {
        printf("\ndecode_secret_file_size has failed..\n");
        return e_failure;
    }

    if( decode_secret_file_data(encInfo) == e_success)
    {
        printf("\ndecode_secret_file_data completed..\n");
    }
    else
    {
        printf("\ndecode_secret_file_data has failed..\n");
        return e_failure;
    }
    
    return e_success;
}

Status decode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    
    fseek(encInfo -> fptr_src_image,54,SEEK_SET);
    
    if ( decode_data_from_image(magic_string, strlen(magic_string), encInfo) == e_success)
    {
        return e_success;
    }
    else
    {
        printf("Given file is not Encoded perfectly (magic string)..\n");
        return e_failure;
    }
        
}

Status decode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    int i=0,j,k,size_secret_file = 0;
    
    //size of secret file extension
    while(i<4)
    {
        fread(encInfo -> image_data, sizeof(encInfo -> image_data), 1, encInfo -> fptr_src_image);
        
        encInfo -> secret_data[0] = 0;

        for(j=7,k=0 ; j>=0 ;j--,k++)
        {
            if (encInfo -> image_data[k] & 1)
            {
                encInfo -> secret_data[0] |= (1<<j);
            }
        }
        size_secret_file += encInfo -> secret_data[0];
        i++;
    }
    printf("size of secret file extension : %d\n",  size_secret_file);
    if ( size_secret_file != 4 )
    {
        printf("Size of extension not matched..\n");
        return e_failure;
    }

    //secret file extension
    if ( decode_data_from_image(file_extn, strlen(file_extn), encInfo) != e_success )
    {
        printf("Given file is not Encoded correctly(.txt)..\n");
        return e_failure;
    }
    return e_success;
}

Status decode_secret_file_size(EncodeInfo *encInfo)
{
    printf("\n");
    int i=0,j,k;
    encInfo -> size_secret_file = 0;
    //size of secret file extension
    while(i<8)
    {
        fread(encInfo -> image_data, sizeof(encInfo -> image_data), 1, encInfo -> fptr_src_image);
        
        encInfo -> secret_data[0] = 0;

        for(j=7,k=0 ; j>=0 ;j--,k++)
        {
            if (encInfo -> image_data[k] & 1)
            {
                encInfo -> secret_data[0] |= (1<<j);
            }
        }
        (encInfo -> size_secret_file) += (encInfo -> secret_data[0]);
        i++;
    }
    printf("size of secret file data : %ld\n", encInfo -> size_secret_file);
    return e_success;   
}

Status decode_secret_file_data(EncodeInfo *encInfo)
{
    int i=0,j,k;   
    while( i < (encInfo ->size_secret_file))
    {
        fread(encInfo -> image_data, sizeof(encInfo -> image_data), 1, encInfo -> fptr_src_image);
    
        encInfo -> secret_data[0] = 0;

        for(j=7,k=0 ; j>=0 ;j--,k++)
        {
            if (encInfo -> image_data[k] & 1)
            {
                encInfo -> secret_data[0] |= (1<<j);
            }
        }
        
        fwrite(&(encInfo -> secret_data[0]), sizeof(encInfo -> secret_data[0]), 1, encInfo ->fptr_secret);
        i++;
    }
    return e_success;    
}