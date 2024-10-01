# Compiler for your own programming language -2022

## Description of the project

This project is the development of a compiler for its own programming language -2022. The compiler translates the source code of the program into the appropriate sequence of commands that can be executed by the computer.

### Compiler composition:
- **Lexical analyzer** — performs the splitting of the source code into tokens.
- **The parser** — checks the correctness of the program syntax.
- **Semantic analyzer** — checks the logical correctness of the program.
- **Code Generator** — converts source code into assembly language code.

## Programming language -2022

- **Typing**: A strongly typed, procedural language.
- **Supported data types**:
- Unsigned integer type (`int')
- String type (`string`)
- **Standard functions for working with strings**:
- Lexicographic string comparison.
  - Calculation of the string length.

### Language Features:
- Does not support type conversion.
- The language is not object-oriented.
- Equality, inequality, and magnitude comparison operations are supported.
- Data entry is not supported.

### Alphabet of the language:
It is based on the Windows-1251 encoding and includes:
- Latin and Cyrillic letters.
- Numbers [0-9].
- Special characters: `(){} , ; : + - / * % > < ! '`.

## The main objectives of the project:

1. **Development of a programming language specification** — definition of the syntax and semantics of the language.
2. **Development of the translator structure** — creation of architecture for all stages of translation.
3. **Implementation of the lexical analyzer** — splitting the source text into tokens.
4. **Implementation of the parser** — checking the correctness of the program structure.
5. **Implementation of the semantic analyzer** — logical verification of the program.
6. **Implementation of expression conversion** — conversion of expressions into Polish notation for subsequent generation of assembly code.
7. **Implementation of the code generator** — translation of the source text into assembly language.
8. **Software testing** — checking the functionality of the compiler and its correctness.

## Calculating expressions

- Standard operations are used: equality, inequality, comparison of values.
- Expressions can include calls to functions that return integer values.
- **Expression Conversion**: Before code generation, expressions are converted to Polish notation to facilitate their execution at the assembler level.

## Features of function calls

- The language uses the **stdcall** calling convention:
- All function parameters are passed through the stack.
  - Memory release is performed by the called function.
  - Parameters are passed from right to left.

## Object code generation

The source code in the -2022 language is compiled into an assembly language, and then converted into object code for execution on the target machine.

## Entry point

Each program in the -2022 language must contain the main function from which the program execution begins.

## Preprocessor

A preprocessor is not provided in the -2022 programming language.

## Conclusion

The project covers the complete process of creating a compiler for the programming language -2022, starting from the lexical analysis of the source text, ending with the generation of object code.
