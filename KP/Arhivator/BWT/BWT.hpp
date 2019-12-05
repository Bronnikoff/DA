#ifndef BWT_H
#define BWT_H

#include <iostream>
#include <inttypes.h>
#include "../Compressor.hpp"

using namespace std;

class BWT : public Compressor{
    public:
        BWT();
        BWT(istream& inpt, ostream& otpt);
        BWT(istream* inpt, ostream* otpt);
        void buffer_encode() override;
        void buffer_decode() override;

};


#endif