# Base64 Encoder

## Overview
This C++ application is designed to encode files into Base64 format. It reads the input file in binary format, encodes it to Base64, and saves the output to a specified file. The application utilizes a custom encoding table and handles different file sizes by padding the output correctly according to the Base64 standard.

## Features
- **File Encoding:** Converts binary files into Base64 encoded text.
- **Custom Encoding Table:** Utilizes a user-provided encoding table from a text file for encoding.
- **Input/Output Control:** Allows users to specify input and output files through the command line.
- **Error Handling:** Provides feedback on file access errors and operational issues.

## Getting Started

### Prerequisites
- C++ compiler (GCC or Clang recommended)
- Standard C++ libraries
- Development environment capable of compiling C++ code (e.g., Visual Studio, Code::Blocks)

## Usage

Prepare the Encoding Table: Ensure tablica_kodowania.txt is in the same directory as the executable or provide the path when prompted.
Run the Program: Launch the compiled executable.
Follow On-Screen Instructions: The program will ask for the names of the input file to encode and the output file to save the encoded data.
Input and Output Files: Provide the file names when prompted. The program handles the rest, encoding the content and saving it to the specified output file.

## How it Works
The program reads up to three bytes from the input file, encodes these bytes into four characters using the Base64 encoding scheme, and writes the encoded characters to the output file. If the bytes read are less than three, it pads the result accordingly to ensure valid Base64 output.

## Author

Katarzyna Szczerba
