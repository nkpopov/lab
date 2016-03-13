
srcs   = main.c
objs   = $(srcs:.c=.o)
libs   = -lpcap -lasound
flags  = -Wall
target = a.out

$(target): $(objs)
	gcc -o $@ $^ $(flags) $(libs)

$(objs): %.o: %.c
	gcc -c $< $(flags)

clean:
	rm -f $(objs)
	rm -f $(target)

.PHONY: clean


