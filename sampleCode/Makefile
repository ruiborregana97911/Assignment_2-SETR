CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC = main.c cmdproc.c
OBJ = $(SRC:.c=.o)
EXEC = programa

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean

