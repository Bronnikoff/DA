#ifndef RLE_H
#define RLE_H
#include <iostream>
#include <inttypes.h>
#include "../Compressor.hpp"

using namespace std;

///////////////////////////////////////////////
////////////// RLE ENCODE READY ///////////////
///////////////////////////////////////////////

class RLE : public Compressor{
    public:
        RLE();
        RLE(istream& inpt, ostream& otpt);
        RLE(istream* inpt, ostream* otpt);
    protected:
        void buffer_encode() override;
        void buffer_decode() override;

};

#endif