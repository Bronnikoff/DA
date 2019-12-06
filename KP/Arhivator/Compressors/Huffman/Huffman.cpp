#include "Huffman.hpp"

using namespace std;


Huffman::Huffman() : Compressor(){}

Huffman::Huffman(istream& inpt, ostream& otpt) : Compressor(inpt, otpt){}

Huffman::Huffman(istream* inpt, ostream* otpt) : Compressor(inpt, otpt){}


void Huffman::buffer_decode(){
    tree = nullptr;
    vertex.clear();
    uint32_t i = 0;
    // get tree from input
    tree_from_input(i);

    uint32_t size = 0;

    // Reading size of decoded buffer
    for(uint8_t j = 0; j < 4; ++j){
        // read 1 byte
        uint8_t byte = static_cast<uint8_t>(in_buffer[i++]);
        size |= static_cast<uint32_t>(byte) << (8*(3 - j)); // insert bits on needed positions
    }

    // DECODE:
    // counter of simbols needs for stop decode
    uint32_t j = 0; 
    Huffman::node* it = tree;
    for(; i < in_buffer.size() && j < size; ++i){
        // reading byte of information:
        uint8_t bits_of_code = in_buffer[i];
        // create mask for reading bits
        uint8_t mask = 1 << 7; // 10000000
        // reading bits
        for(uint8_t k = 0; k < 8; ++k){
            // if bit - 1 => go right
            if(bits_of_code & mask){
                it = it->right;
            }else{
            // if bit - 0 => go left
                it = it->left;
            }
            // if in list => push decoded simbol in output
            if(!it->left){
                out_buffer.push_back(it->value);
                // and go back in root
                it = tree;
                // apdate counter of simbols
                ++j;
            }
            // update mask for read next bit
            mask >>= 1;
        }
    }
}


void Huffman::buffer_encode(){
    tree = nullptr;
    vertex.clear();
    // At first create tree for encode
    tree_create();
    // At second write tree in file
    tree_write(tree);
    // it simbol for end of tree 
    // cant do without this but too lazy
    out_buffer.push_back('2'); // simbol of tree end
    // Create table of simbols codes:
    map<char, vector<uint8_t>> encode_table;
    create_encode_table(encode_table);

    // Save size of buffer:

    uint32_t size = in_buffer.size();
    uint32_t s_mask = 1 << 8; --s_mask;

    uint8_t byte4 = static_cast<uint8_t>(size & s_mask);
    size >>= 4;
    uint8_t byte3 = static_cast<uint8_t>(size & s_mask);
    size >>= 4;
    uint8_t byte2 = static_cast<uint8_t>(size & s_mask);
    size >>= 4;
    uint8_t byte1 = static_cast<uint8_t>(size & s_mask);


    out_buffer.push_back(static_cast<char>(byte1));
    out_buffer.push_back(static_cast<char>(byte2));
    out_buffer.push_back(static_cast<char>(byte3));
    out_buffer.push_back(static_cast<char>(byte4));


    // ENCODING:

    // byte for put in output
    uint8_t bits_of_code = 0;
    uint8_t mask = 1 << 7; // 10000000
    // reaing simbols for encode
    for(uint32_t i = 0; i < in_buffer.size(); ++i){
        // get code of input simbol
        vector<uint8_t>& simbol_code = encode_table[in_buffer[i]];
        // insert code in byte 
        for(uint8_t k = 0; k < simbol_code.size(); ++k){
            // if bit code - 1 => put 1 on needed position(else nothing to do)
            if(simbol_code[k]){
                bits_of_code |= mask;
            }
            // get new position for bit
            mask >>= 1;
            // if mask - 00000000 => put encoded byte in output and go to encode new byte
            if(!mask){
                out_buffer.push_back(static_cast<char>(bits_of_code));
                mask = 1 << 7;
                bits_of_code = 0;
            }
        }
    }

}


// this function needs for walking in tree and construct code table of simbols  
void Huffman::depth_walker(Huffman::node* obj ,vector<uint8_t>& vec, map<char, vector<uint8_t>>& mp){
    // Wlking in tree and collect way in vector
    // And after that save way for all alfabet simbols 
    if(obj->left){
        vec.push_back(0);
        depth_walker(obj->left, vec, mp);
        vec.pop_back();
        vec.push_back(1);
        depth_walker(obj->right, vec, mp);
        vec.pop_back();
    }else{
        mp[obj->value] = vec;
    }
}


