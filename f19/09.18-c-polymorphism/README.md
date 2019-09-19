# Agenda

## Class Admin and Office Hours

- Using the PQ O(N) in the PQ O(1)
- Ring buffer full/empty

## Advanced Topic: Polymorphism in C

C supports a number of primitive features that are simple enough to implement object-variant behaviors: polymorphism!
These include:

- function pointers for generic operations on data-structures (see last week!),
- structs of function pointers for polymorphic behavior, and
- macros to generate functions specialized for different types (not this class).

### Structs of function pointers

A common technique to implement polymorphism
In short: A variable of a given type can have different behavior, based on how it was instantiated.
Example: read and write in Unix operate on pipes, devices, and files, but we're using read and write on the "file descriptor".
See the top part of the system diagram [here](https://upload.wikimedia.org/wikipedia/commons/3/30/IO_stack_of_the_Linux_kernel.svg) to see what this looks like graphically.
The system has to, at some point, call code specific for pipe/device/file.
Two options:

- Conditions (if pipe {...} else if file {...})
- object-specific function pointers

See xv6's file.c: {fileread, filewrite} to see how it handles this.
Want more information?
Read about the VFS (virtual file system) layer in Linux.

### Object Orientation

The object models for Java (based on explicit single inheritance), and javascript (based on prototype inheritance of properties) can both be implemented in C.
Unfortunately, there is quite a bit to be desired in terms of code clarity and "beauty", so go into the exercise with low expectations there.
You would not *use* these implementations.
Think of them mainly as educational.

https://github.com/gparmer/c_object_models_instructional

- Polymorphism: a struct of function pointers that are specialized for the type of data
- Inheritance: that struct contains the parent's struct of function pointers so that it can inherit (and override) those implementations
- Prototype-driven inheritance: all "objects" are maps from keys to values (some of those values are functions), and a "child" can inherent the keys/values from a parent.
