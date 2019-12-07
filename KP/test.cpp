#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

        typedef struct Node{
            uint32_t count;
            char value;
        }node;
        typedef struct functor{
            bool operator()(const node& lhs, const node& rhs){
                return lhs.count > rhs.count;
            }
        }comparator;

int main(){

    ifstream is("uin.cpp", ios::in | ios::binary);
    if(!is){
        cout << "Error of open" << endl;
    }
    return 0;
}