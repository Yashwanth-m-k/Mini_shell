OBJ=$(patsubst %.c, %.o, $(wildcard *.c))
myoutput.exe : $(OBJ)
	gcc -o $@ $^
clean:
	rm *.exe *.o

