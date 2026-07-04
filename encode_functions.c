#include <stdio.h>
#include <string.h>

#include "encode.h" 
#include "decode.h"
#include "types.h" 

Status do_encoding(EncodeInfo *encInfo)
{
    printf("\nEntered to Encoding Process,,\n");
    if( copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("copy_bmp_header completed..\n");
    }
    else 
    {
        printf("\ncopy_bmp_header has failed..\n");
        return e_failure;
    }
    
    char *magic_string = MAGIC_STRING;
    if( encode_magic_string(magic_string, encInfo) == e_success)
    {
        printf("encode_magic_string completed..\n");
    }
    else
    {
        printf("\nencode_magic_string has failed..\n");
        return e_failure;
    }

    strcpy(encInfo -> extn_secret_file, ".txt");
    if( encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
    {
        printf("encode_secret_file_extn completed..\n");
    }
    else
    {
        printf("\nencode_secret_file_extn has failed..\n");
        return e_failure;
    }
    
    if( encode_secret_file_size( encInfo -> size_secret_file, encInfo) == e_success)
    {
        printf("encode_secret_file_size completed..\n");
    }
    else
    {
        printf("\nencode_secret_file_size has failed..\n");
        return e_failure;
    }

    if( encode_secret_file_data(encInfo) == e_success)
    {
        printf("encode_secret_file_data completed..\n");
    }
    else
    {
        printf("\nencode_secret_file_data has failed..\n");
        return e_failure;
    }

    if( copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("copy_remaining_img_data completed..\n");
    }
    else
    {
        printf("\ncopy_remaining_img_data has failed..\n");
        return e_failure;
    }
    
    
    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    unsigned char BMP_Header[54];
        
    if ( fread(BMP_Header,sizeof(BMP_Header),1,fptr_src_image) == 0 )
    {        
        return e_failure;
    }
    
    if ( fwrite(BMP_Header,sizeof(BMP_Header),1,fptr_dest_image) == 0 )
    {
        return e_failure;
    }
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    if ( encode_data_to_image(magic_string, strlen(magic_string), encInfo -> fptr_src_image, encInfo ->fptr_stego_image) == e_success )
        return e_success;
    else
        return e_failure;
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    uint img_size = strlen(file_extn);   
    if(img_size > MAX_FILE_SUFFIX)
    {
        printf("MAX_FILE_SUFFIX for secret file is 4 only..\n");
        return e_failure;
    }
    
    printf("Sec file Extension has %d letters\n",img_size);
    
    int ch,i,j;
    for (i=31 ; i>=0 ; i--)
    {
        fread(&ch, 1, 1, encInfo -> fptr_src_image);
        if( img_size & (1<<i) )
            ch = ch | 1;
        else
            ch = ch & (~1);
        fwrite(&ch,1,1,encInfo -> fptr_stego_image);
    }
    if (i>=0)
    {
        printf("secret file ext size writing is failed..\n");
        return e_failure;
    }
    else
    {
        printf("secret file ext size writing is completed..\n");
    }
    
    if ( encode_data_to_image(file_extn, strlen(file_extn), encInfo -> fptr_src_image, encInfo ->fptr_stego_image) == e_success )
        return e_success;
    else
        return e_failure;

}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    int i,ch;
    printf("file_size : %ld\n",file_size);

    for (i=63 ; i>=0 ; i--)
    {
        fread(&ch,1,1,encInfo -> fptr_src_image);
        if( file_size & ((long)1<<i) )
            ch = ch | 1;
        else
            ch = ch & (~1);
        fwrite(&ch,1,1,encInfo -> fptr_stego_image);
    }
    if (i>=0)
    {
        return e_failure;
    }
    
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int i=0,j;
    char secret_data[encInfo -> size_secret_file +1],ch;
    fread(secret_data,sizeof(secret_data),1,encInfo -> fptr_secret);
    secret_data[encInfo -> size_secret_file]='\0';
    printf("Secret Data is : %s\n",secret_data);

    if ( encode_data_to_image(secret_data, strlen(secret_data), encInfo -> fptr_src_image, encInfo ->fptr_stego_image) == e_success )
        return e_success;
    else
        return e_failure;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    printf("In copy_remaining_img_data\n");
    int ch;
    
    while( (fread(&ch,1,1,fptr_src)) >0 )
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    int ch,i=0,j;
    while(data[i])
    {        
        for(j=7 ; j>=0 ; j--)
        {
            
            fread( &ch, 1, 1, fptr_src_image);
            if ( data[i] & (1<<j) )
                ch = ch | 1 ;
            else
                ch = ch & (~1) ;
            fwrite(&ch,1,1,fptr_stego_image);
            if(j==0)
            {
                i++;
                break;
            }
        }
    }
    if (i != size)
    {
        return e_failure;
    }

    return e_success;

}
