#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include <iostream>
#include <inttypes.h>
#include <iomanip>

using namespace std;

class Compressor{
    public:
        Compressor(); // basic constructor, set input and output as standart
        Compressor(istream& inpt, ostream& otpt); // constructor when streams known
        Compressor(istream* inpt, ostream* otpt); // constructor when streams known
        void set_input(istream& inpt); // set input stream
        void set_input(istream* inpt); // set input stream
        void set_output(ostream* otpt); // set output stream
        void set_output(ostream& otpt); // set output stream
        uint64_t get_until_size();
        uint64_t get_after_size();
        void encode(); // main encode function of compressor
        void decode(); // main decode function of compressor
        virtual ~Compressor(){}; // nothing to do:)
                
    protected:
        virtual void buffer_encode() = 0; // function of encodeing by settedd algorithm, override
        virtual void buffer_decode() = 0; // function of decodeing by setted algorithm if it was encoded by this algo, override
        uint64_t until_size = 0;
        uint64_t after_size = 0;
        istream* is;
        ostream* os;
        string in_buffer;
        string out_buffer;
        const uint32_t max_buffer_size = 4194304; // 4Mb
};

#endif