#include "Compressor.hpp"
#include <iostream>

using namespace std;

Compressor::Compressor(){
    is = &cin;
    os = &cout;
    //decoding = false;
}

Compressor::Compressor(istream& inpt, ostream& otpt){
    is = &inpt;
    os = &otpt;
    //decoding = false;
}

Compressor::Compressor(istream* inpt, ostream* otpt){
    is = inpt;
    os = otpt;
    //decoding = false;
}


void Compressor::set_input(istream& inpt){
    is = &inpt;
}

void Compressor::set_output(ostream& otpt){
    os = &otpt;
}

void Compressor::set_input(istream* inpt){
    is = inpt;
}

void Compressor::set_output(ostream* otpt){
    os = otpt;
}

void Compressor::encode(){
    // alloc need memory
    in_buffer.resize(max_buffer_size);

    while (*is){
        // read full buffer for encode
        is->read(&in_buffer[0], max_buffer_size);
        // if read less thn max_size
        if(is->gcount() < max_buffer_size){
            in_buffer.resize(is->gcount());
            until_size += is->gcount;
        }else{
            until_size += max_buffer_size;
        }


        // encoding buffer
        buffer_encode();


        //writing encoded buffer in output
        uint32_t size_to_write = out_buffer.size();
        os->write(reinterpret_cast<const char*>(&size_to_write), sizeof(uint32_t));
        os->write(out_buffer.c_str(), size_to_write);
        after_size += size_to_write + sizeof(uint32_t);
    }
}

void Compressor::decode(){
    while(*is){
        // Reading buffer size and buffer from input for decode
        uint32_t size_to_read = 0;
        is->read(reinterpret_cast<char *>(&size_to_read), sizeof(uint32_t));
        in_buffer.resize(size_to_read);
        is->read(&in_buffer[0], size_to_read);
        until_size += size_to_read + sizeof(uint32_t);

        // decoding
        buffer_decode();


        // writing decoded buffer in output 
        uint32_t size_to_write = out_buffer.size();
        os->write(out_buffer.c_str(), size_to_write);
        after_size += size_to_write;
    }
}

uint64_t Compressor::get_until_size(){
    return until_size;
}


uint64_t Compressor::get_after_size(){
    return after_size;
}