#include <iostream>
#include <ctime>
#include <fstream>
#include <climits>

#define N 1000
#define M 1000

int main(){
  std::ofstream file;
  file.open("./file");
  file << N << ' ' << M << std::endl;
  srand(time(0));
  int a;
  for(int i = 0; i < N; ++i){
    for(int j = 0; j < M; ++j){
      a = rand()%2;
      if(a){
        file << '-';
      }
      file << rand()%9 + 1;
      for(int k = 0; k < 8; ++k){
        file << rand()%10;
      }
      file << ' ';
    }
    file << std::endl;
  }
}
