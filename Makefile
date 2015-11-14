.PHONY:clean
CC=gcc
CFLAGS=-Wall -g
BIN=minishell
OBJS=main.o parse.o init.o execute.o builtin.o
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
