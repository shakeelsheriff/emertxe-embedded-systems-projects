OBJ:= $(patsubst %.c, %.o, $(wildcard *.c))
out.exe: $(OBJ)
	gcc -o $@ $^  
clean:
	rm *.o *.exe


