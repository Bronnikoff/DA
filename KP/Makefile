all: gzzip

gzzip: Arhivator.o main.o
	g++  Arhivator.o main.o -o gzzip
	rm -rf *.o

main.o: main.cpp
	g++ -c main.cpp

Arhivator.o: Arhivator/Arhivator.cpp
	g++ -c Arhivator/Arhivator.cpp


clean:
	rm -rf *.o gzzip
