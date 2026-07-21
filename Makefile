


CC   = gcc
CFLAGS = -g

TARGET= clisp
OBJDIR = objs
SRCS = main.c common.c lexer.c cstring.c object.c parser.c eval.c hashmap.c env.c repl.c
OBJS = $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(SRCS)))

all: $(TARGET)

$(OBJDIR)/%.o: %.c
	@mkdir -p objs
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
