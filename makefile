major: shellmain.o piping.c ioredirection.o custom_shell.o builtin.o
	gcc -o major shellmain.c piping.c ioredirection.c custom_shell.c builtin.c
shellmain.o: shellmain.c headers.h
	gcc -c shellmain.c
piping.o: piping.c headers.h
	gcc -c piping.c
ioredirection.o: ioredirection.c headers.h
	gcc -c ioredirection.c
custom_shell.o: custom_shell.c headers.h
	gcc -c custom_shell.c
builtin.o: builtin.c headers.h
	gcc -c builtin.c 
clean:
	rm *o major

