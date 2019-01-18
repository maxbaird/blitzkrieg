PROJECT       = planet_cracker.out
CC            = gcc
CFLAGS        = -c -Wall -Werror -Wextra

.PHONY : all clean

all : $(PROJECT)

$(PROJECT) : main.o
	$(CC) -o $@ $^

main.o : main.c
	$(CC) $(CFLAGS) $<

clean :
	rm -rf *.o *.out $(PROJECT)
