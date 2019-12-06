// Made By Max Bronnikov
#include <iostream>
#include "Arhivator/Arhivator.hpp"

using namespace std;

// Demo of using Arhivator class
// This Demo equal light version of gzip
int main(int argc, char const *argv[])
{
    if(argc < 2){
        cout << "gzip: compressed data not written to a terminal." << endl;
        cout << "For help, type: gzip -h" << endl;
        return 0;
    }
    bool worked = false;
    Arhivator arhive;
    try{
        for(int i = 1; i < argc; ++i){
            cout << argv[i] << endl;
            if(argv[i][0] == '-'){
                switch (argv[i][1])
                {
                case 'd':

                case '1':

                case '9':
     
                case 'l':

                case 't':
        
                case 'c':

                case 'k': 
                    
                case '\0': 

                default:
                    break;
                }
            }else{
                arhive.set_path(string(argv[i]));
                worked = true;
                arhive.start();
            }
        }
        if(!worked){
            arhive.start();
        }
    }catch(std::exception &exeption){
        cerr << "gzip: " << exeption.what() << endl;
        return 0;
    }

    return 0;
}
