CC=gcc
CFLAGS=-g
FILES=*.c
EXEC=xorcrypt

raycaster: $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(EXEC)
