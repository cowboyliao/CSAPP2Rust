CC = gcc
CFLAGS = -Wall -Wextra
SRCS = main.c math_operations.c
OBJS = $(SRCS:.c=.o)
TARGET = a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f $(OBJS) $(TARGET)
