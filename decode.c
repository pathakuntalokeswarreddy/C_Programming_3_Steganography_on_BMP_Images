#include <stdio.h>
#include <string.h>
// #include <limits.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo)
{
    int flag=0;
    printf("\nPerforming read_and_validate_decode_args..\n");
    
    if(argv[2] != NULL)
    {
        if ( strstr(argv[2],".bmp") != NULL )
        {
            if( ( *( (char *)(strstr(argv[2],".bmp")) +4 )) == '\0' )
            {
                encInfo -> src_image_fname = argv[2];
                printf("Entered stego src is %s\n",encInfo -> src_image_fname);
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
        printf("Stego.bmp file not provided..\n");
        return e_failure;
    }

    if( argv[3] != NULL )
    {
        if ( strstr(argv[3],".txt") != NULL )
        {
            if( ( *( (char *)(strstr(argv[3],".txt")) +4 )) == '\0' )
            {
                encInfo -> secret_fname = argv[3];
                printf("Entered .txt is %s\n",encInfo -> secret_fname);
            }
            else
                flag=1;
        }
        else
            flag=1;
    }
    else if(argv[3] == NULL)
    {
        encInfo -> secret_fname = "decoded_data.txt";
        printf("written .txt is %s\n",encInfo -> secret_fname);
    }
    else
        flag=1;
    if(flag)
    {
        printf("Failed at Reading decoded_data.txt..\n");
        return e_failure;
    }
    return e_success;
}

Status open_files_for_decode(EncodeInfo *encInfo)
{
    short int BMP;
    // Src Image file for stego,bmp
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    fread(&BMP, sizeof(BMP),1,encInfo->fptr_src_image);
    if ( BMP != 0x4d42 )
    {
        printf("Stego.bmp is not an .bmp file..\n");
        return e_failure;
    }


    // Secret Data file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;

}


Status decode_data_from_image(char *data, int size, EncodeInfo *encInfo)
{
    int i=0,j,k;
    printf("\nDecoded :");
    while(i<size)
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
        printf("%c",encInfo -> secret_data[0]);
        if ( encInfo -> secret_data[0] != data[i] )
        {
            break;
        }
        i++;
    }
    return e_success;
}