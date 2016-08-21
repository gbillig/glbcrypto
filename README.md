# glbcrypto
glbcrypto is a cryptographic library written in C.  
This library currently provides implementations of the the AES (Advanced Encryption Standard) family of block ciphers, as well as the SHA-2 (Secure Hash Algorithm 2) family of cryptographic hash functions.

## How to build
make all

## How to run the test cases
The binary will be built in the bin folder: bin/glbcrypto_tests

## How to add the library to your project
Copy the lib/libglbcrypto.a file as well as the necessary header files located in the inc/ directory to your project.  
When building your project, don't forget to link these files with your other source files.
