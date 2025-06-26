CC=gcc
CFLAGS=-Wall -Wextra -g
TARGET=mini-shell

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET) 