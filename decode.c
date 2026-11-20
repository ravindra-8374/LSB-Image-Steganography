#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo){
    if(argv[2]==NULL){
        printf("Stego image is not provided\n");
        return e_failure;
    }else if(strstr(argv[2],".bmp")!=NULL){
        decInfo->stego_image_fname=argv[2];
        printf("Stego image: %s\n",decInfo->stego_image_fname);
        return e_success;
    }else{
        printf("Error : File is not a BMP file\n");
        return e_failure;
    }
}


Status open_decode_files(DecodeInfo *decInfo){
    decInfo->fptr_stego_image =fopen(decInfo->stego_image_fname, "rb");
    if(decInfo->fptr_stego_image == NULL){
        printf("Error: Unable to open Stego image\n");
        return e_failure;
    }
    printf("Stego image opened successfully\n");
    return e_success;
}


Status do_decoding(DecodeInfo *decInfo){
    Status status;
    char user_magic[50];
    printf("Do Decoding\n");
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    printf("Do Decode Magic String Size\n");
    status=decode_magic_string_size(decInfo);
    if(status==e_success){
        printf("Successfully Completed\n");
        printf("Magic String Size : %d\n",decInfo->magic_size);
        printf("Do Decode Magic String\n");
        status=decode_magic_string(decInfo);
        if(status==e_success){
            printf("Successfully Completed\n");
            printf("Enter the Magic String : ");
            scanf("%49s",user_magic);
            if(strcmp(user_magic,decInfo->magic)==0){
                printf("Magic String Matched\n");
                printf("Do Decode Secret File Extension Size\n");
                status = decode_secret_file_extn_size(decInfo);
                if(status == e_success){
                    printf("Successfully Completed\n");
                    printf("Extension File Size : %d\n",decInfo->extn_size);
                    printf("Do Decode Secret File Extension\n");
                    status = decode_secret_file_extn(decInfo);
                    if(status==e_success){
                        printf("Successfully Completed\n");
                        printf("Decoded Extension : %s\n",decInfo->extn_secret_file);
                        printf("Do Decode Secret file size\n");
                        status = decode_secret_file_size(decInfo);
                        if(status == e_success){
                            printf("Successfully Completed\n");
                            printf("Secret File size : %ld\n",decInfo->size_secret_file);
                            printf("Do Open Secret File\n");
                            status=open_secret_file(decInfo);
                            if(status==e_success){
                                printf("Secret file created successfully\n");
                                printf("Do Decode Secret File Data\n");
                                status=decode_secret_file_data(decInfo);
                                if(status==e_success){
                                    printf("Secret File Data decoded successfully\n");
                                    fclose(decInfo->fptr_stego_image);
                                    fclose(decInfo->fptr_secret);
                                    printf("Files Closed Successfully\n");
                                }else{
                                    printf("Decode Secret File Data failed\n");
                                    return e_failure;
                                }
                            }else{
                                printf("Open Secret File failed\n");
                                return e_failure;
                            }
                        }else{
                            printf("Decode Secret file size failed\n");
                            return e_failure;
                        }
                    }else{
                        printf("Decode Secret File Extension failed\n");
                        return e_failure;
                    }
                }else{
                    printf("Decode Secret File Extension size failed\n");
                    return e_failure;
                }
            }else{
                printf("Invalid Magic String\n");
                return e_failure;
            }
        }else{
            printf("Decode Magic String failed");
            return e_failure;
        }
    }else{
        printf("Decode magic string size failed\n");
        return e_failure;
    }

    return e_success;
}

Status decode_magic_string_size(DecodeInfo *decInfo){
    char data[32];
    fread(data, sizeof(char), 32, decInfo->fptr_stego_image);
    decInfo->magic_size = 0;

    printf("LSB bits: ");
    for(int i=0;i<32;i++){
        decInfo->magic_size=(decInfo->magic_size << 1) | (data[i] & 1);
    }
    printf("\n");
    printf("Decoded magic size = %d\n", decInfo->magic_size);
    if(decInfo->magic_size<=0 || decInfo->magic_size>=50){
        printf("Invalid Magic String Size\n");
        return e_failure;
    }
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo){
    char image_buffer[8];
    for(int i=0;i<decInfo->magic_size;i++){
        fread(image_buffer, sizeof(char), 8, decInfo->fptr_stego_image);
        decInfo->magic[i] = decode_byte_from_lsb(image_buffer);
    }
    decInfo->magic[decInfo->magic_size] = '\0';
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo){
    char image_buffer[32];
    fread(image_buffer,sizeof(char),32,decInfo->fptr_stego_image);
    decInfo->extn_size = 0;
    for(int i=0;i<32;i++){
        decInfo->extn_size=(decInfo->extn_size<<1)|(image_buffer[i]&1);
    }
    printf("Decoded extension size = %d\n",decInfo->extn_size);
    if(decInfo->extn_size <=0 ||decInfo->extn_size >=50){
        printf("Invalid extension size\n");
        return e_failure;
    }
    return e_success;
}


Status decode_secret_file_extn(DecodeInfo *decInfo){
    char image_buffer[8];
    for(int i=0;i<decInfo->extn_size;i++){
        fread(image_buffer,sizeof(char),8,decInfo->fptr_stego_image);
        decInfo->extn_secret_file[i]=decode_byte_from_lsb(image_buffer);
    }
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo){
    char image_buffer[64];
    long size = 0;
    fread(image_buffer,sizeof(char),sizeof(long)*8,decInfo->fptr_stego_image);
    for(int i=0;i<sizeof(long)*8;i++){
        size=(size<<1) | (image_buffer[i]&1);
    }
    decInfo->size_secret_file = size;
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo){
    char image_buffer[8];
    char data;
    for(long i=0;i<decInfo->size_secret_file;i++){
        fread(image_buffer,sizeof(char),8,decInfo->fptr_stego_image);
        data=decode_byte_from_lsb(image_buffer);
        fwrite(&data,sizeof(char),1,decInfo->fptr_secret);
    }
    return e_success;
}

Status open_secret_file(DecodeInfo *decInfo){
    decInfo->secret_fname ="decoded.txt";
    decInfo->fptr_secret=fopen(decInfo->secret_fname,"w");
    if(decInfo->fptr_secret==NULL){
        printf("Error: Unable to create output secret file\n");
        return e_failure;
    }
    return e_success;
}

char decode_byte_from_lsb(char *image_buffer){
    char data=0;
    for(int i=0;i<8;i++){
        data=(data<<1)|(image_buffer[i]&1);
    }
    return data;
}