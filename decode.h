#ifndef DECODE_H
#define DECODE_H

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_for_decode(EncodeInfo *encInfo);

/* Perform the decoding */
Status do_decoding(EncodeInfo *encInfo);

/* Decodes Magic String */
Status decode_magic_string(char *magic_string, EncodeInfo *encInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);

/* Decode secret file size */
Status decode_secret_file_size(EncodeInfo *encInfo);

/* Decode secret file data*/
Status decode_secret_file_data(EncodeInfo *encInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size, EncodeInfo *encInfo);

#endif