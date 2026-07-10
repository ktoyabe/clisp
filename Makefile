
SRCS = main.c 
OBJS = $(SRCS:.c=.o)
CC   = gcc
CFLAGS = -g
TARGET= clisp

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) *.db

test: $(TARGET)
	uv run pytest

format: *.c
	clang-format --style=file:".clang-format" -i *.c
