PROJECT       = blitzkrieg.out
CC            = gcc
CFLAGS        = -c -g -Wall -Werror -Wextra

.PHONY : all clean

all : $(PROJECT)

$(PROJECT) : blitzkrieg.o tile.o board.o lexis.o avltree.o finder.o
	$(CC) -o $@ $^

blitzkrieg.o : blitzkrieg.c
	$(CC) $(CFLAGS) $<

tile.o : tile.c
	$(CC) $(CFLAGS) $<

board.o : board.c
	$(CC) $(CFLAGS) $<

lexis.o : lexis.c
	$(CC) $(CFLAGS) $<

avltree.o : avltree.c
	$(CC) $(CFLAGS) $<

finder.o : finder.c
	$(CC) $(CFLAGS) $<

clean :
	rm -rf *.o *.out $(PROJECT)
