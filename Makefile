all: map

map:
	g++ -std=c++11 -Wall -Werror -pedantic -g -ldl test3.cpp -o map

clean:
	rm -f ~* map