#ifndef BWT_H
#define BWT_H

#include <iostream>
#include <inttypes.h>
#include <vector>

#include "../Compressor.hpp"

using namespace std;

////////////////////////////////////////////////////////
/////////// BURROWS-WHEELER TRANSFORM READY ////////////
////////////////////////////////////////////////////////

class BWT : public Compressor{
    public:
        BWT();
        BWT(istream& inpt, ostream& otpt);
        BWT(istream* inpt, ostream* otpt);
    protected:
        void buffer_encode() override;
        void buffer_decode() override;
    private:
        uint32_t position; // position in sorted cyclic table
        uint32_t count_suff_array(vector<uint32_t>& array); // returns position
        uint32_t count_karkainen_sanders(vector<uint32_t>& array);
};


#endif