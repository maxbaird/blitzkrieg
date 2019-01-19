PROJECT       = planet_cracker.out
CC            = gcc
CFLAGS        = -c -Wall -Werror -Wextra

.PHONY : all clean

all : $(PROJECT)

$(PROJECT) : main.o tile.o board.o
	$(CC) -o $@ $^

main.o : main.c
	$(CC) $(CFLAGS) $<

tile.o : tile.c
	$(CC) $(CFLAGS) $<

board.o : board.c
	$(CC) $(CFLAGS) $<

clean :
	rm -rf *.o *.out $(PROJECT)
