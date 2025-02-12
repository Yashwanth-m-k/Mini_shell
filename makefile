OBJ=$(patsubst %.c, %.o, $(wildcard *.c))
yashu.exe : $(OBJ)
	gcc -o $@ $^
clean:
	rm *.exe *.o

