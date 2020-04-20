CC=gcc
CFLAGS=-g
FILES=*.c
EXEC=xorcrypt

xorcrypt: $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(EXEC)
