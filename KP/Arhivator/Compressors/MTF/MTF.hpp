#ifndef MTF_H
#define MTF_H

#include <iostream>
#include <inttypes.h>
#include "../Compressor.hpp"
#include <list>

using namespace std;

class MTF : public Compressor{
    public:
        MTF();
        MTF(istream& inpt, ostream& otpt);
        MTF(istream* inpt, ostream* otpt);
        
    protected:
        void buffer_encode() override;
        void buffer_decode() override;
    private:
        list<uint8_t> alfabet;
        uint8_t move_to_front_encode(uint8_t c);
        uint8_t move_to_front_decode(uint8_t c);
};

#endif