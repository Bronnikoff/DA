#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTVALUE 2
#define SUPR 1000000

typedef unsigned long long int TypeV;

typedef struct Item{
  TypeV value;
  int key;
} Item;

typedef struct vector{
  unsigned int len;
  unsigned int buzy;
  Item* data;
} Vector;

void Count_Sort(Vector* A);
void Create_Vector(Vector* A);
void Delete_Vector(Vector* A);
void Insert_Vector(Vector* A, Item* item);
void Read_Item(Vector* A);
void Vector_Print(Vector* A);


void Count_Sort(Vector* A){
  if(A->buzy){
    unsigned j = 0/*, C[SUPR]*/;
    unsigned* C = (unsigned*)malloc(sizeof(unsigned) * SUPR);
    if(!C){
      fprintf(stderr, "Allocate Second Vector Memory Error!\n");
      exit(EXIT_SUCCESS);
    }
    memset(C, 0, SUPR*sizeof(unsigned));
  // for(j = 0; j < SUPR; j++){
  //   C[j]=0;
  // }
    for(j = 0; j < A->buzy; j++){
      ++C[A->data[j].key];
    }
    for(j = 1; j < SUPR; j++){
      C[j]+=C[j-1];
    }
    Item* B = (Item*) malloc(sizeof(Item) * A->buzy);
    if(!B){
      fprintf(stderr, "Allocate Second Vector Memory Error!\n");
      exit(EXIT_SUCCESS);
    }
    for(j=A->buzy-1; j>0; j--){
      --C[A->data[j].key];
      B[C[A->data[j].key]].value = A->data[j].value; //Use of uninitialised value of size 8
      B[C[A->data[j].key]].key = A->data[j].key; // in ~half of itterations
    //memcpy(&B[--C[A->data[j].key]], &A->data[j], sizeof(Item));
    }
    --C[A->data[0].key];
    B[C[A->data[0].key]].value = A->data[0].value; //Use of uninitialised value of size 8
    B[C[A->data[0].key]].key = A->data[0].key;
    free(A->data);
    free(C);
    A->data = B;
  }
}

void Create_Vector(Vector* A){
  A->len = STARTVALUE;
  A->buzy = 0;
  A->data = (Item*) malloc(STARTVALUE * sizeof(Item));
  if(!A->data){
    fprintf(stderr, "Allocate Vector Memory Error!\n");
    exit(EXIT_SUCCESS);
  }
}

void Delete_Vector(Vector* A){
  free(A->data);
  A->data = NULL;
}

void Resize_Vector(Vector* A){
  A->len *= STARTVALUE;
  A->data = (Item*) realloc(A->data, A->len * sizeof(Item));
  if(!A->data){
    fprintf(stderr, "Reallocate Vector Memory Error!\n");
    exit(EXIT_SUCCESS);
  }
}

void Insert_Vector(Vector* A, Item* item){
  if(A->len == A->buzy){
    Resize_Vector(A);
  }
  A->data[A->buzy].key = item->key;
  A->data[A->buzy++].value = item->value;
}

void Read_Item(Vector* A){
  char buf[6];
  Item item;
  while(scanf("%s\t%llu", buf, &item.value) != EOF){
    item.key = (int) atoi(buf);
    Insert_Vector(A, &item);
  }
}

void Vector_Print(Vector* A){
  for(unsigned i = 0; i < A->buzy; i++){
    for(unsigned j = 100000; j != 1; j/=10){
      if(!(A->data[i].key / j)){
        printf("0");
      }else{
        break;
      }
    }
    printf("%d\t%llu\n", A->data[i].key, A->data[i].value);
  }
}

int main(){
    Vector vec;
    Create_Vector(&vec);
    Read_Item(&vec);
    Count_Sort(&vec);
    Vector_Print(&vec);
    Delete_Vector(&vec);
    return 0;
}
