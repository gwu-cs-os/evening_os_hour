headers:
	gcc -I. -o main main.c

preproc:
	gcc -I. -E -o main.preproc main.c

visibility:
	gcc -I. -o main main.c more.c

clean:
	rm -rf *.o *.preproc main
