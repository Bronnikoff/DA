#include "RLE.hpp"

using namespace std;

RLE::RLE() : Compressor(){}

RLE::RLE(istream& inpt, ostream& otpt) : Compressor(inpt, otpt){}

RLE::RLE(istream* inpt, ostream* otpt) : Compressor(inpt, otpt){}

void RLE::buffer_encode(){
    int8_t count = 0;
    string buffer;
    buffer.clear();
    char last_ch = in_buffer[0]; // set unique
    // Always save last char from in buffer
    for(uint32_t i = 1; i < in_buffer.size(); ++i){
        // if repeat
        if(in_buffer[i] == last_ch){
            // if its first repeat, push in output unique char until repeat chars
            if(count < 0){
                out_buffer.push_back(static_cast<char>(count));
                out_buffer += buffer;
                buffer.clear();
                count = 0;
            }
            // we have +1 repeating char - last char
            ++count;
            // if num of repeats until this char = 127 => push back this info and go next
            if(count == INT8_MAX){
                out_buffer.push_back(static_cast<char>(count));
                out_buffer.push_back(last_ch);
                count = 0;
            }
        }else{ // - if not repeat
            // if its first unique char => push count and char of repeats
            if(count > 0){
                // count + 1 because last char also repeated
                ++count;
                out_buffer.push_back(static_cast<char>(count));
                out_buffer.push_back(last_ch);
                count = 0;
            }else{
                // if its not first unique char - push_back it in temp buffer
                // and increment count of unique
                --count;
                buffer.push_back(last_ch);
                if (count == INT8_MIN){
                    out_buffer.push_back(static_cast<char>(count));
                    out_buffer += buffer;
                    buffer.clear();
                    count = 0;
                }
            }
            
        }
        last_ch = in_buffer[i];
    }
    // we should define group for last char of input and 
    // push nesassary info in output
    if(count < 0){
        --count;
        buffer.push_back(last_ch);
        out_buffer.push_back(static_cast<char>(count));
        out_buffer += buffer;
    }else{
        ++count;
        out_buffer.push_back(static_cast<char>(count));
        out_buffer.push_back(last_ch);
    }
    
}

void RLE::buffer_decode(){
    uint32_t i = 0;
    int8_t count = 0;
    while(i < in_buffer.size()){
        count = static_cast<int8_t>(in_buffer[i++]);
        while(count < 0){
            out_buffer.push_back(in_buffer[i++]);
            ++count;
        }
        if(count > 0){
            char tmp = in_buffer[i++];
            while (count > 0){
                out_buffer.push_back(tmp);
                --count;
            }
        }
    }
}