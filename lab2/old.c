#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define BUFSIZE 257 // 256 + '\0'

typedef unsigned long long int TVal;

typedef struct BRTree{
  TVal value;
  char* key; // зaранение делаем в нижнем регистре
  bool color; //0-красный, 1-черный
  struct BRTree* right; //листы дерева - NULL
  struct BRTree* left; //считаем по дефолту черными
}BRTree;

typedef struct Node{
  BRTree* head;
  unsigned int size;
}Node;

BRTree* Search_Tree(Node* t, char* k);
void Create_Tree(Node* t); //врядли будем юзать
void Error(short unsigned int e); //вывод ошибок после анализа
short unsigned int Tree_Insert(Node* t, char* k, short unsigned int si, TVal val);//si - размер буффера
short unsigned int Tree_Delete(Node* t, char* k);
short unsigned int Load_Tree(Node* t, char* path);
short unsigned int Save_Tree(Node* t, char* path);
void Destroy_Tree(Node* t);
BRTree* Left_Rotate(BRTree *h);
BRTree* Right_Rotate(BRTree *h);
///////////////////////////////////////////////////////////////////////////////
void Destroy_Tree(Node* t){
  if(!t->size){
    t->head = NULL;
    return;
  }
  unsigned int S = t->size + 1;
  short unsigned int i;
  for(i = 0; S > 1; i++){
    S >>= 1;
  }
  i+=i;
  BRTree* h = t->head;
  BRTree* stack[i];
  i = 0;
  stack[i++]=h;
  while(1){
      //stack[i++] = h;
      if(h->left){
        h = h->left;
        stack[i++] = h;
        continue;
      }else if(h->right){
        h = h->right;
        stack[i++] = h;
        continue;
      }else{
        // printf("h->key: %s\n", h->key);
        //free(h);
        if(i > 1){
          i-=2;
        //  h = stack[i];
          if(stack[i]->right == h){
            free(h);
            h = stack[i++];
            h->right = NULL;
          }else{
            free(h);
            h = stack[i++];
            h->left = NULL;
          }
        }else{
          free(stack[0]);
          t->size = 0;
          t->head = NULL;
          return;
        }
        continue;
      }
  }
}
////////////////////////////////////////////////////////////////////////
BRTree* Left_Rotate(BRTree *a){
  BRTree* b = a->right;
  a->right = b->left;
  b->left = a;
  a->color = 0;
  b->color = 1;
  return b;
}

BRTree* Right_Rotate(BRTree *a){
  BRTree* b = a->left;
  a->left = b->right;
  b->right = a;
  a->color = 0;
  b->color = 1;
  return b;
}
// написать удаление

/* BRTree* Search_Tree(BRTree* t, char* k){
  if(!t){
    return NULL;
  }
  short i = strcmp(k, t->key);
  if(!i){
    return t;
  }else if(i > 0){
    return Search_Tree(t->right, k);
  }else{
    return Search_Tree(t->left, k);
  }
}
*/

BRTree* Search_Tree(Node* t, char* k){
  if(!t->size){
    return NULL;
  }
  BRTree* n = t->head;
  while(n){
    short i = strcmp(k, n->key);
    if(i < 0){
      n = n->left;
    }else if(i){
      n = n->right;
    }else{
      return n;
    }
  }
  return NULL;
}

void Create_Tree(Node* t){
  t->size = 0;
  t->head = NULL;
}

void Error(short unsigned int e){
  switch(e){
    case 1:
      printf("ERROR: Wrong command or format\n");
      break;
    case 2:
      printf("ERROR: Allocate error\n");
      break;
    case 3:
      printf("ERROR: Fileopen error\n");
      break;
    case 4:
      printf("ERROR: Wrong format of filedata\n");
      break;
  }
  return;
}


