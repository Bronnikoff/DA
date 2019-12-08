#include "RLE.hpp"
#include <iostream>
#include <fstream>

int main(){
    int a;
    cout << "Enter 0 if decode, 1 if encode: ";
    cin >> a;
    if(a){ 
        ifstream is("test.pdf", ios::in | ios::binary);
        ofstream os("test1.gz", ios::out | ios::binary);
        RLE cmp(is, os);
        cmp.encode();
        is.close();
    }else{
        ifstream is("test1.gz", ios::in | ios::binary);
        ofstream os("test2.pdf", ios::out | ios::binary);
        RLE cmp(is, os);
        cmp.decode();
        is.close();
    }
    cout << endl;
    return 0;
}