void Huffman::create_encode_table(map<char, vector<uint8_t>>& encode_table){
    // DEPTH WALK FOR CREATE ENCODE TABLE
    vector<uint8_t> vec;
    depth_walker(tree, vec, encode_table);
}


void Huffman::tree_write(Huffman::node* obj){
    // Main idea: when we go left put 0
    // when we come to node put value of node
    // when go right put 1 
    // P.S. something like this i made in lab2

    // value
    out_buffer.push_back(obj->value);
    if(obj->left){
        out_buffer.push_back('0');
        tree_write(obj->left);
    }else{
        // if left - null => right - null in our tree
        return;
    }
    out_buffer.push_back('1');
    tree_write(obj->right);
}

void Huffman::tree_from_input(uint32_t& i){
    // it very bed code for unerstning but i will try to explain:
    // We construct tree by path simbols: 0 - go left, 1 - go right, 2 - end of building
    // this simbols - instruction to go in depth walking

    // in structure of tree we have not useful for us attribute: count
    // we will save in it index of vertex for up move in depth walking
    // (it not good idea may be, but working)

    // At first create root:
    vertex.push_back(Huffman::node{nullptr, nullptr, 0, in_buffer[i++]});
    tree = &vertex.back();
    Huffman::node* it = tree;

    // depth walking:
    while(1){
        // if go left: 
        // 1) create new node
        // 2) save in new node index of father in vertex vector
        // 3) init left and go
        if(in_buffer[i] == '0'){
            ++i;
            vertex.push_back(Huffman::node{nullptr, nullptr, static_cast<uint32_t>(vertex.size()-1), in_buffer[i++]});
            // create left
            it->left = &vertex.back();
            // go left
            it = it->left;
            continue;
        }
        // if go right:
        // 1) move up to vertex for create right child for go right 
        // 2) create new Huffman::node
        // 3) set here upper travel as upper travel for her father
        // 4) init right and go 
        if(in_buffer[i] == '1'){
            ++i;
            // move up
            it = &vertex[it->count];
            // here set father travel
            vertex.push_back(Huffman::node{nullptr, nullptr, it->count, in_buffer[i++]});
            // create node 
            it->right = &vertex.back();
            // go right
            it = it->right;
        }
        // if read 2 => end of walking
        if(in_buffer[i] == '2'){
            ++i;
            return;
        }
    }
}

void Huffman::tree_create(){
    // table of counts simbol repeats in buffer
    map<char, uint32_t> table_of_counts;
    count_table(table_of_counts);

    // priority queue for Huffman algo(may be not too fast as custom, but create custom lazy)
    priority_queue<Huffman::node*, vector<Huffman::node*>, comparator> que;

    // create and insert basic nodes(lists) in queue
    for(auto it : table_of_counts){
        vertex.push_back(Huffman::node{nullptr, nullptr, it.second, it.first});
        que.push(&vertex.back());
    }

    // situation when only 1 simbol in alfabet: will encode all simbols as 0
    // it exception from rule:)
    if(que.size() == 1){
        vertex.push_back(Huffman::node{nullptr, nullptr, vertex[0].count, static_cast<char>(vertex[0].value+1)});
        vertex.push_back(Huffman::node{&vertex[0], &vertex[1], vertex[0].count, vertex[0].value});
        tree = &vertex[2];
        return;
    }

    // build tree from queue: itteration while not 1 elem in queue
    while(que.size() > 1){
        // take 2 nodes with less counts
        Huffman::node* left = que.top(); que.pop();
        Huffman::node* right = que.top(); que.pop();
        // create new node with left and right childs
        vertex.push_back(Huffman::node{left, right, left->count + right->count, left->value});
        // push new node to queue
        que.push(&vertex.back());
    }
    
    tree = que.top();
    que.pop(); // clear queue(not need)
}


// count table of repeats in buffer
void Huffman::count_table(map<char, uint32_t>& table_of_counts){
    for(uint32_t i = 0; i < in_buffer.size(); ++i){
        ++table_of_counts[in_buffer[i]];
    }
}