short unsigned int Tree_Insert(Node* t, char* k, short unsigned int si, TVal val){
  if(!t->size){ // если дерево пусто, то создаем голову
    t->head = malloc(sizeof(BRTree) + si);
    if(!t->head){
      return 2;
    }
    t->head->key = (char *)t->head + sizeof(BRTree);
    memcpy(t->head->key, k, si);
    t->head->value = val;
    t->head->left = t->head->right = NULL;
    t->head->color = 1;
    ++t->size;
    return 0;
  }

  BRTree* my = t->head; //если дерево не пусто
  BRTree *father, *grandpa, *ggf;
  ggf = grandpa = father = NULL;
  short int i = 0;
  bool gv = 0;
  bool fv = 0;
  bool ggv = 0;
  while(1){
    if(my->left && my->right && !my->left->color && !my->right->color){
      my->left->color = my->right->color = 1;
      my->color = 0;
    }
    if(!my->color && father && !father->color){
      if(ggf){ //если папа красный, значит дед существут и он черный
        if(ggv){
          if(gv){
            if(!fv){
              grandpa->right = Right_Rotate(father);
            }
            ggf->right = Left_Rotate(grandpa);
          }else{
            if(fv){
              grandpa->left = Left_Rotate(father);
            }
            ggf->right = Right_Rotate(grandpa);
          }
        }else{
          if(gv){
            if(!fv){
              grandpa->right = Right_Rotate(father);
            }
            ggf->left = Left_Rotate(grandpa);
          }else{
            if(fv){
              grandpa->left = Left_Rotate(father);
            }
            ggf->left = Right_Rotate(grandpa);
          }
        }
      }else{
        if(gv){
          if(!fv){
            grandpa->right = Right_Rotate(father);
          }
          t->head = Left_Rotate(grandpa);
        }else{
          if(fv){
            grandpa->left = Left_Rotate(father);
          }
          t->head = Right_Rotate(grandpa);
        }
      }
    }
    if(i){
      t->head->color = 1;
      return 0;
    }
    ggf = grandpa;
    ggv = gv;
    grandpa = father;
    gv = fv;
    father = my;
    i = strcmp(k, my->key);
    if(i > 0){
      my = my->right;
      fv = 1;
      if(!my){
        father->right = my = malloc(sizeof(BRTree) + si);
        if(!my){
          return 2;
        }
        my->key = (char *)my + sizeof(BRTree);
        memcpy(my->key, k, si);
        my->value = val;
        my->left = my->right = NULL;
        my->color = 0;
        ++t->size;
        continue;
      }
    }else if(i){ //i < 0
      my = my->left;
      fv = 0;
      if(!my){
        father->left = my = malloc(sizeof(BRTree) + si);
        if(!my){
          return 2;
        }
        my->key = (char *)my + sizeof(BRTree);
        memcpy(my->key, k, si);
        my->value = val;
        my->left = my->right = NULL;
        my->color = 0;
        ++t->size;
        continue;
      }
    }else{
      t->head->color = 1;
      return 1;
    }
    i = 0;
  }
}

