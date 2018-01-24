.phony all:
all: shell inf args

shell: shell.c
	gcc shell.c -lreadline -lhistory -ltermcap -o shell

inf: inf.c
	gcc inf.c -o inf

args: args.c
	gcc args.c -o args

.PHONY clean:
clean:
	-rm -rf *.o *.exe
