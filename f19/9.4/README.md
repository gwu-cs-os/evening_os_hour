# Topics

## Purpose and Rules

### Purpose

Provide extended Q&A time for the class, and coverage of skills and background in effective systems programming (beyond what's covered in the class).

### Ground rules

- Be appropriate, be kind, be respectful
- Please ask questions; keep them relatively short
- Stop me if something is distracting or needs clarification
- Make suggestions for future events on Piazza

## Understanding C development

Starting out with C.
C is a programming language that lets you do many dumb things.
You want to restrict your C to a subset of C that is responsible, easier to read, and more tractable to develop.
This is a complicated topic (see the Composite Style Guide).
These are some of the higher-order bits you should know.

### The C preprocessor

- purpose of header files and what goes there
    - `-E`
- header guards
- macro hygiene and paren <skip>

### Visibility and storage modifiers

Functions:

- static
- static inline
- extern

Data:

- static

Conventions:

- *always* use header guards
- no data in headers
- generally avoid extern
	- use headers and declarations for functions, and avoid direct data accesses across files (efficient with LTO), instead
- public facing API in header file
- namespacing
	- `_` namespacing to denote non-public functions in headers
	- static for non-API (e.g. helper) functions

### `nm` and `objdump` for introspection

### C compiler flags

- debugging and optimizations
- objects
- warnings
- architecture