short unsigned int Tree_Delete(Node* t, char* k){
  if(!t->size){ // если дерево пусто, то создаем голову
    return 1;
  }
  // creating static stack on stack(haha), because h of tree so small
  int s = t->size + 1;
  short unsigned int i, j = 0;
  for(i = 0; s > 1; i++){
    s>>=1;
  }
  BRTree* h = NULL;
  BRTree* stack[i+i+1];
  stack[0] = t->head;
  i = 0;
  // search a place
  while(stack[i]){
    s = strcmp(k, stack[i]->key);
    if(s < 0){
      stack[i+1] = stack[i]->left;
      ++i;
    }else if(s){
      stack[i+1] = stack[i]->right;
      ++i;
    }else{
      j = i;
      h = stack[j];
      stack[i+1] = stack[i]->left;
      ++i;
      break;
    }
  }
  while(stack[i]){
    stack[i+1] = stack[i]->right;
    ++i;
  }
  if(s){
    return 1;
  }
  bool col = stack[--i]->color;
  if(i != j){
    if(j && stack[j-1]->right == stack[j]){
      stack[j] = stack[j-1]->right = stack[i];
    }else if(j){
      stack[j] = stack[j-1]->left = stack[i];
    }else{
      stack[0] = t->head = stack[i];
    }
    stack[j]->color = h->color;
    if(i == j+1){
      stack[i] = stack[j]->left = stack[i]->left;
      stack[j]->right = h->right;
    }else{
      stack[i] = stack[i-1]->right = stack[i]->left;
      stack[j]->right = h->right;
      stack[j]->left = h->left;
    }
    free(h);
    --t->size;
  }else{
    h = stack[i];
    if(i && stack[i-1]->right == h){
      stack[i] = stack[i-1]->right = h->right;
    }else if(i){
      stack[i] = stack[i-1]->left = h->right;
    }else{
      stack[0] = t->head = h->right;
    }
    free(h);
    --t->size;
  }
  if(!t->size){
    t->head = NULL;
    return 0;
  }
  //ballance:
  if(!col){
    return 0;
  }
  if(stack[i] && !stack[i]->color){
    stack[i]->color = 1;
    return 0;
  }
  while(i){
    if(stack[i-1]->left == stack[i]){
      if(!stack[i-1]->right->color){ //1-st
        h = stack[i-1];
        if(i-1 && stack[i-2]->left == stack[i-1]){
          stack[i-1] = stack[i-2]->left = Left_Rotate(stack[i-1]);
        }else if(i-1){
          stack[i-1] = stack[i-2]->right = Left_Rotate(stack[i-1]);
        }else{
          stack[0] = t->head = Left_Rotate(stack[0]);
        }
        stack[i] = h;
        stack[++i] = h->left;
      }
      if(stack[i-1]->right->color){// 2-nd
        if((!stack[i-1]->right->right || stack[i-1]->right->right->color) && (!stack[i-1]->right->left || stack[i-1]->right->left->color)){//2.1
          stack[--i]->right->color = 0;
          if(stack[i]->color){
            continue;
          }else{
            stack[i]->color = 1;
            return 0;
          }
        }else if(stack[i-1]->right->left && !stack[i-1]->right->left->color && (!stack[i-1]->right->right || stack[i-1]->right->right->color)){//2.2
          stack[i-1]->right = Right_Rotate(stack[i-1]->right);
        }
        if(stack[i-1]->right->right && !stack[i-1]->right->right->color){
          stack[i-1]->right->right->color = 1;
          col = stack[i-1]->color;
          if(i-1 && stack[i-2]->right == stack[i-1]){
            stack[i-2]->right = Left_Rotate(stack[i-1]);
            stack[i-2]->right->color = col;
            stack[i-1]->color = 1;
            return 0;
          }else if(i-1){
            stack[i-2]->left = Left_Rotate(stack[i-1]);
            stack[i-2]->left->color = col;
            stack[i-1]->color = 1;
            return 0;
          }else{
            t->head = Left_Rotate(stack[i-1]);
            stack[0]->color = 1;
            return 0;
          }
        }
      }
    }else{
      if(!stack[i-1]->left->color){ //1-st
        h = stack[i-1];
        if(i-1 && stack[i-2]->left == stack[i-1]){
          stack[i-1] = stack[i-2]->left = Right_Rotate(stack[i-1]);
        }else if(i-1){
          stack[i-1] = stack[i-2]->right = Right_Rotate(stack[i-1]);
        }else{
          stack[0] = t->head = Right_Rotate(stack[0]);
        }
        stack[i] = h;
        stack[++i] = h->right;
      }
      if(stack[i-1]->left->color){// 2-nd
        if((!stack[i-1]->left->right || stack[i-1]->left->right->color) && (!stack[i-1]->left->left || stack[i-1]->left->left->color)){//2.1
          stack[--i]->left->color = 0;
          if(stack[i]->color){
            continue;
          }else{
            stack[i]->color = 1;
            return 0;
          }
        }else if(stack[i-1]->left->right && !stack[i-1]->left->right->color && (!stack[i-1]->left->left || stack[i-1]->left->left->color)){//2.2
          stack[i-1]->left = Left_Rotate(stack[i-1]->left);
        }
        if(stack[i-1]->left->left && !stack[i-1]->left->left->color){
          stack[i-1]->left->left->color = 1;
          col = stack[i-1]->color;
          if(i-1 && stack[i-2]->right == stack[i-1]){
            stack[i-2]->right = Right_Rotate(stack[i-1]);
            stack[i-2]->right->color = col;
            stack[i-1]->color = 1;
            return 0;
          }else if(i-1){
            stack[i-2]->left = Right_Rotate(stack[i-1]);
            stack[i-2]->left->color = col;
            stack[i-1]->color = 1;
            return 0;
          }else{
            t->head = Right_Rotate(stack[i-1]);
            stack[0]->color = 1;
            return 0;
          }
        }
      }
    }
  }
  //end ballance
  return 0;
}


