CC = gcc
CFLAGS = -lpthread -Wall -Wextra
SRCS = config.c sendconfig.c astrodata.c file.c menu.c netpack.c semaphore.c net.c flist.c msg.c main.c
OBJS = config.o sendconfig.o astrodata.o file.o menu.o netpack.o semaphore.o net.o flist.o msg.o main.o
P = lionel
P_PATH = ../$(P)

$(P): $(OBJS)
	$(CC) -o $(P_PATH) $^ $(CFLAGS)

all: $(MAIN)
	
.PHONY: clean

clean:
	rm -f $(OBJS) $(P_PATH)
