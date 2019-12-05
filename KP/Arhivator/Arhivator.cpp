#include "Arhivator.hpp"
#include <cstdio>

using namespace std;

Arhivator::Arhivator(){
    stdinput = true;
    decod = false;
    stdoutput = false;
    hard = false;
    easy = false;
    keep = false;
    recursive = false;
    check = false;
    information = false;
    path.clear();
}

void Arhivator::set_stdoutput(){
    stdoutput = true;
}

void Arhivator::set_path(string str){
    path = str;
    stdinput = false;
}

void Arhivator::set_hard(){
    hard = true;
    if(easy){
        easy = false;
    }
}

void Arhivator::set_easy(){
    easy = true;
    if(hard){
        hard = false;
    }
}

void Arhivator::set_recursive(){
    recursive = true;
}

void Arhivator::set_information(){
    information = true;
}

void Arhivator::set_check(){
    check = true;
}

void Arhivator::set_keep(){
    keep = true;
}

void Arhivator::set_decode(){
    decod = true;
}

void Arhivator::start(){
    if(stdinput){
        stdoutput = true;
    }
    if(information){
        throw MyException("Not Ready Yet");
    }
    if(recursive){
        throw MyException("Not Ready Yet");
    }
    if(keep){
        throw MyException("Not Ready Yet");
    }
    if(decod){
        decode_file(path);
    }else{
        encode_file(path);
    }
}

void Arhivator::encode_file(const string& path){
    cout << "encode: ";
    cout << path << endl;
    Compressor* cmp;
    string tmpname = tmpnam(nullptr);
    ifstream input_file(path, ios::in | ios::binary);
    
    // 1 step
    ofstream tmp1(tmpname + ".1", ios::out | ios::binary);
    cmp = new BWT();
    // 2 step
    // 3 step
    // 4 step
}

// will coding streams: 
// is - stream of input file(or stdin), os -steam of output file(or stdout)
void Arhivator::encode_stream(istream& is, ostream& os){
    cout << "encode stream" << endl;
    Compressor* cmp;
    string tmpnm = tmpnam(nullptr);
    string file1 = tmpnm + ".1";
    string file2 = tmpnm + ".2";
    string file3 = tmpnm + ".3";
    string file4 = tmpnm + ".4";
    ifstream tmp_in;
    ofstream tmp_out;
    uint64_t input_size = 0;
    uint64_t output_size = 0;
    uint32_t hash_c = 0;

    // ENCODING:

    // 1 step
    tmp_out.open(file1, ios::out | ios::binary);
    cmp = new BWT(is, tmp_out);
    cmp->encode();
    input_size = cmp->get_until_size();
    delete cmp;
    tmp_out.close(); // TODO exceptions

    // 2 step
    tmp_in.open(file1, ios::in | ios::binary);
    tmp_out.open(file2, ios::out | ios::binary);
    cmp = new MTF(tmp_in, tmp_out);
    cmp->encode();
    delete cmp;
    tmp_in.close();
    tmp_out.close();
    remove(file1.c_str());

    // 3 step
    tmp_in.open(file2, ios::in | ios::binary);
    tmp_out.open(file3, ios::out | ios::binary);
    cmp = new RLE(tmp_in, tmp_out);
    cmp->encode();
    delete cmp;
    tmp_in.close();
    tmp_out.close();
    remove(file2.c_str());

    // 4 step
    tmp_in.open(file3, ios::in | ios::binary);
    tmp_out.open(file4, ios::out | ios::binary);
    cmp = new Huffman(tmp_in, tmp_out);
    cmp->encode();
    // size of encoded input
    output_size = cmp->get_after_size();
    delete cmp;
    tmp_in.close();
    tmp_out.close();
    remove(file3.c_str());

    // gen and add info into output
    // 1 - add constant prefix for check gzip format
    os.write(prefix.c_str(), prefix.size());
    // 2 - add old size of file
    os.write(reinterpret_cast<const char*>(&input_size), sizeof(uint64_t));
    // 3 - count new size of file
    // encoded inpt + preix size + 2 sizes + hash of file
    output_size += 2 * sizeof(uint64_t) + prefix.size() + sizeof(uint32_t);
    os.write(reinterpret_cast<const char*>(&output_size), sizeof(uint64_t));
    // 4 - add hash
    tmp_in.open(file4, ios::in | ios::binary);
    hash_c = hash_count(tmp_in);
    // return to start pos
    tmp_in.seekg(0, ios::beg);
    os.write(reinterpret_cast<const char*>(&hash_c), sizeof(uint32_t));

    // 5 - insert encoed data at least of file
    from_stream_to_stream(tmp_in, os);
    // and remove old file
    tmp_in.close();
    remove(file4.c_str());
}

