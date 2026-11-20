/*
Name : Venkata Ravindra Nandhem  
Batch Id : 26015b  
Date : 7/10/2026  
Project Title : LSB Image Steganography  

Description:  
LSB Image Steganography is a C-based application used to securely hide secret text data inside a BMP image without visibly changing the image. 
The project uses the Least Significant Bit (LSB) technique to encode and decode secret information within the image pixels. 
It allows users to encode a secret file into a BMP image using a user-defined magic string (PIN) and 
decode the hidden data only after successful PIN verification.

Main Features  
Encode Secret Data – Hide a secret file inside a BMP image using the LSB technique.  
Magic String Authentication – Use a user-defined magic string as a PIN to protect the hidden data.  
Magic String Size Encoding – Store the size of the magic string to support variable-length PINs.  
Secret File Extension Encoding – Store and recover the extension of the hidden secret file.  
Secret File Size Encoding – Store the size of the secret file for accurate decoding.  
Decode Secret Data – Extract the hidden secret data from the stego image after successful PIN verification.  
BMP Header Handling – Preserve the original BMP header while embedding secret information.  
File Handling – Use binary file operations to read, write, and process image and secret file data.
*/

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>

int main(int argc,char *argv[])
{
    
    OperationType operation;
    Status status;
    if(argc < 3){
        printf("Invalid Input\n");
        return 0;
    }
    operation = check_operation_type(argv);
    printf("Do Check operation Type\n");
    if(operation == e_encode){
        printf("Successfully Completed\n");
        printf("Encode\n");
        EncodeInfo encInfo;
        printf("Do Read and Validate \n");
        status = read_and_validate_encode_args(argc,argv,&encInfo);
        if(status==e_success){
            printf("Successfully Completed\n");
            printf("Do Open Files\n");
            status = open_files(&encInfo);
            if(status==e_success){
                printf("Files opened successfully Completed\n");
                printf("Enter the magic string: ");
                scanf("%49s", encInfo.magic);
                status=do_encoding(&encInfo);
                printf("Do Encoding\n");
                if(status==e_success){
                    printf("Encoding completed successfully\n");
                }else{
                    printf("Encoding Failed\n");
                }
            }else{
                printf("Failed to Open files\n");
            }
            //printf("Validation is Completed Successfully\n");
        }else{
            printf("Validation is not Completed\n");
        }
    }else if(operation == e_decode){
        DecodeInfo decInfo;
        printf("Decode\n");
        printf("Do Read and Validate\n");
        status = read_and_validate_decode_args(argv,&decInfo);
        if(status == e_success){
            printf("Successfully Completed\n");
            printf("Do Open Files for Decode\n");
            status = open_decode_files(&decInfo);
            if(status == e_success){
                printf("Successfully Completed\n");
                printf("Do Decoding\n");
                status = do_decoding(&decInfo);
                if(status==e_success){
                    printf("Successfully completed\n");
                }else{
                    printf("Decoding failed\n");
                }
            }else{
                printf("Open Files for Decode is failed\n");
            }
        }else{
            printf("Read and Validate is failed\n");
        }
    }else if(operation == e_unsupported){
        printf("The input Operation is invalid\n");
    }
    return 0;
}

OperationType check_operation_type(char *argv[]){
    if(strcmp(argv[1],"-e")==0){
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0 && (strstr(argv[2],".bmp")!=NULL)){
        return e_decode;
    }else{
        return e_unsupported;
    }
}