short unsigned int Save_Tree(Node* t, char* path){
//нужен стэк на массиве(структуру создавать не буду)
FILE* f = fopen(path, "wb");
if(!f){
  return 3;
}
unsigned int S = t->size + 1;
short unsigned int i;
for(i = 0; S > 1; i++){
  S>>=1;
}
i<<=1;
fprintf(f, "%hu", i);
if(t->size){
  fprintf(f, "h");
}else{
  fprintf(f, "n");
  fclose(f);
  return 0;
}
BRTree* h = t->head;
BRTree* stack[i];
memset(stack, 0, i * sizeof(BRTree*));
i = 0;
while(1){
  if(stack[i]!=h){
    fprintf(f, "%llu%s%d", h->value, h->key, h->color);
    stack[i++] = h;
    if(h->left){
      fputc((int)'l', f);
      h = h->left;
      continue;
    }else if(h->right){
      fputc((int)'r', f);
      h = h->right;
      continue;
    }else{
      fputc((int)'n', f);
      if(i > 1){
        i-=2;
        h = stack[i];
      }else{
        fclose(f);
        return 0;
      }
      continue;
    }
  }else if(stack[i]==h && stack[i+1]!=h->right){
    if(h->right){
      fputc((int)'r', f);
      ++i;
      h = h->right;
      continue;
    }else{
      fputc((int)'n', f);
      if(i){
        --i;
        h = stack[i];
      }else{
        fclose(f);
        return 0;
      }
      continue;
    }
  }else{
    fputc((int)'n', f);
    if(i){
      --i;
      h = stack[i];
    }else{
      fclose(f);
      return 0;
    }
    continue;
  }
}
}

short unsigned int Load_Tree(Node* t, char* path){
  FILE* f = fopen(path, "rb");
  if(!f){
    return 3;
  }
  short unsigned int i, j;
  char buffer[BUFSIZE];
  if(fscanf(f, "%hu%c", &i, &buffer[0]) == EOF){
    fclose(f);
    return 2;
  }
  if(buffer[0] == 'n'){
    Destroy_Tree(t);
    t->size = 0;
    t->head = NULL;
    return 0;
  }else if(buffer[0] != 'h'){
    fclose(f);
    return 4;
  }
  TVal buf;
  if(fscanf(f, "%llu", &buf) == EOF){
    fclose(f);
    return 2;
  }
  bool col;
  for(j = 0; j < BUFSIZE; j++){
    buffer[j] = fgetc(f);
    if(buffer[j] < 'a' || buffer[j] > 'z'){
      if(buffer[j] == '1'){
        col = 1;
      }else{
        fclose(f);
        return 4;
      }
      buffer[j++] = '\0';
      break;
    }
  }
  BRTree* h;
  Node newtr;
  newtr.head = h = malloc(sizeof(BRTree) + j);
  if(!newtr.head){
    fclose(f);
    return 2;
  }
  newtr.size = 1;
  h->key = (char*) h + sizeof(BRTree);
  memcpy(h->key, buffer, j);
  h->value = buf;
  h->color = 1;
  h->right = h->left = NULL;
  BRTree* stack[i];
  i = 0;
  stack[i++] = h;
  while(1){
    buffer[0] = fgetc(f);
    if(buffer[0] == 'r'){
      fscanf(f, "%llu", &buf);
      for(j = 0; j < BUFSIZE; j++){
        buffer[j] = fgetc(f);
        if(buffer[j] < 'a' || buffer[j] > 'z'){
          if(buffer[j] == '1'){
            col = 1;
          }else if(buffer[j] == '0'){
            col = 0;
          }else{
            Destroy_Tree(&newtr);
            fclose(f);
            return 4;
          }
          buffer[j++] = '\0';
          break;
        }
      }
      h = h->right = malloc(sizeof(BRTree) + j);
      if(!h){
        Destroy_Tree(&newtr);
        fclose(f);
        return 2;
      }
      ++newtr.size;
      stack[i++] = h;
      h->key = (char*) h + sizeof(BRTree);
      memcpy(h->key, buffer, j);
      h->value = buf;
      h->color = col;
      h->right = h->left = NULL;
    }else if(buffer[0] == 'l'){
      fscanf(f, "%llu", &buf);
      for(j = 0; j < BUFSIZE; j++){
        buffer[j] = fgetc(f);
        if(buffer[j] < 'a' || buffer[j] > 'z'){
          if(buffer[j] == '1'){
            col = 1;
          }else if(buffer[j] == '0'){
            col = 0;
          }else{
            Destroy_Tree(&newtr);
            fclose(f);
            return 4;
          }
          buffer[j++] = '\0';
          break;
        }
      }
      h = h->left = malloc(sizeof(BRTree) + j);
      if(!h){
        Destroy_Tree(&newtr);
        fclose(f);
        return 2;
      }
      ++newtr.size;
      stack[i++] = h;
      h->key = (char*) h + sizeof(BRTree);
      memcpy(h->key, buffer, j);
      h->value = buf;
      h->color = col;
      h->right = h->left = NULL;
    }else if(buffer[0] == 'n'){
      if(i > 1){
        h = stack[--i - 1];
      }else{
        break;
      }
    }else{
      Destroy_Tree(&newtr);
      fclose(f);
      return 4;
    }
  }
  Destroy_Tree(t);
  t->head = newtr.head;
  t->size = newtr.size;
  fclose(f);
  return 0;
}

