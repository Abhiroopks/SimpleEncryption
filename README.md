# SimpleEncryption
Tool to encrypt /decrypt files using very simple algorithm

Parallelizes portion of code to speedup performance, using OpenMP

- To compile: `make proj`
- Create key of X byte length: `./test/genBytes [X] [OUTPUT FILE]`
- To encrypt file: `./bin/Encode.o [INPUT FILE] [OUTPUT FILE] [KEY]`
- Decrypt file: `./bin/Decode.o [INPUT FILE] [OUTPUT FILE] [KEY]`
