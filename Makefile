PROJECT       = blitzkrieg.out
CC            = gcc
CFLAGS        = -c -O3 -Wall -Werror -Wextra
LDFLAGS       = -lpthread

.PHONY : all clean

all : $(PROJECT)

debug : CFLAGS += -g3
debug : clean $(PROJECT)

$(PROJECT) : main.o blitzkrieg.o tile.o board.o lexis.o avltree.o finder.o print.o config.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c %.h
	$(CC) $(CFLAGS) $<

clean :
	rm -rf *.o *.out $(PROJECT)
