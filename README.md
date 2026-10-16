# Hex Inspector
#### **Description:** A fast, lightweight C command-line binary file inspector that generates a hex dump and ASCII representation for arbitrary files.
#### **Date:** 03 October 2026

## Features
- **Binary Stream Processing:** Uses `fread` to stream 16-byte chunks from disk without reading entire files into memory.
- **64-bit Offset Tracking:** Displays absolute file offsets formatted in 16-digit uppercase hexadecimal (`%016llX`).
- **Sign Extension Prevention:** Utilizes `unsigned char` buffers to prevent negative byte values from corrupting hex output formatting.
- **ASCII Filtering:** Filters control characters outside the printable ASCII range (`0x20` to `0x7E`), mapping non-printable bytes to `.`.
- **Dynamic Padding:** Automatically calculates and injects alignment padding for tail chunks under 16 bytes.

## Sample Output:
```
0000000000000000  |  4D 5A 90 00 03 00 00 00 04 00 00 00 FF FF 00 00  |  MZ..............
0000000000000010  |  B8 00 00 00 00 00 00 00 40 00 00 00 00 00 00 00  |  ........@.......
0000000000000020  |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |  ................
0000000000000030  |  00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 00  |  ................
0000000000000040  |  0E 1F BA 0E 00 B4 09 CD 21 B8 01 4C CD 21 54 68  |  ........!..L.!Th
0000000000000050  |  69 73 20 70 72 6F 67 72 61 6D 20 63 61 6E 6E 6F  |  is program canno
0000000000000060  |  74 20 62 65 20 72 75 6E 20 69 6E 20 44 4F 53 20  |  t be run in DOS 
0000000000000070  |  6D 6F 64 65 2E 0D 0D 0A 24 00 00 00 00 00 00 00  |  mode....$.......
0000000000000080  |  50 45 00 00 64 86 12 00 53 CF C0 6A 00 D8 00 00  |  PE..d...S..j....
0000000000000090  |  9B 06 00 00 F0 00 26 00 0B 02 02 2E 00 8A 00 00  |  ......&.........
00000000000000A0  |  00 28 00 00 00 0C 00 00 5F 10 00 00 00 10 00 00  |  .(......_.......
00000000000000B0  |  00 00 00 40 01 00 00 00 00 10 00 00 00 02 00 00  |  ...@............
00000000000000C0  |  04 00 00 00 00 00 00 00 05 00 02 00 00 00 00 00  |  ................
00000000000000D0  |  00 C0 01 00 00 06 00 00 99 DA 01 00 03 00 60 01  |  ..............`.
00000000000000E0  |  00 00 20 00 00 00 00 00 00 10 00 00 00 00 00 00  |  .. .............
00000000000000F0  |  00 00 10 00 00 00 00 00 00 10 00 00 00 00 00 00  |  ................
0000000000000100  |  00 00 00 00 10 00 00 00 00 00 00 00 00 00 00 00  |  ................
0000000000000110  |  00 00 01 00 EC 07 00 00 00 20 01 00 E0 01 00 00  |  ......... ......
0000000000000120  |  00 D0 00 00 A0 05 00 00 00 00 00 00 00 00 00 00  |  ................
0000000000000130  |  00 30 01 00 78 00 00 00 00 00 00 00 00 00 00 00  |  .0..x...........
0000000000000140  |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |  ................
0000000000000150  |  00 B1 00 00 28 00 00 00 00 00 00 00 00 00 00 00  |  ....(...........
0000000000000160  |  00 00 00 00 00 00 00 00 10 02 01 00 D0 01 00 00  |  ................
0000000000000170  |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |  ................
0000000000000180  |  00 00 00 00 00 00 00 00 2E 74 65 78 74 00 00 00  |  .........text...
```

The output is divided into three distinct columns:
- **Memory Offset (Left Side):** 16-digit uppercase hex showing byte position in the file
- **Hex View (Middle):** 16 space-delimited bytes in uppercase hex (%02X).
- **ASCII View (Right Side):** Printable ASCII character representation of the corresponding bytes.

## Compilation & Usage:
**Prerequisites:**  
- Any C standard compliant compiler such as gcc, clang, ...

**Build Instructions Example Using GCC:**
- gcc -Wall -Wextra -O3 main.c -o HexInspector

*HexInspector is the name of the program, you can name it anything you want

**Execution Examples:**
- **General Syntax:** .\HexInspector.exe \<path_to_file\>
- **Example:** .\HexInspector.exe HexInspector.exe

## Technical Journey & Challenges:
1. **The Signed char Extension Bug:**
- **Issue:** Initial tests using a char buffer[16] produced malformed hex formatting with padded FFFFFF characters for high-value bytes (example: FFFFFF90 instead of 90).
- **Root Cause:** In C, char is signed on most compilers. Values greater than 127 (0x7F) overflow into negative numbers. When passed to printf("%02X", ...) which expects an integer, sign extension converts negative values into 32-bit signed integers prefixed with 0xFFFFFF...
- **Fix:** Changed the buffer data type from char to unsigned char.

2. **Formatting & Alignment on Partial Reads:**
- **Issue:** Files rarely align perfectly to 16-byte boundaries, leaving the final row incomplete. Simply printing the remaining bytes breaks the alignment of the final ASCII column.
- **Fix:** Implemented a padding calculation (16 - bytesRead) that prints three spaces ("   ") per missing byte, preserving column alignment across arbitrary file sizes.

3. **Handling Non-Printable Control Characters:**
- **Issue:** Printing raw bytes directly to standard output rendered unpredictable terminal escape sequences, newline splits, and invisible control characters.
- **Fix:** Constrained printable output strictly to the valid ASCII boundary range: 0x20 through 0x7E. Any byte outside this range is printed as a dot (0x2E).

## Core Learnings:
- Binary file I/O requires explicit "rb" flag in fopen to prevent OS-level text translation.
- fread() returns the number of items successfully read, making it ideal for standard streaming loops.
- unsigned char is mandatory for raw byte representation to avoid implicit integer sign extension during formatting.
- Printable ASCII characters occupy decimal values 32 through 126 (0x20 to 0x7E).

## References & Resources:
- [File Handling in C](https://www.geeksforgeeks.org/c/basics-file-handling-c/)
- [ASCII Values in C](https://intellipaat.com/blog/ascii-values-in-c/)