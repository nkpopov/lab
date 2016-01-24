
src := \
main.c \
lsh_builtins.c \

inc := \
lsh_builtins.h \

target := lsh.out
flags  := -g -Wall -pedantic
objs   := $(src:.c=.o)

$(target): $(objs)
	gcc -o $@ $^

$(objs): $(inc)
$(objs): %.o: %.c
	gcc -c $< $(flags)

clean:
	rm -rf $(objs)
	rm -rf $(target)

.PHONY: clean

