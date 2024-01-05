CC = gcc
CFLAGS = -Wall -Wextra
LIBS= -lcrypto

SRCS = main.c encryption.c decryption.c keygen.c validation.c error_handling.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o file_encryption_tool $(LIBS)

clean:
	rm -f file_encryption_tool
