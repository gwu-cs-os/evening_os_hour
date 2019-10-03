# Schedule

## Class Organization

- HW3
	- procrastination is not a viable strategy for this class
	- please take re-submission seriously
	- C and stack allocation
```c
void
foo(void)
{
	struct pstat *a = malloc(sizeof(struct pstat));
	...
	free(a);
}

/* why don't we do stack allocation instead? */
void
foo_stk_alloc(void)
{
	struct pstat a;
	...
}

/* make it global instead? */
struct pstat a;
void
foo_global(void)
{
	/* reference a */
	...
}

pthread_create(foo_global, ...)
pthread_create(foo_global, ...)
/* races on the global variable, so don't do this! */
```

- HW4
    - implementing threads
	- really must get this working (hw5!)

- what else?

- Use diff to update subsequent homework repos...won't be able to maintain the git history
	- `diff` produces a patch
	- `patch` will apply the patch

## Build systems

A build system's goals are to

- *organize the magical incantations*: automate the compilation and linking of source files into executables
- *process only what's necessary*: recompile only the changed portion of the source code, and the portion dependent on changed code
- *make maintaining the build system easier*: the build system should be a programming language that allows you to avoid redundant (copied) code

To motivate and give examples of each of these, see

- [`01_build_script`](https://github.com/gparmer/evening_os_hour/tree/master/f19/10.2-makefiles/01_build_script) - This shows how to use a shell script to encode the magical incantations of your build.
	This has a number of downsides including repetition within the script, and the fact that it can perform a single function.
	You could make the script arbitrarily complex and add arguments to do multiple operations (e.g. `clean` and `qemu` in `xv6`), but now you're *writing a build system*, and it won't track modified files and only rebuild them.
- [`02_simple_makefile`](https://github.com/gparmer/evening_os_hour/tree/master/f19/10.2-makefiles/02_simple_makefile) - A `Makefile` to organize the build process.
	We see a number of features here.

	1. Variables avoid repetition within the code,
	1. the `result: dependencies` rules denote the file that is to be generate as a result of a build step, and the dependencies on other necessary results for its construction, and
	1. regular expression-style rules such as `%.o:%.c` that say "generate a .o file from a .c file of the same prefix".

	The dependencies specify other necessary rules for execution, or files.
	In the latter case, a rule is only executed if the file changed since the last time you called `make`.
- [`03_featureful_makefile`](https://github.com/gparmer/evening_os_hour/tree/master/f19/10.2-makefiles/03_featureful_makefile) - this example includes

    - features to remove code repetition and to allow you to avoid `Makefile` updates by using regular expression-like wildcards, pattern substitution, and iteration,
	- compiler-integrated support to generate additional dependencies based on the `.h` files included in our `.c` files (via `DEP*` logic and `.d` files),
	- a demonstration of how to control the output of the `Makefile` and make it a little more human readable (though less debuggable), and
	- a rule to simply use some shell commands to create a tarball, along with a rule to spit out a bunch of `git` change information.

	So at this point, we have a build system that doesn't require updating when we add new `.c` and `.h` files, will accurately recompile files only in response to modifications to one of their dependencies, and that provides some additional features leveraging the shell.

Make is an old build system, and newer systems certainly exist.
I don't know nearly as much about most of them:

- [`Autoconf`](https://en.wikipedia.org/wiki/Autoconf) is a build system build system that will check system dependencies (library versions, if specific software is present, etc..) and create `Makefile`s for your project based on the dependencies.
- [`ant`](https://en.wikipedia.org/wiki/Apache_Ant) and [`Maven`](https://en.wikipedia.org/wiki/Apache_Maven) are popular build systems for Java.
- [`ninja`](https://en.wikipedia.org/wiki/Ninja_(build_system)) which is like `make` in that it is often the output target of build system generation tools, but it is not as powerful (nor as slow) as `make`.
- [`CMake`](https://en.wikipedia.org/wiki/CMake) is a cross-platform means of generating build systems in `make`, `ninja`, and other systems.
	It aims to simplify the build system creation, but removes some of the power of `make` (often a good thing).
- and [many more](https://en.wikipedia.org/wiki/List_of_build_automation_software)
