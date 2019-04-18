#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFSIZE 17

typedef struct Flag{
  unsigned int lenght;
  unsigned int number;
}Flag;

typedef struct KANode{

  struct Massive{
    struct KANode** data;
    unsigned int size;
    unsigned int buzy;
  } dict;

  Flag* flag;
  char key[BUFSIZE];
  struct KANode* suffix;
  struct KANode* goodsuf;

}KANode;


typedef struct Massive Massive;

typedef struct Bor{
  KANode* head; //указатель на голову
  unsigned int height; //высота бора
}Bor;


void Make_KA(Bor* bor); // преобразуем бор в конечный автомат
bool Create_Bor(Bor* bor); // создаем каскад бора(а именно его голову, начальное состояние автомата)
short unsigned int Make_Bor(Bor* bor); // создаем бор с параллельным считыванием образцов
void Destroy_KA(Bor* bor); //уничтожаем бор(автомат)
void Error(short unsigned int i); //вывод ошибки
void Massive_Sort(Massive* mass); //сортировка массива
KANode* Massive_Search(Massive* mass, char buffer[BUFSIZE]); //бинарный поиск по массиву
short unsigned int Text_Search(Bor* bor); // функция работы по тексту
KANode* Get_Next(KANode* bor, char buffer[BUFSIZE]);  // получение следующего состояния автомата

KANode* Get_Next(KANode* node, char buffer[BUFSIZE]){
  KANode* it = Massive_Search(&node->dict, buffer);
  while(!it){
    if(node == node->suffix){
      return NULL;
    }else{
      node = node->suffix;
    }
    it = Massive_Search(&node->dict, buffer);
  }
  return it;
}

short unsigned int Text_Search(Bor* bor){
  KANode *il, *it = bor->head;
  char buffer[BUFSIZE];
  unsigned int i, num = 0, j = 1, h = 1;
  unsigned int size = bor->height - 1;
  Flag vertex[size]; //для хранения количества слов в считаной строке
  while(1){ // цикл, считывающий образец
    while(1){ // цикл, считывающий слово
      i = 0;
      while(1){
        buffer[0] = tolower(getchar());
        if(buffer[0]==EOF){ // если пустая строка => все образцы считаны
          return 0;
        }else if(buffer[0]==' ' || buffer[0]=='\n' || buffer[0]=='\t'){
          if(buffer[0]=='\n'){
            ++h;
            j = 1;
          }
          continue;
        }
        break;
      }
      while(buffer[i] >= 'a' && buffer[i] <= 'z'){ // считываем слово до разделителя
        buffer[++i] = tolower(getchar());
      }
      if(buffer[i]=='\n'){
        vertex[num].number = h;
        vertex[num].lenght = j;
        ++h;
        j = 1;
        buffer[i] = '\0'; //слов считано
        break;
      }else if(buffer[i]==' ' || buffer[i] == '\t'){
        vertex[num].number = h;
        vertex[num].lenght = j;
        buffer[i] = '\0';
        ++j;
        break;
      }else{
        return 2;
      }
    }
    it = Get_Next(it, buffer); // ищем следующее состояние автомата
    if(!it){
      num = 0;
      it = bor->head;
      continue;
    }
    if(it->flag){
      for (i = 1; i < it->flag[0].number; i++) {
        printf("%d,%d,%d\n", vertex[(size+num+1-it->flag[i].lenght)%size].number, vertex[(size+num+1-it->flag[i].lenght)%size].lenght, it->flag[i].number);
      }
    }
    il = it->goodsuf;
    while(il){
      for (i = 1; i < il->flag[0].number; i++) {
        printf("%d,%d,%d\n", vertex[(size+num+1-il->flag[i].lenght)%size].number, vertex[(size+num+1-il->flag[i].lenght)%size].lenght, il->flag[i].number);
      }
      il = il->goodsuf;
    }
    num = (num+1)%size;
  }
}

