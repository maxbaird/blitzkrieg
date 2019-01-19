PROJECT       = blitzkrieg.out
CC            = gcc
CFLAGS        = -c -Wall -Werror -Wextra

.PHONY : all clean

all : $(PROJECT)

$(PROJECT) : blitzkrieg.o tile.o board.o dictionary.o
	$(CC) -o $@ $^

blitzkrieg.o : blitzkrieg.c
	$(CC) $(CFLAGS) $<

tile.o : tile.c
	$(CC) $(CFLAGS) $<

board.o : board.c
	$(CC) $(CFLAGS) $<

dictionary.o : dictionary.c
	$(CC) $(CFLAGS) $<

clean :
	rm -rf *.o *.out $(PROJECT)
