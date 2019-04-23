PROJECT       = blitzkrieg.out
CC            = gcc
CFLAGS        = -c -O3 -Wall -Werror -Wextra
LDFLAGS       = -lpthread
SRC_DIR       = src
OBJ_DIR       = obj
OBJECTS       = main.o blitzkrieg.o tile.o board.o lexis.o trie.o finder.o print.o config.o welcome.o
OBJS          = $(patsubst %, $(OBJ_DIR)/%, $(OBJECTS))
MKDIR_P       = mkdir -p
CLEAN_R       = rm -rf

.PHONY : all clean directories

all : directories $(PROJECT)

debug : CFLAGS += -g3
debug : CFLAGS += -DDEBUG
debug : clean directories $(PROJECT)

$(PROJECT) : $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

directories : ${OBJ_DIR}

${OBJ_DIR} :
	${MKDIR_P} ${OBJ_DIR}

clean :
	$(CLEAN_R) $(PROJECT) $(OBJ_DIR)
