CC = gcc
CFLAGS = -Wall -Wextra
SRCS = config.c file.c menu.c netpack.c net.c main.c
OBJS = config.o file.o menu.o netpack.o net.o main.o
P = mcgruder
P_PATH = ../$(P)

$(P): $(OBJS)
	$(CC) -o $(P_PATH) $^ $(CFLAGS)

all: $(MAIN)
	
.PHONY: clean

clean:
	rm -f $(OBJS) $(P_PATH)
