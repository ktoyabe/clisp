
SRCS = main.c common.c lexer.c cstring.c
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
	rm -f $(TARGET) $(OBJS)

test: $(TARGET)
	uv run pytest

format: *.c
	clang-format --style=file:".clang-format" -i *.c
