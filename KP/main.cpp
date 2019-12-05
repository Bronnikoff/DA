// Made By Max Bronnikov
#include <iostream>
#include "Arhivator/Arhivator.hpp"

using namespace std;

// Demo of using Arhivator class
// This Demo equal light version of gzip
int main(int argc, char const *argv[])
{
    /*if(argc < 2){
        cout << "gzip: compressed data not written to a terminal." << endl;
        cout << "For help, type: gzip -h" << endl;
        return 0;
    }*/
    bool worked = false;
    Arhivator arhive;
    try{
        for(int i = 1; i < argc; ++i){
            cout << argv[i] << endl;
            if(argv[i][0] == '-'){
                switch (argv[i][1])
                {
                case 'd':
                    cout << "decode" << endl;
                    arhive.set_decode();
                    break;
                case '1':
                    cout << "easy" << endl;
                    arhive.set_easy();
                    break;
                case '9':
                    cout << "hard" << endl;
                    arhive.set_easy();
                    break;       
                case 'l':
                    cout << "infrm" << endl;
                    arhive.set_information();
                    break;
                case 't':
                    cout << "check" << endl;
                    arhive.set_check();
                    break;          
                case 'c':
                    cout << "stdout" << endl;
                    arhive.set_stdoutput();
                    break;
                case 'k': 
                    cout << "keep" << endl;
                    arhive.set_keep();
                    break;
                case '\0': 
                    cout << "NO FILE" << endl;
                    worked = true;
                    arhive.start();
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
