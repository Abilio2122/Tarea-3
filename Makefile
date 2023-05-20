CC = gcc
CFLAGS = -g -Wall

debug: main.c
	$(CC) $(CFLAGS) -o main-debug main.c