void Make_KA(Bor* bor){
  if(!bor->head){
    return;
  }
  unsigned int stack2[bor->height];
  KANode* stack1[bor->height];
  stack1[0]=bor->head;
  stack2[0]=0;
  unsigned int i = 0;
  for(i = 0; i < bor->head->dict.buzy; i++){
    bor->head->dict.data[i]->suffix = bor->head;
  }
  i = 0;
  KANode *n, *it, *la;
  bool down = 0;
  while(1){
    if(down && !stack1[i]->suffix){
      la = stack1[i];
      it = stack1[i-1]->suffix;
      while(it != bor->head){
        n = Massive_Search(&it->dict, la->key);
        it = it->suffix;
        if(n){
          la->suffix = n;
          la = n;
        }else{
          continue;
        }
        if(n->flag && !stack1[i]->goodsuf){
          stack1[i]->goodsuf = n;
        }
      }
      n = Massive_Search(&bor->head->dict, la->key);
      if(n){
        la->suffix = n;
        n->suffix = bor->head;
        if(n->flag && !stack1[i]->goodsuf){
          stack1[i]->goodsuf = n;
        }
      }else{
        la->suffix = bor->head;
      }
    }else if(down){
      it = stack1[i]->suffix;
      while(it != bor->head){
        if(it->flag){
          stack1[i]->goodsuf = it;
          break;
        }
        it = it->suffix;
      }
    }
    if(stack2[i] < stack1[i]->dict.buzy){
      stack1[i+1] = stack1[i]->dict.data[stack2[i]];
      ++stack2[i++];
      stack2[i]=0;
      down = 1;
    }else{
      if(!i){
        break;
      }
      --i;
      down = 0;
    }
  }
}

KANode* Massive_Insert(Massive* mass, char buffer[BUFSIZE]){
  KANode* it = Massive_Search(mass, buffer);
  if(it){
    return it;
  }
  if(mass->size){
    unsigned int i = mass->buzy;
    if(mass->size == i){
      mass->size<<=1;
      mass->data = realloc(mass->data, mass->size*sizeof(KANode*));
      if(!mass->data){
        return NULL;
      }
      mass->data[i] = malloc(sizeof(KANode));
      if(!mass->data[i]){
        return NULL;
      }
      ++mass->buzy;
      strcpy(mass->data[i]->key, buffer);
      mass->data[i]->suffix = NULL;
      mass->data[i]->goodsuf = NULL;
      mass->data[i]->flag = NULL;
      mass->data[i]->dict.size = mass->data[i]->dict.buzy = 0;
    }else{
      mass->data[i] = malloc(sizeof(KANode));
      if(!mass->data[i]){
        return NULL;
      }
      ++mass->buzy;
      strcpy(mass->data[i]->key, buffer);
      mass->data[i]->suffix = NULL;
      mass->data[i]->goodsuf = NULL;
      mass->data[i]->flag = NULL;
      mass->data[i]->dict.size = mass->data[i]->dict.buzy = 0;
    }
    it = mass->data[i];
    Massive_Sort(mass);
    return it;
  }else{
    mass->data = malloc(2 * sizeof(KANode*));
    if(!mass->data){
      return NULL;
    }
    mass->data[0] = malloc(sizeof(KANode));
    if(!mass->data[0]){
      return NULL;
    }
    mass->size = 2;
    mass->buzy = 1;
    strcpy(mass->data[0]->key, buffer);
    mass->data[0]->suffix = NULL;
    mass->data[0]->goodsuf = NULL;
    mass->data[0]->flag = NULL;
    mass->data[0]->dict.size = mass->data[0]->dict.buzy = 0;
    return mass->data[0];
  }
}

void Massive_Sort(Massive* A){
  KANode* key;
  unsigned int i;
  bool flag = 1;
  for(unsigned int j = 1; j < A->buzy; j++){
    key = A->data[j];
    i = j-1;
    for(; strcmp(A->data[i]->key, key->key) > 0; i--){
      A->data[i+1] = A->data[i];
      if(!i){
        A->data[0] = key;
        flag = 0;
        break;
      }
    }
    if(flag){
      A->data[i+1] = key;
      flag = 1;
    }
  }
  return;
}

KANode* Massive_Search(Massive* m, char buffer[BUFSIZE]){
  if(!m->buzy){
    return NULL;
  }
  unsigned int a = 0, b = m->buzy -1;
  unsigned int c = (a+b)/2;
  short int s = 0;
  while(a != b){
    s = strcmp(buffer, m->data[c]->key);
    if(s < 0){
      b = c;
      c = (a + b)/2;
      continue;
    }else if(s){
      a = c + 1;
      c = (a + b)/2;
      continue;
    }else{
      return m->data[c];
    }
  }
  if(strcmp(buffer, m->data[c]->key)){
    return NULL;
  }else{
    return m->data[c];
  }
}

