#include <stdio.h>
#include<string.h>
#include "encode.h"
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
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open source file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open secret file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open stego file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo){
    FILE *fp;
    if(strstr(argv[2],".bmp")!=NULL){
        encInfo -> src_image_fname = argv[2]; 
        fp=fopen(encInfo->src_image_fname,"rb");
        if(fp==NULL){
            printf("Error : The source image is not present in the directory\n");
            return e_failure;
        }
        fclose(fp);
    }else{
        printf("Error : The File is not .bmp format\n");
        return e_failure;
    }

    if(argv[3]==NULL){
        fp = fopen("secret.txt","w+");
        if(fp==NULL){
            printf("Error : Unable to create secret file\n");
            return e_failure;
        }
        fputs("Hello Good Morning",fp);
        fclose(fp);
        encInfo->secret_fname = "secret.txt";
        strcpy(encInfo->extn_secret_file, ".txt");
    }else if(strstr(argv[3],".txt")!=NULL){
        encInfo -> secret_fname = argv[3];
        fp= fopen(encInfo->secret_fname,"rb");
        if(fp==NULL){
            printf("Error : The input File is not present\n");
            return e_failure;
        }
        fclose(fp);
        char *dot=strrchr(encInfo->secret_fname,'.');
        if(dot!=NULL){
            strcpy(encInfo->extn_secret_file,dot);
        }
        printf("%s\n",dot);
    }else{
        printf("Error : File is not in .txt formatted\n");
        return e_failure;
    }
    if(argc>4){
        encInfo -> stego_image_fname=argv[4];
    }else{
        encInfo -> stego_image_fname="Stego.bmp";
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo){
    Status status;
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    printf("====>%u\n",encInfo->image_capacity);

    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    printf("===>File Size %ld\n",encInfo->size_secret_file);
    printf("Do Check Capacity\n");
    status = check_capacity(encInfo);
    if(status==e_success){
        printf("Successfully Completed\n");
        printf("Do Copy bmp Header\n");
        status = copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);
        if(status==e_success){
            printf("Successfully Completed\n");
            printf("Do Encode the magic string size\n");
            int magic_size=strlen(encInfo->magic);
            status=encode_magic_string_size(magic_size,encInfo);
            if(status==e_success){
                printf("Successfully Completed\n");
                printf("Do Encode the magic string\n");
                status = encode_magic_string(encInfo->magic, encInfo);
                if(status == e_success){
                    printf("Successfully completed\n");
                    printf("Do Encode Secret File Extension Size\n");
                    status=encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo);
                    if(status==e_success){
                        printf("Successfully Completed\n");
                        printf("Do Encode Secret File Extension\n");
                        status = encode_secret_file_extn(encInfo->extn_secret_file,encInfo);
                        if(status==e_success){
                            printf("Successfully Completed\n");
                            printf("Do Encode Secret File Size\n");
                            status = encode_secret_file_size(encInfo->size_secret_file,encInfo);
                            if(status == e_success){
                                printf("Successfully Completed\n");
                                printf("Do Encode Secret File Data\n");
                                status = encode_secret_file_data(encInfo);
                                if(status == e_success){
                                    printf("Successfully Completed\n");
                                    printf("Do copy Remaining Image Data\n");
                                    status=copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
                                    if(status==e_success){
                                        printf("Successfully Completed\n");
                                    }else{
                                        printf("Copy Remianing Image Data failed\n");
                                    }
                                }else{
                                    printf("Encode Secret File Data failed\n");
                                }
                            }else{
                                printf("Encode Secret File Size failed\n");
                            }
                        }else{
                            printf("Encode Secret File Extension failed\n");
                        }
                    }else{
                        printf("Encode Secret File Extension size failed\n");
                    }
                }else{
                    printf("Encode Magic string is failed\n");
                }
            }else{
                printf("Encode Magic string size failed\n");
            }

        }else{
            printf("Copy bmp header is failure\n");
        }
        return e_success;
    }else{
        printf("Check Capacity is failed\n");
        return e_failure;
    }
}
 
uint get_file_size(FILE *fptr){
    uint count=0;
    char ch;
    while((ch=fgetc(fptr))!=EOF){
        count++;
    }
    rewind(fptr);
    return count;
}

Status check_capacity(EncodeInfo *encInfo){
    uint size_sec=32+(strlen(encInfo->magic)*8)+32+(strlen(encInfo->extn_secret_file)*8)+(sizeof(long)*8)+(encInfo->size_secret_file*8);
    if((encInfo->image_capacity)>(size_sec)){
        printf("Size is Enough to encode\n");
        return e_success;
    }else{
        printf("Size is not Enough to encode\n");
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    rewind(fptr_src_image);
    char header[54];
    fread(header, 1, 54, fptr_src_image);
    fwrite(header, 1, 54, fptr_dest_image);
    return e_success;
}

Status encode_magic_string_size(int size,EncodeInfo *encInfo){
    encode_size_to_lsb((char *)&size,sizeof(size),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo){
    for(int i=0;i<strlen(magic_string);i++){
        fread(encInfo->image_data,sizeof(char),MAX_IMAGE_BUF_SIZE,encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i],encInfo->image_data);
        fwrite(encInfo->image_data,sizeof(char),MAX_IMAGE_BUF_SIZE,encInfo->fptr_stego_image);
    }
    return e_success;
}


Status encode_byte_to_lsb(char data, char *image_buffer){
    int bit;
    for(int i=7;i>=0;i--){
        bit=(data &(1<<i)) >> i;
        image_buffer[7-i]=(image_buffer[7-i] & (~1)) | bit;
    }
    return e_success;
}



Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo){
    encode_size_to_lsb((char *)&size,sizeof(size),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}


Status encode_size_to_lsb(char *data, int size,FILE *fptr_src_image,FILE *fptr_stego_image){
    char image_buffer[64];
    fread(image_buffer,sizeof(char), size*8, fptr_src_image);
    for(int i = 0; i < size*8 ; i++){
        int bit=(data[size-1-(i/8)]&(1<<(7-(i%8))))>>(7-(i%8));
        image_buffer[i] =(image_buffer[i] & (~1)) | bit;
    }
    fwrite(image_buffer, sizeof(char), size*8 , fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo){
    for(int i=0;i<strlen(file_extn);i++){
        fread(encInfo->image_data,sizeof(char),MAX_IMAGE_BUF_SIZE,encInfo->fptr_src_image);

        encode_byte_to_lsb(file_extn[i],encInfo->image_data);
        fwrite(encInfo->image_data,sizeof(char),MAX_IMAGE_BUF_SIZE,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo){
    encode_size_to_lsb((char *)&file_size,sizeof(file_size),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo){
    int ch;
    while((ch=fgetc(encInfo->fptr_secret))!=EOF){
        fread(encInfo->image_data,sizeof(char),MAX_IMAGE_BUF_SIZE,encInfo->fptr_src_image);
        encode_byte_to_lsb(ch,encInfo->image_data);
        fwrite(encInfo->image_data,sizeof(char),MAX_IMAGE_BUF_SIZE,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest){
    int ch;
    while((ch=fgetc(fptr_src))!= EOF){
        fputc(ch,fptr_dest);
    }
    return e_success;
}