void Arhivator::decode_stream(istream& is, ostream& os){
    cout << "decode stream" << endl;
    Compressor* cmp;
    string tmpnm = tmpnam(nullptr);
    string file0 = tmpnm + ".0";
    string file1 = tmpnm + ".1";
    string file2 = tmpnm + ".2";
    string file3 = tmpnm + ".3";
    ifstream tmp_in;
    ofstream tmp_out;

    // CHECK ZIP FORMAT OF FILE

    // Writing all stream in tmp file
    tmp_out.open(file0, ios::out | ios::binary);
    from_stream_to_stream(is, tmp_out);
    tmp_out.close();

    // Check zip format and take pointer to start of encoded data 
    // after special simbols and numbers
    tmp_in.open(file0, ios::in | ios::binary);

    if(!check_zip(tmp_in)){
        throw MyException("not a zip formt");
    }

    tmp_in.seekg(2*sizeof(uint64_t) + prefix.size() + sizeof(uint32_t), ios::beg);


    // DECODING:
    
    // Decode all characters after specil simbols

    // 1 step
    tmp_out.open(file1, ios::out | ios::binary);
    cmp = new Huffman(tmp_in, tmp_out);
    cmp->decode();
    delete cmp;
    tmp_out.close(); // TODO exceptions
    tmp_in.close();
    remove(file0.c_str());

    // 2 step
    tmp_in.open(file1, ios::in | ios::binary);
    tmp_out.open(file2, ios::out | ios::binary);
    cmp = new RLE(tmp_in, tmp_out);
    cmp->decode();
    delete cmp;
    tmp_in.close();
    tmp_out.close();
    remove(file1.c_str());

    // 3 step
    tmp_in.open(file2, ios::in | ios::binary);
    tmp_out.open(file3, ios::out | ios::binary);
    cmp = new MTF(tmp_in, tmp_out);
    cmp->decode();
    delete cmp;
    tmp_in.close();
    tmp_out.close();
    remove(file2.c_str());

    // 4 step decoding in output
    tmp_in.open(file3, ios::in | ios::binary);
    cmp = new BWT(tmp_in, os);
    cmp->decode();
    delete cmp;
    tmp_in.close();
    remove(file3.c_str());
}

bool Arhivator::check_zip(istream& is){
    string buffer;
    buffer.resize(prefix.size());
    if(is){
        is.read(&buffer[0], prefix.size());
        if(buffer != prefix){
            return false;
        }
    }
    uint64_t placebo;
    if(is){
        is.read(reinterpret_cast<char*>(&placebo), sizeof(uint64_t));
    }
    if(is){
        is.read(reinterpret_cast<char*>(&placebo), sizeof(uint64_t));
    }
    uint32_t hash_f = 0;
    if(is){
        is.read(reinterpret_cast<char*>(&hash_f), sizeof(uint32_t));
    }
    uint32_t hash_c = hash_count(is);
    return hash_f == hash_c;
}

uint32_t Arhivator::hash_count(istream& is){
    string buffer;
    uint32_t ans = 0;
    buffer.resize(max_size_buffer);
    std::hash<string> func;
    while(is){
        is.read(&buffer[0], max_size_buffer);
        ans += func(buffer);
    }
    return ans;
}

void Arhivator::from_stream_to_stream(istream& is, ostream& os){
    string bufer;
    bufer.resize(max_size_buffer);
    while(is){
        is.read(&bufer[0], max_size_buffer);
        os.write(bufer.c_str(), is.gcount());
    }
}