void Error(short unsigned int i){
  switch (i) {
    case 1:
      printf("ERROR: Alloc error\n");
      break;
    case 2:
      printf("ERROR: Wrong format of data\n");
      break;
  }
  return;
}

bool Create_Bor(Bor* bor){
  bor->head = malloc(sizeof(KANode));
  if(!bor->head){
    return 1;
  }
  bor->head->dict.data = NULL;
  bor->head->flag = NULL;
  bor->head->dict.size = bor->head->dict.buzy = 0;
  bor->height = 1;
  bor->head->suffix = bor->head;
  bor->head->goodsuf = NULL;
  return 0;
}

void Destroy_KA(Bor* bor){
  if(!bor->head){
    return;
  }
  unsigned int stack2[bor->height];
  KANode* stack1[bor->height];
  stack1[0]=bor->head;
  stack2[0]=0;
  unsigned int i = 0;
  while(1){
    while(stack2[i] < stack1[i]->dict.buzy){
      stack1[i+1] = stack1[i]->dict.data[stack2[i]];
      stack2[++i] = 0;
    }
    if(stack1[i]->flag){
      free(stack1[i]->flag);
    }
    if(stack1[i]->dict.buzy){
      free(stack1[i]->dict.data);
    }
    // printf("%s\n", stack1[i]->key);
    free(stack1[i]);
    if(!i){
      break;
    }
    --i;
    ++stack2[i];
  }
  ///////////
  bor->head = NULL;
  bor->height = 0;
  return;
}

short unsigned int Make_Bor(Bor* bor) {
  KANode* it = bor->head;
  //KANode* su = NULL;
  char c = ' ';
  bool flag = 0; // флаг сообщает о том, что образец завершается здесь
  char buffer[BUFSIZE];
  memset(buffer, 0, BUFSIZE);
  unsigned int h, i, j = 1;
  //здесь пишем считывание образцов с моментальным дбавлением в бор
  while(1){ // цикл, считывающий образец
    it = bor->head;
    h = 1;
    while(1){ // цикл, считывающий слово с добавлением в бор
      while(c==' ' || c=='\t'){
        c = tolower(getchar());
      }
      if(c == '\n'){
        return 0;
      }
      buffer[0]=c;
      i = 1;
      buffer[1]=tolower(getchar());
      while(buffer[i] >= 'a' && buffer[i] <='z'){ //считываем слово до разделителя
        buffer[++i] = tolower(getchar());
      }
      c = buffer[i];
      flag = 0;
      while(c == ' ' || c == '\t'){
        c = getchar();
      }
      if(c == '\n'){
        c = ' ';
        flag = 1;
      }else{
        c = tolower(c);
        if(c < 'a' || c > 'z'){
          Destroy_KA(bor);
          return 2;
        }
      }
      buffer[i]='\0';
      /*  здесь начинаем блок добавления слова в бор  */
      it = Massive_Insert(&it->dict, buffer);
      if(!it){
        Destroy_KA(bor);
        return 1;
      }
      if(flag){
        if(it->flag){
          i = it->flag[0].number;
          if(i%2){
            it->flag[i].number = j;
            it->flag[0].lenght = h;
            ++it->flag[0].number;
          }else{
            it->flag = realloc(it->flag, (i + 2) * sizeof(Flag));
            it->flag[i].number = j;
            it->flag[i].lenght = h;
            ++it->flag[0].number;
          }
        }else{
          it->flag = malloc(2 * sizeof(Flag));
          it->flag[0].number = 2;
          it->flag[1].lenght = h;
          it->flag[1].number = j;
        }
        ++h;
        break;
      }
      ++h;
    }
    if(bor->height < h){
      bor->height = h;
    }
    ++j;
  }
}

int main(int argc, char const *argv[]) {
  short unsigned int i;
  Bor bor;
  Create_Bor(&bor);
  i = Make_Bor(&bor);
  if(i){
    Error(i);
    return 1;
  }
  Make_KA(&bor);
  i = Text_Search(&bor);
  if(i){
    Error(i);
    Destroy_KA(&bor);
    return 1;
  }
  Destroy_KA(&bor);
  return 0;
}
