#include "MTF.hpp"

using namespace std;

// TO DO

// change algo for do possible count and insert numbers more than 127

MTF::MTF() : Compressor(){}

MTF::MTF(istream& inpt, ostream& otpt) : Compressor(inpt, otpt){}

MTF::MTF(istream* inpt, ostream* otpt) : Compressor(inpt, otpt){}

void MTF::buffer_encode(){
    alfabet.clear();
    // init alfabet
    for(uint8_t i = 0; i < 256; ++i){
        alfabet.push_back(i);
    }

    // ENCODE:
    for(char c : in_buffer){
        out_buffer.push_back(
            static_cast<char>(
                move_to_front_encode(
                    static_cast<uint8_t>(c)
                )
            )
        );
    }
}

void MTF::buffer_decode(){
    alfabet.clear();
    // init alfabet:
    for(uint8_t i = 0; i < 256; ++i){
        alfabet.push_back(i);
    }

    // DECODE:
    for(char c : in_buffer){
        out_buffer.push_back(
            static_cast<char>(
                move_to_front_decode(
                    static_cast<uint8_t>(c)
                )
            )
        );
    }
}

uint8_t MTF::move_to_front_encode(uint8_t c){
    uint8_t i = 0;
    for (auto it = alfabet.begin(); ; ++it, ++i){
        if(*it == c){
            alfabet.erase(it);
        }
    }
    alfabet.push_front(c);
    return i;
}


uint8_t MTF::move_to_front_decode(uint8_t c){
    auto it = alfabet.begin();
    advance(it, c);
    uint8_t i = *it;
    alfabet.erase(it);
    alfabet.push_front(i);
    return i;
}