#include <iostream>

using namespace std;

long long min_of_three(long long a1, long long a2, long long a3){
  if(a1 < a2){
    if(a1 < a3){
      return a1;
    }else{
      return a3;
    }
  }else{
    if(a2 < a3){
      return a2;
    }else{
      return a3;
    }
  }
}

int main(){
  short unsigned n, m; // 4 байта
  cin >> n >> m;
  if(n > 1000 || n < 2 || m > 1000 || m < 2){
    cout << "ERROR\n";
    return 0;
  }
  long int* A = (long int*)malloc(sizeof(long int) * n * m); // n*m*4 байт
  long long int* B = (long long int*)malloc(sizeof(long long int) * n * m);
  short unsigned k = 0, j = 0, i = 0;
  /* Заполняем значениями матрицу */
  for(; i < n; ++i){
    for(j = 0; j < m; ++j){
      cin >> A[i*m + j];
    }
  }
  for(j = 0; j < m; ++j){
    B[(n-1)*m + j] = A[(n - 1)*m + j];
  }
  for(i = n - 2; ; --i){
    B[i*m] = min_of_three(B[(i+1)*m], B[(i+1)*m], B[(i+1)*m + 1]) + A[i*m];
    for(j = 1; j < m - 1; ++j){
      B[i*m + j] = min_of_three(B[(i+1)*m + j-1], B[(i+1)*m + j], B[(i+1)*m + j+1]) + A[i*m + j];
    }
    B[i*m + m - 1] = min_of_three(B[(i+1)*m + m-2], B[(i+1)*m + m-1], B[(i+1)*m + m-1]) + A[i*m + m-1];
    if(!i){
      break;
    }
  }
  for(j = 1; j < m; ++j){
    if(B[j] < B[k]){
      k = j;
    }
  }
  cout << B[k] << '\n' << "(1," << k + 1 << ')';
  for(i = 1; i < n; ++i){
    if(k && B[i*m + k-1] < B[i*m + k]){
      if(k < m - 1 && B[i*m + k-1] > B[i*m + k+1]){
        ++k;
      }else{
        --k;
      }
    }else{
      if(k < m - 1 && B[i*m + k+1] < B[i*m + k]){
        ++k;
      }
    }
    cout << ' ' << '(' << i+1 << ',' << k+1 << ')';
  }
  cout << endl;
  free(B);
  free(A);
  return 0;
}
