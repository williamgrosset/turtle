.phony setup:
setup: shell inf args

shell: shell.c
	# gcc shell.c -lreadline -lhistory -ltermcap -o shell
	gcc shell.c -lreadline -ltermcap -o shell.o

	#gcc shell.c -lreadline -lhistory -ltermcap -o shell
inf: inf.c
	gcc inf.c -o inf.o

args: args.c
	gcc args.c -o args.o

.PHONY clean:
clean:
	-rm -rf *.o *.exe

run: shell.o
	./shell.o