int main(){
  Node tree; //наше деpево
  tree.size = 0;
  tree.head = NULL;
  char buffer[BUFSIZE]; //считаем, что пути к файлам не превысят 257 символов
  TVal val;
  //buffer[0]=getchar();
  unsigned short int i = 0;
  while((buffer[0]=getchar())!=EOF){
    buffer[1]=getchar();
    if(buffer[0] == '+' && buffer[1] == ' '){
      i = 0;
      do{
        buffer[i] = tolower(getchar());
        if(buffer[i] < 'a' || buffer[i] > 'z'){
          break;
        }
        ++i;
      }while(1);
      if(buffer[i] != ' ' || i == 0){
        if(buffer[i] == '\n'){
          Error(1);
          continue;
        }
        while(getchar()!='\n');
        Error(1);
        continue;
      }
      //buffer[i++] = '\0';
      if(scanf("%llu%c", &val, &buffer[i]) != 2){
        //while(getchar()!='\n'){}
        Error(1);
        continue;
      }
      if(buffer[i] != '\n' && buffer[i] != EOF){
        while(getchar()!='\n');
        Error(1);
        continue;
      }
      buffer[i++] = '\0';
      i = Tree_Insert(&tree, buffer, i, val);
      if(!i){
        printf("OK\n");
      } else if(i == 1){
        printf("Exist\n");
      } else{
        Error(i);
      }
      continue;
    }else if(buffer[0] == '-' && buffer[1] == ' '){
      i = 0;
      do{
        buffer[i] = tolower(getchar());
        if(buffer[i] < 'a' || buffer[i] > 'z'){
          break;
        }
        ++i;
      }while(1);
      if(buffer[i] != '\n'){
        while(getchar()!='\n');
        Error(1);
        continue;
      }
      buffer[i] = '\0';
      i = Tree_Delete(&tree, buffer);
      if(!i){
        printf("OK\n");
      }else if(i == 1){
        printf("NoSuchWord\n");
      }else{
        Error(i);
      }
      continue;
    }else if(buffer[0] == '!' && buffer[1] == ' '){
      for(i = 0; i < 4; i++){
        buffer[i] = getchar();
      }
      buffer[i] = '\0';
      i = 0;
      if(!strcmp("Load", buffer)){
        if((buffer[0] = getchar()) != ' '){
          while(getchar()!='\n');
          Error(1);
          continue;
        }
        do{
          buffer[i] = getchar();
        }while(buffer[i++] != '\n');
        buffer[i - 1] = '\0';
        i = Load_Tree(&tree, buffer);
        if(i){
          Error(i);
          continue;
        }
        printf("OK\n");
      }else if(!strcmp("Save", buffer)){
        if((buffer[0] = getchar()) != ' '){
          while(getchar()!='\n');
          Error(1);
          continue;
        }
        do{
          buffer[i] = getchar();
        }while(buffer[i++] != '\n');
        buffer[i - 1] = '\0';
        i = Save_Tree(&tree, buffer);
        if(i){
          Error(i);
          continue;
        }
        printf("OK\n");
      }else{
        while(getchar()!='\n');
        Error(1);
        continue;
      }
    } else{ /* if((buffer[0] >= 'a' && buffer[0]) || (buffer[0] >= 'A' && buffer[0] <= 'Z')) */
      for(i = 0; i < 2; i++){
        buffer[i] = tolower(buffer[i]);
        if(buffer[i] > 'z' || buffer[i] < 'a'){
          break;
        }
      }
      if(i == 1 && buffer[1] == '\n'){
        buffer[1] = '\0';
      } else if(i == 0 && buffer[0] == '\n'){
        Error(1);
        continue;
      } else if(i != 2){
        while(getchar()!='\n');
        Error(1);
        continue;
      } else{
        while(1){
          buffer[i] = tolower(getchar());
          if(buffer[i] < 'a' || buffer[i] > 'z'){
            break;
          }
          ++i;
        }
        if(buffer[i] != '\n'){
          while(getchar()!='\n');
          Error(1);
          continue;
        }
        buffer[i] = '\0';
      }
      BRTree* t = Search_Tree(&tree, buffer);
      if(!t){
        printf("NoSuchWord\n");
        continue;
      }
      printf("OK: %llu\n", t->value);
      continue;
    }
  }
  Destroy_Tree(&tree);
  return 0;
}
