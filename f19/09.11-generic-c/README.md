# Agenda

## Class Admin and Office Hours

- VM problems
- Valgrind errors (-Wall vs. Valgrind detection of errors)
	+ gcc -Wall

```c
void
foo(void)
{
	int i;  // java: i = 0, same in c -g
		// c -O2 i can be random

	...
	if (...) {
		i = N-1;
	}
	for (; i < N ; i++) {
		...
	}
}
```

	+ Valgrind

```c
void
foo(int a, int *x)
{
	if (a == 0) {
		*x = 0;
	}
}

void
bar(int *x)
{
	printf("%d\n", *x);
}

...main...
int myx;
foo(1, &myx);
bar(&myx);
```

- Prints in your implementation

**General rule: don't print in library code. Evar.**

	+ print vs. return value/exceptions
		- logging is an "exception"
	+ assertions (in #include <assert.h>)
		- cause program failure
		- their used to ensure invariants/assumption are held
			"the user must never pass a NULL linked list pointer to this function"
			assert(ll != NULL); // if not true: "assertion failure ll != NULL at line XXX"
	+ malloc failures
		- always check that it returns != NULL
		- APIs will specify a return value for that, errno.h, (int)ENOMEM

## Advanced Topic: Generic C

C doesn't have language features like generics in Java that enable you to implement data-structures independent of what they "hold", nor does it support inheritance and polymorphism for object oriented programming.

Think:

```java
HashMap hm = new HashMap<Integer>();
```

In C: `void *`

...and...

```java
public static <T> void sort(List<T> list, Comparator<? super T> c)
```

The C equivalent:

```c
       void qsort(void *base, size_t nmemb, size_t size,
                  int (*compar)(const void *, const void *));
```

> Sorts the specified list according to the order induced by the specified comparator. All elements in the list must be mutually comparable using the specified comparator (that is, c.compare(e1, e2) must not throw a ClassCastException for any elements e1 and e2 in the list).

(from [here](https://docs.oracle.com/javase/7/docs/api/java/util/Collections.html#sort(java.util.List,%20java.util.Comparator))

However, C does support a number of primitive features that are simple enough to implement many of the same behaviors.
These include:

- function pointers for generic operations on data-structures,
- structs of function pointers for polymorphic behavior, and
- macros to generate functions specialized for different types.

In this class, you'll see the first two of these used in threading APIs, and in operating system code.

### Function pointers

Goal: you want a data-structure to hold data, and be able to intelligently compare that data, but your data-structure is *generic* -- its code should work regardless the type of data it contains.

- Case study: qsort
- Sample implementation: sorter

### Structs of function pointers

A common technique to implement polymorphism
In short: A variable of a given type can have different behavior, based on how it was instantiated.
Example: read and write in Unix operate on pipes, devices, and files, but we're using read and write on the "file descriptor".
The system has to, at some point, call code specific for pipe/device/file.
Two options:

- Conditions (if pipe...)
- function pointers

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
