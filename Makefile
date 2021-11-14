CFLAGS := -g -Wall -Wextra

all: test
test: test.o sp_tree.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) test *.o
