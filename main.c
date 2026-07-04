#include <stdio.h>
#include <string.h>
// #include <limits.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc , char* argv[])
{
    EncodeInfo encInfo;
    
    int operation = check_operation_type(argv);
    if( operation == e_encode )
    {
        printf("\nEntered to Encoding Process..\n");
        
        if( read_and_validate_encode_args(argv,&encInfo) != e_failure) 
        {
            printf("read_and_validate_encode_args completed..\n");
            if ( open_files(&encInfo) == e_success)
            {
                printf("\nopen_files completed..\n");
            }
            else
            {
                printf("\nopen_files failed..\n");
                return -1;
            }

            if ( check_capacity(&encInfo) == e_success)
            {
                printf("\ncheck_capacity is completed..\n");
            }
            else
            {
                printf("\nImage size is not sufficient for the given Secret Image..\n");
                return -1;
            }

            if ( do_encoding(&encInfo) == e_success)
            {
                printf("do_encoding is Completed..\n");
            }
            else
            {
                printf("\ndo_encoding is Failed..\n");
            }

        }
        else
        {
            printf("\nread_and_validate_encode_args failed..\n");
            return -1;
        }
    }
    else if( operation == e_decode )
    {
        printf("\nEntered to Decoding Process..\n");
        if( read_and_validate_decode_args(argv,&encInfo) != e_failure) 
        {
            printf("\nread_and_validate_decode_args completed..\n");
            if ( open_files_for_decode(&encInfo) == e_success)
            {
                printf("\nopen_files completed..\n");
            }
            else
            {
                printf("\nopen_files failed..\n");
                return -1;
            }

            if ( do_decoding(&encInfo) == e_success)
            {
                printf("\ndo_decoding completed..\n");
            }
            else
            {
                printf("\ndo_decoding failed..\n");
                return -1;
            }
        }
        else
        {
            printf("\nread_and_validate_decode_args failed..\n");
            return -1;
        }

    }
    else
    {
        printf("Enter -e for encoding\n      -d for decoding\n");
        return -1;
    }


    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(argv[1][0] == '-')
    {
        if(argv[1][1] == 'e' && argv[1][2] == '\0')
        {
            printf("selected Encoding!!..\n");
            return e_encode;
        }
        else if(argv[1][1] == 'd' && argv[1][2] == '\0')
        {
            printf("selected Decoding!!..\n");
            return e_decode;
        }
        else
        {   printf("Unsupported Operation Requested..\n");
            return e_unsupported;
        }
    }
    else
    {
        printf("Unsupported Operation Requested..\n");
        return e_unsupported;
    }
}