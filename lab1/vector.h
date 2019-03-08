#ifndef VECTOR_H
#define VECTOR_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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

#endif
