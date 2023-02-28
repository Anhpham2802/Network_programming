CC=gcc
CFLAGS=-w

DEPS = double_linklist.h

OBJ = user_management.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

user_management: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o user_management
