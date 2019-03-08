#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void GenerateTest(int lines, char* name);

void GenerateTest(int lines, char* name){
  FILE* f = fopen(name, "w");
  srand(time(NULL));
  int key;
  for(int i = 0; i < lines; i++){
    key = rand()%1000000;
    for(int j = 100000; j != 0; j/=10){
      if(!(key/j)){
        fprintf(f, "0");
      } else{
        break;
      }
    }
    fprintf(f, "%d\t%u\n", key, rand());
  }
  fclose(f);
}

int main() {
  int a[23];
  printf("%ld\n", sizeof(bool));
  char name[20];
  int lines;
  printf("Enter filename of test: ");
  scanf("%s", name);
  printf("Enter number of lines: ");
  scanf("%d", &lines);
  GenerateTest(lines, name);
  return 0;
}
