# Virus Scanner

A C++ static malware scanning project that recursively analyzes files and filters potential targets based on file permissions and MIME types.

## Requirements
### Build Tools
Cmake 3.20+

C++20 compatible compiler

### Dependencies
OpenSSL 3.x - For cryptographic functions

libmagic    - For filetype identification

YARA        - For rule based detection / pattern matching



## Current Features
Recursive filesystem parsing

File permission filtering

MIME-type identification using libmagic

SHA-256 file signature generation

Python prototype Bloom filter using Kirsch–Mitzenmacher double hashing for efficient signature membership queries

## Technologies
C++

Python

OpenCV

libmagic

YARA

## Status
Currently in development. The parser and hashing stages are implemented, with additional scanning analysis features, and file quarantining systems planned.
