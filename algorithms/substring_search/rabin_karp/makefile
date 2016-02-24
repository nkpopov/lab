
src = main.cpp
obj = $(src:.cpp=.o)
tgt = a.out

$(tgt): $(obj)
	g++ -std=c++11 -o $@ $^

$(obj): %.o: %.cpp
	g++ -std=c++11 -c $< -g

clean:
	rm -f $(tgt)
	rm -f $(obj)

.PHONY: clean


