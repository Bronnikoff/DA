#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#define NUMBER_OF_TESTS 110000


int main ()
{
    std::vector<std::string> vector_string;
    std::vector<std::string> alphabet = {"a\0","b\0", "c\0", "d\0", "e\0", "f\0", "g\0", "h\0", "i\0", "j\0", "k\0", "l\0", "m\0", "n\0", "o\0", "p\0", "q\0", "r\0", "s\0", "t\0", "u\0", "v\0", "w\0", "x\0", "y\0", "z\0"};
    std::ofstream file;

    srand(time(0));
    int random_number, random_number_1;
    file.open("./file");
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        random_number = 1 + rand() % 256;
        std::string str;
        for (int i = 0; i < random_number; i++) {
            random_number_1 = rand() % (alphabet.size() - 1);
            str += alphabet[random_number_1];
        }
        vector_string.push_back(str);
        file << "+ " << vector_string[i] << ' ' << rand() << std::endl;
    }
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        random_number = rand() % (vector_string.size() - 1);
        random_number_1 = rand() % (vector_string.size() - 1);
        std::swap(vector_string[random_number], vector_string[random_number_1]);
    }

    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        file << vector_string[i] << std::endl;
        file << "- " << vector_string[i] << std::endl;
    }
    file << "! Save loadfile" << std::endl;
    file << "! Load loadfile" << std::endl;
    return 0;
}
