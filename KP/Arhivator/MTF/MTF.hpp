#ifndef MTF_H
#define MTF_H

#include <iostream>
#include <inttypes.h>
#include "../Compressor.hpp"

using namespace std;

class MTF : public Compressor{
    public:
        MTF();
        MTF(istream& inpt, ostream& otpt);
        MTF(istream* inpt, ostream* otpt);
        void buffer_encode() override;
        void buffer_decode() override;

};

#endif