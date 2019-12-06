all: gzzip

gzzip: Compressor.o Arhivator.o RLE.o BWT.o Huffman.o MTF.o main.o
	g++ Compressor.o Arhivator.o RLE.o BWT.o Huffman.o MTF.o main.o -o gzzip
	rm -rf *.o

main.o: main.cpp
	g++ -std=c++17 -Wall -pedantic -c main.cpp

Arhivator.o: Arhivator/Arhivator.cpp
	g++ -std=c++17 -Wall -pedantic -c Arhivator/Arhivator.cpp

RLE.o: Arhivator/Compressors/RLE/RLE.cpp
	g++ -std=c++17 -Wall -pedantic -c Arhivator/Compressors/RLE/RLE.cpp

BWT.o: Arhivator/Compressors/BWT/BWT.cpp
	g++ -std=c++17 -Wall -pedantic -c Arhivator/Compressors/BWT/BWT.cpp

MTF.o: Arhivator/Compressors/MTF/MTF.cpp
	g++ -std=c++17 -Wall -pedantic -c Arhivator/Compressors/MTF/MTF.cpp

Huffman.o: Arhivator/Compressors/Huffman/Huffman.cpp
	g++ -std=c++17 -Wall -pedantic -c Arhivator/Compressors/Huffman/Huffman.cpp

Compressor.o: Arhivator/Compressors/Compressor.cpp
	g++ -std=c++17 -Wall -pedantic -c Arhivator/Compressors/Compressor.cpp


clean:
	rm -rf *.o gzzip
