LSB-Steganography (BMP – C++)

This project is a C++ implementation of Least Significant Bit (LSB) steganography that allows hiding and extracting secret text messages inside 24-bit BMP image files.
Unlike basic LSB examples, this implementation supports variable bit-depth embedding (1–8 bits per color channel), allowing a trade-off between capacity and visual distortion.

Features

Embed secret messages into BMP images
Extract hidden messages from stego-images
Supports 1 to 8 LSBs per color channel
Uses RGB channels independently
Null-terminated message handling ('\0')
No external libraries
Fully manual bitwise manipulation
Educational and security-focused design

Steganography Method

Each pixel consists of 3 color channels (B, G, R).
The program replaces the last N bits of each channel with bits from the secret message.

Example (1-bit mode):

Original byte : 10101100
Message bit   :        1
Result byte   : 10101101

Higher Bit Modes:

1 bit → minimal distortion, low capacity
4 bits → balanced
8 bits → maximum capacity, full color overwrite (visible distortion)

Capacity Calculation

Maximum character capacity is calculated as:
(width × height × 3 channels) / 8

Capacity dynamically scales based on selected bit depth (1–8).

Technologies Used

Language: C++
Image Format: BMP (24-bit, uncompressed)
Core Concepts:
Bitwise operations
Binary file I/O
Memory management
Steganography fundamentals

Compilation

Compile using g++:
g++ main.cpp -o lsb_steganography

Usage

Step 1: Run the program
./lsb_steganography

Step 2: Enter image dimensions
Enter width and height (e.g., 400 300)

Step 3: Choose operation
1 - Embed secret message
2 - Extract hidden message

Embedding a Message

Select bit depth (1–8)
Enter secret message
Output BMP is generated with embedded data

Extracting a Message

Provide the correct bit depth
Message is reconstructed bit-by-bit
Stops automatically at '\0'

Limitations

Works only with uncompressed BMP files
Image width must be a multiple of 4 (padding ignored)
No encryption (pure steganography)
Requires correct bit depth during extraction

Possible Improvements

Add AES/XOR encryption before embedding
Automatic BMP header parsing (remove manual width/height input)
PNG support
Password-based extraction
GUI version
Error detection (CRC / checksum)

Educational Purpose

This project is suitable for:
Computer Engineering students
Information security courses
Bit-level data manipulation practice
Steganography demonstrations
C++ low-level programming portfolios

License
MIT License — free to use, modify, and distribute.




