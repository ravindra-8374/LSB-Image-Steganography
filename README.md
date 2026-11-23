# LSB Image Steganography

## Project Description
LSB Image Steganography is a C-based application used to securely hide secret text messages inside BMP images without visibly changing the image.

The project uses the Least Significant Bit (LSB) technique to encode and decode secret information within image pixels.

## Features
- Encode secret text into a BMP image.
- Decode hidden messages from a stego image.
- Preserve the visual appearance of the original image.
- Validate input files and encoding requirements.

## Technologies Used
- C Programming
- File Handling
- Structures and Pointers
- Bitwise Operations
- Least Significant Bit (LSB) Technique

## Project Structure
- `main.c` – Main program execution.
- `encode.c` – Encoding implementation.
- `decode.c` – Decoding implementation.
- `encode.h` – Encoding function declarations.
- `decode.h` – Decoding function declarations.
- `common.h` – Common definitions.
- `types.h` – Data types and status definitions.

## Compilation
Compile the source files using GCC:

```bash
gcc main.c encode.c decode.c -o stego
```

## Execution
Run the generated executable:

```bash
./stego
```

Refer to the command-line arguments supported by the application for encoding and decoding operations.

## Author
Venkata Ravindra Nandhem

## Project
LSB Image Steganography using C