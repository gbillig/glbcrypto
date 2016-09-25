# glbcrypto

glbcrypto is an open source cryptographic library written in C.

This library can be used in any project that requires implementations of various cryptographic functions.
The following functions are included in this library:

* aes_128_ecb : The Advanced Encryption Standard (AES) block cipher with 128-bit key using the Electronic codebook (ECB) mode of operation.
* aes_192_ecb : The Advanced Encryption Standard (AES) block cipher with 192-bit key using the Electronic codebook (ECB) mode of operation.
* aes_256_ecb : The Advanced Encryption Standard (AES) block cipher with 256-bit key using the Electronic codebook (ECB) mode of operation.
* aes_128_cbc : The Advanced Encryption Standard (AES) block cipher with 128-bit key using the Cipher Block Chaining (CBC) mode of operation.
* aes_192_cbc : The Advanced Encryption Standard (AES) block cipher with 192-bit key using the Cipher Block Chaining (CBC) mode of operation.
* aes_256_cbc : The Advanced Encryption Standard (AES) block cipher with 256-bit key using the Cipher Block Chaining (CBC) mode of operation.
* sha_256 : The Secure Hashing Algorithm 2 (SHA-2) with 256-bit digest (hash value). 
* sha_384 : The Secure Hashing Algorithm 2 (SHA-2) with 384-bit digest (hash value). 
* sha_512 : The Secure Hashing Algorithm 2 (SHA-2) with 512-bit digest (hash value). 
* get_random : A function acts as a cryptographically secure pseudo-random number generator (CSRNG).

## Installing

To build this project, run the following command:
```
make all
```

## Running the tests

A binary named glbcrypto_tests will be generated in the bin folder. You can execute it to runs the tests.

## Adding the library to another project

Copy the lib/libglbcrypto.a file as well as the necessary header files located in the inc/ directory to the other project. 
When building the project, remember to link the provided headers with your source files.

## Authors

* **Gleb Billig** - *Initial work* - [gbillig](https://github.com/gbillig)

## License

This project is licensed under the GPLv3 License - see the [LICENSE.md](LICENSE.md) file for details