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

Compile the project using GCC:

```bash
gcc main.c encode.c decode.c -o stego
```

## Execution

### 1. Encoding

To hide a secret text file inside a BMP image:

```bash
./stego -e beautiful.bmp secret.txt
```

Enter the magic string (PIN) when prompted.

The encoded image is generated as `Stego.bmp`.

### 2. Decoding

To extract the hidden secret file:

```bash
./stego -d Stego.bmp
```

Enter the same magic string used during encoding.

The decoded file is generated as `decoded.txt`.

## Verification

Compare the original and decoded files:

```bash
cmp secret.txt decoded.txt
```

If there is no output, both files are identical.

## Author

Venkata Ravindra Nandhem

## Project

LSB Image Steganography using C