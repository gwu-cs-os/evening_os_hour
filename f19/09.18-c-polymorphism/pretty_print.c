#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define N_THINGS 32

struct things {
	void *all_the_things[N_THINGS];
};

int
fib(int a)
{
	if (a == 0) return 0;
	if (a == 1) return 1;
	return fib(a - 1) + fib(a - 2);
}

void
print_em(struct things *ts)
{
	int i;

	for (i = 0; i < N_THINGS; i++) {
		void *thing = ts->all_the_things[i];
		if (thing == NULL) continue;
		printf("%p\n", thing); /* not all that interesting, right? */
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


	/* The two types of "things" */
	struct fibonacci {
		int f;
	};
	struct parmer {
		char *saying;
		int rinse_repeat;
	};
	char *the_words[] = {
		"OS = sandwich good stuff (that I can't eat!!!)",
		"which is better?",
		"its a trap!",
		"TRADE-OFFS"
	};

	for (i = 0; i < 8; i += 2) {
		struct fibonacci *f;
		struct parmer *gp;

		f    = malloc(sizeof(struct fibonacci));
		assert(f);
		f->f = i;
		gp   = malloc(sizeof(struct parmer));
		assert(gp);
		*gp  = (struct parmer) {
			.saying       = the_words[i],
			.rinse_repeat = i
		};

		ts.all_the_things[i]   = f;
		ts.all_the_things[i+1] = gp;
	}

	print_em(&ts);
}
