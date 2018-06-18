all: Widget
	clang++ -Wall -Wextra -Wpedantic -std=c++14 $^.a t.cpp
	./a.out


Widget: Widget.cpp
	clang++ -Wall -Wextra -Wpedantic -std=c++14 -c $^ -o $@.o
	ar rvs $@.a $@.o

clean:
	rm -f a.out *.a *.o
