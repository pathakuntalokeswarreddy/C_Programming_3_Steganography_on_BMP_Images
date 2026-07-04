#include <stdio.h>
#include <string.h>

#include "encode.h"  
#include "decode.h"
#include "types.h"

/* Function Definitions */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    int flag=0;
    printf("\nPerforming read_and_validate_encode_args..\n");
    
    if(argv[2] != NULL)
    {
        if ( strstr(argv[2],".bmp") != NULL )
        {
            if( ( *( (char *)(strstr(argv[2],".bmp")) +4 )) == '\0' )
            {
                encInfo -> src_image_fname = argv[2];
                printf("Entered src is %s\n",encInfo -> src_image_fname);
            }
            else
            {
                flag=1;
            }
        }
        else
            flag=1;
        if(flag)
        {
            printf("Failed at Reading src.bmp..\n");
            return e_failure;
        }
    }
    else
    {
        printf("src.bmp not provided..\n");
        return e_failure;

    }

    if(argv[3] != NULL)
    {
        
        if ( strstr(argv[3],".txt") != NULL )
        {
            if( ( *( (char *)(strstr(argv[3],".txt")) +4 )) == '\0' )
            {
                encInfo -> secret_fname = (char *)argv[3];
                printf("Entered txt is %s\n",((char *)encInfo -> secret_fname));
            }
            else
                flag=1;
        }
        else
            flag=1;
        if(flag)
        {
            printf("Failed at Reading file.txt..\n");
            return e_failure;
        }
    }
    else
    {
        printf("sec.txt not provided..\n");
        return e_failure;
    }

    if( argv[4] != NULL )
    {
        if ( strstr(argv[4],".bmp") != NULL )
        {
            if( ( *( (char *)(strstr(argv[4],".bmp")) +4 )) == '\0' )
            {
                encInfo -> stego_image_fname = argv[4];
                printf("Entered dst is %s\n",encInfo -> stego_image_fname);
            }
            else
                flag=1;
        }
        else
            flag=1;
    }
    else if(argv[4] == NULL)
    {
        encInfo -> stego_image_fname = "stego.bmp";
        printf("written dst is %s\n",encInfo -> stego_image_fname);
    }
    else
        flag=1;
    if(flag)
    {
        printf("Failed at Reading dest.bmp..\n");
        return e_failure;
    }
    return e_success;
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


Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    printf(" Source File size is : %u\n",encInfo -> image_capacity);
    rewind(encInfo -> fptr_src_image);

    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    printf(" Secret File size is : %lu\n",encInfo -> size_secret_file);
    rewind(encInfo -> fptr_secret);
 
    /*
    Header : 54
    magic string : 16
    ext file fmt : 32
    ext file .txt: 32
    size of data : 64
    sec data     : (user choise)
    */
    
    if( (encInfo -> image_capacity) > (54+16+32+32+64+(encInfo -> size_secret_file * 8)) )
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return (ftell(fptr));

}

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
    printf("\nwidth = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

