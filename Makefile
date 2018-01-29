.phony setup:
setup: shell

shell: shell.c
	# gcc shell.c -lreadline -lhistory -ltermcap -o shell
	gcc shell.c -lreadline -ltermcap -o shell.o

	#gcc shell.c -lreadline -lhistory -ltermcap -o shell

run: shell.o
	./shell.o

.PHONY clean:
clean:
	-rm -rf *.o *.exe
