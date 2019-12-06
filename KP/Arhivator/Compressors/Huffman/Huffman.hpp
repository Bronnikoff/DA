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
            struct Node* left;
            struct Node* right;
            uint32_t count;
            char value; // has value only if list, else it rubish
        }node;
        // comparator for priority queue
        typedef struct functor{
            bool operator()(const node* lhs, const node* rhs){
                return lhs->count > rhs->count;
            }
        }comparator;

        // TREE (too lazy create new structure):
        node* tree = nullptr; // root
        vector<node> vertex; // vector of vertexes of tree
        // AND OF TREE
        
        void tree_create(); // create tree of encoding
        void tree_from_input(uint32_t& i); // create tree from input for decoding
        void tree_write(node* obj); // write tree in output
        void count_table(map<char, uint32_t>& table_of_counts); // count simbols
        void create_encode_table(map<char, vector<uint8_t>>& encode_table);
        void depth_walker(node* obj ,vector<uint8_t>& vec, map<char, vector<uint8_t>>& mp);  
};


#endif