// Made by Max Bronnikov
#ifndef ARHIVATOR_H
#define ARHIVATOR_H

#include <iostream>
#include <fstream>
#include <exception> // для std::exception
#include "Compressors/Compressor.hpp"
#include "Compressors/RLE/RLE.hpp"
#include "Compressors/MTF/MTF.hpp"
#include "Compressors/Huffman/Huffman.hpp"
#include "Compressors/BWT/BWT.hpp"

using namespace std;



class MyException: public std::exception
{
private:
	string m_error;
 
public:
	MyException(string error) : m_error(error){}
 
	const char* what() const noexcept { return m_error.c_str(); } 
};



class Arhivator{
    public:
        Arhivator();
        void set_decode();
        void set_stdoutput();
        void set_path(string& str);
        void set_hard();
        void set_easy();
        void set_keep();
        void set_recursive();
        void set_check();
        void set_information();
        void start();

    private:
        void decode_file(string& path);
        void encode_file(string& path);
        void check_info(istream& is);
        // void decode_dir(const string& path);
        // void encode_dir(const string& path);
        void encode_stream(istream& is, ostream& os);
        void decode_stream(istream& is, ostream& os);
        void from_stream_to_stream(istream& is, ostream& os);
        uint32_t hash_count(istream& is);
        bool check_hash(istream& is);
        bool check_zip(istream& is);

        bool stdinput; // without filename

        bool stdoutput; // -c
        bool hard; // -9
        bool easy; // -1
        bool keep; // -k
        bool recursive; // -r
        bool decode; // -d
        bool check; // -t
        bool information; // -l

        uint32_t max_size_buffer = 4194304; // size of bufer for excange and hash counting
        string path; // file or directory path
        const string prefix = "\3\2\1\0"; // for check format

};

#endif