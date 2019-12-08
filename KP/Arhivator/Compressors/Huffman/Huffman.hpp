#ifndef HUFFMAN_H
#define HUFFMAN_H


#include <iostream>
#include <inttypes.h>
#include <map>
#include <queue>


#include "../Compressor.hpp"

using namespace std;

////////////////////////////////////////////////
///////////// HUFFMAN ENCODE READY /////////////
////////////////////////////////////////////////




class Huffman : public Compressor{
    public:
        Huffman();
        Huffman(istream& inpt, ostream& otpt);
        Huffman(istream* inpt, ostream* otpt);
    protected:
        void buffer_encode() override;
        void buffer_decode() override;
    private:
        typedef struct Node{
            int32_t left;
            int32_t right;
            uint32_t count;
            char value; // has value only if list, else it rubish
        }node;
    
        // comparator for priority queue
        typedef struct functor{
            bool operator()(const pair<int32_t, uint32_t> lhs, const pair<int32_t, uint32_t> rhs){
                return lhs.second > rhs.second;
            }
        }comparator;

        // TREE (too lazy create new structure):
        int32_t tree = -1; // root
        vector<node> vertex; // vector of vertexes of tree
        // AND OF TREE
        
        void tree_create(); // create tree of encoding
        void tree_from_input(uint32_t& i); // create tree from input for decoding
        void tree_write(int32_t obj); // write tree in output
        void count_table(map<char, uint32_t>& table_of_counts); // count simbols
        void create_encode_table(map<char, vector<uint8_t>>& encode_table);
        void depth_walker(int32_t obj ,vector<uint8_t>& vec, map<char, vector<uint8_t>>& mp);  
};


#endif