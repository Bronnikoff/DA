// Made By Max Bronnikov
#include <iostream>
#include <vector>
#include <string>
#include "Arhivator/Arhivator.hpp"

using namespace std;

// Demo of using Arhivator class

// This Demo light version of gzip
int main(int argc, char const *argv[])
{
    if(argc < 2){
        cout << "gzip: compressed data not written to a terminal." << endl;
        cout << "For help, type: gzip -h" << endl;
        return 0;
    }
    vector<string> vector_of_path;
    Arhivator arhive;

    for(int i = 1; i < argc; ++i){
        if(argv[i][0] == '-'){
            switch (argv[i][1]){
                case 'd':
                    arhive.set_decode();
                    break;
                case '1':
                    arhive.set_easy();
                    break;
                case '9':
                    arhive.set_hard();
                    break;
                case 'l':
                    arhive.set_information();
                    break;
                case 't':
                    arhive.set_check();
                    break;
                case 'r':
                    arhive.set_recursive();
                    break;
                case 'k':
                    arhive.set_keep();
                    break;
                case 'c':
                    arhive.set_stdoutput();
                    break;
                case '\0': 
                    vector_of_path.push_back(string());
                    break;   
                default:
                    break;
            }
        }else{
            vector_of_path.push_back(argv[i]);
        }
    }
    if(vector_of_path.empty()){
        vector_of_path.push_back(string());
    }

    try{
        for(unsigned i = 0; i < vector_of_path.size(); ++i){
            arhive.set_path(vector_of_path[i]);
            arhive.start();
        }
    }catch(std::exception &exeption){
        cerr << "gzip: " << exeption.what() << endl;
        return 0; 
    }

    return 0;
}
