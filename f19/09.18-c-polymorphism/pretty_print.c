#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define N_THINGS 32

struct things {
	void *all_the_things[N_THINGS];
};


typedef enum {
	THING_FIB,
	THING_PARMER
} thing_type_t;

struct thing {
	void (*print_fn)(void *this); /* the pointer passed to the print fn is essentially this/self */
	thing_type_t type;
};

int
fib(int a)
{
	if (a == 0) return 0;
	if (a == 1) return 1;
	return fib(a - 1) + fib(a - 2);
}

/* The two types of "things" */
struct fibonacci {
	struct thing thing;
	int f;
};

void
fib_print_fn(void *this)
{
	struct fibonacci *f = this;

	printf("fib(%d) = %d\n", f->f, fib(f->f));
}

struct parmer {
	struct thing thing;
	char *saying;
	int rinse_repeat;
};

void
parmer_print_fn(void *this)
{
	int i;
	struct parmer *gp = this;

	for (i = 0; i < gp->rinse_repeat; i++) {
		printf("%s ", gp->saying);
	}
	printf("\n");
}

void
print_em(struct things *ts)
{
	int i;

	for (i = 0; i < N_THINGS; i++) {

		struct thing *thing = ts->all_the_things[i];
		if (thing == NULL) continue;

		thing->print_fn(thing);
		/*
		 * This is not great because it requires that the
		 * generic data-structure know of each possible type
		 * of "thing" we can hold...i.e. clients can't define
		 * their own!!
		 */
		/* switch(thing->type) { */
		/* case THING_FIB: */
		/* { */
		/* 	struct fibonacci *f = (struct fibonacci *)thing; */
		/* 	printf("Have a fib struct looking for fib(%d)\n", f->f); */
		/* 	break; */
		/* } */
		/* case THING_PARMER: */
		/* 	break; */
		/* } */


		/* Not generic, go away! */
		/* printf("%p of type %d\n", thing, thing->type); */ /* not all that interesting, right? */
	}
}

void
main(void)
{
	int i;

	struct things ts;
	for (i = 0; i < N_THINGS; i++) {
		ts.all_the_things[i] = NULL;
	}

	char *the_words[] = {
		"OS = sandwich good stuff (that I can't eat!!!)",
		"which is better?",
		"its a trap!",
		"TRADE-OFFS"
	};

	for (i = 0; i < 8; i += 2) {
		struct fibonacci *f;
		struct parmer *gp;

		f                 = malloc(sizeof(struct fibonacci));
		assert(f);
		f->f              = i/2;
		f->thing.type     = THING_FIB;
		f->thing.print_fn = fib_print_fn;

		gp   = malloc(sizeof(struct parmer));
		assert(gp);
		*gp  = (struct parmer) {
			.thing.type     = THING_PARMER,
			.thing.print_fn = parmer_print_fn,
			.saying         = the_words[i/2],
			.rinse_repeat   = i+1
		};

		ts.all_the_things[i]   = f;
		ts.all_the_things[i+1] = gp;
	}

	print_em(&ts);
}
