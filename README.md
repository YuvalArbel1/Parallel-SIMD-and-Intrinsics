<h1 align="center">
  
  <br>
<img width="200" alt="pStringMem" src="https://github.com/TalMizrahii/Assembly-Pstrings/assets/103560553/cc350fec-e689-4cd4-bdac-b3986187c914">

  <br>
  Assembly SIMD and C Intrisics
  <br>
</h1>

<h4 align="center"> </h4>

<p align="center">
  <a href="#description">Description</a> •
  <a href="#part-i---strings">Part I - Strings</a> •
  <a href="#part-ii---formulas">Part II - Formulas</a> •
  <a href="#dependencies">Dependencies</a> •
  <a href="#installing-and-executing">Installing And Executing</a> •
  <a href="#author">Author</a> 
</p>

## Description

### About The Exercise

![download](https://github.com/TalMizrahii/Parallel-SIMD-and-Intrinsics/assets/103560553/6b4b0a3f-34e8-4b84-af30-638abd585e58)


The Assembly and C Exercise aims to practice assembly language programming using SSE/AVX instructions and C SSE/AVX intrinsics. It consists of two unrelated parts: string processing and floating-point calculations. Each part comprises various tasks and utilizes different source files for its environment.

![download](https://github.com/TalMizrahii/Parallel-SIMD-and-Intrinsics/assets/103560553/eb54db1c-8269-4bd8-a120-1f2e2288453b)

## Part I - Strings

In this part, two string operators are implemented using SSE/AVX instructions and intrinsics.

### Hamming Distance

![download](https://github.com/TalMizrahii/Parallel-SIMD-and-Intrinsics/assets/103560553/ef9f1921-2ed3-4a47-8387-eeac0e586e1d)

The Hamming distance between two strings is calculated as the number of positions at which the corresponding symbols are different. The implementation is done in assembly language only, specifically in the file named `hamming.s`.

### Base 64 Distance

![images](https://github.com/TalMizrahii/Parallel-SIMD-and-Intrinsics/assets/103560553/719b431d-76ef-4d06-830b-333c32940a0f)

The Base64 distance between two strings involves:
1. Removing non-base 64 characters while maintaining the order of every other character.
2. Calculating the numbers represented by the strings and subtracting the first from the second.
The implementation is in C using intrinsics, in the file named `b64.c`.

## Part II - Formulas

This part involves mathematical formulas applied to arrays of floating-point numbers.

### The First Formula

Formula 1 calculates the expression:

![2](https://github.com/TalMizrahii/Parallel-SIMD-and-Intrinsics/assets/103560553/ab7364b0-bfdd-4df2-9f09-be1b8a863a46)

The implementation is in C using intrinsics, in the file named `formula1.c`.

### The Second Formula

Formula 2 computes the expression:

![1](https://github.com/TalMizrahii/Parallel-SIMD-and-Intrinsics/assets/103560553/5b4fe6eb-762d-4d11-89a4-e22952de3ed2)


The implementation is in assembly language, specifically in the file named `formula2.s`.

## Dependencies

This exercise has the following dependencies:

- **SSE/AVX Support**: Ensure that the target platform supports SSE/AVX instructions for optimal performance.
- **Compiler and Assembler**: A compatible C compiler and assembler are required to compile and assemble the provided source files.

## Installing And Executing

To install and run the program you use [Git](https://git-scm.com). From your command line:

```bash
# Clone this repository.
$ git clone https://github.com/YuvalArbel1/Parallel-SIMD-and-Intrinsics.git

# Navigate to the repository directory:
$ cd Parallel-SIMD-and-Intrinsics
```

### To run strings:
```bash
# Enter strings:
$ cd strings

# Compile the program:
$ make

# Run the program:
$ make run
```

### To run formulas:
```bash
# Enter formulas:
$ cd formulas

# Compile the program:
$ make

# Run the program:
$ make run
```
## Author

* [@Yuval Arbel](https://github.com/YuvalArbel1)
