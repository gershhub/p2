.PHONY:clean

p2: main.o p2.o
	gcc $(CFLAGS) -o $@ $^

%.o:%.cc
	gcc -c $(CFLAGS) -o $@ $<
clean:
	rm p2 *.o
