CC = gcc
CFLAGS = -Wall -Wextra

SRCS = main.c encryption.c decryption.c keygen.c validation.c error_handling.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o file_encryption_tool -lcrypto

clean:
	rm -f file_encryption_tool
