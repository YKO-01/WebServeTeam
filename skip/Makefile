CC=c++
exec = server.o
sources = tst.cpp
objects = $(sources:.c=.o)

$(exec): $(objects)
	gcc $(objects)  -o $(exec)

%.o: %.c %.h
	gcc -c $(flags) $< -o $@