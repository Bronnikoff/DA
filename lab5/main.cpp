#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

#define ALFASIZE 27

typedef struct TNode{
  TNode* child[ALFASIZE];
  unsigned list;
  unsigned lenght;
  unsigned first;
  TNode* suffix;
  //TNode* father;
}TNode;

class SufTree{
public:
  SufTree(istream& is);
  string& GetStr();
  ~SufTree();
  void StatFind(istream& is);

private:
  TNode dime;
  TNode head;
  string str;
  std::vector<TNode*> mass;
};

void SufTree::StatFind(istream& is){
  char c;
  TNode* suff_v = &head;
  TNode* help = nullptr;
  unsigned ms = 1, i = 1, suff_len = 0, helen = 0;
  short suff_ich;
  is >> c;
  do{
    if(suff_len){
      while(c != str[suff_v->child[suff_ich]->first + suff_len]){
        if(ms == str.size()){
          cout << i << endl;
        }
        --ms;
        ++i;
        help = suff_v;
        helen = 0;
        suff_v = suff_v->suffix;
        while(suff_len > suff_v->child[str[help->child[suff_ich]->first + helen]-'a']->lenght){
          suff_v = suff_v->child[str[help->child[suff_ich]->first + helen]-'a'];
          helen+=suff_v->lenght;
          suff_len-=suff_v->lenght;
        }
        if(suff_len == suff_v->child[str[help->child[suff_ich]->first + helen]-'a']->lenght){
          suff_v = suff_v->child[str[help->child[suff_ich]->first + helen]-'a'];
          suff_len = 0;
          suff_ich = c - 'a';
          break;
        }else{
          suff_ich = str[help->child[suff_ich]->first + helen]-'a';
        }
      }
    }
    if(!suff_len){
      suff_ich = c - 'a';
      while(!suff_v->child[suff_ich]){
        if(ms == str.size()){
          cout << i << endl;
        }
        suff_v = suff_v->suffix;
        ++i;
        --ms;
      }
    }
    //std::cout << ms << ' ' << str.size() << '\n';
    ++ms;
    ++suff_len;
    if(suff_v->child[suff_ich]->lenght == suff_len){
      suff_len = 0;
      suff_v = suff_v->child[suff_ich];
    }
    if(!is.get(c)){
      break;
    }
    c = tolower(c);
  }while(c >= 'a' && c <='z');
  if(ms == str.size()){
    cout << i << endl;
  }
}

string& SufTree::GetStr(){
  return str;
}

SufTree::~SufTree(){
  for(unsigned i = 0; i < mass.size(); ++i){
    free(mass[i]);
  }
}

SufTree::SufTree(istream& is){
  //std::cout << (char)('z' + 1) << '\n';
  char c;
  is >> c;
  c = tolower(c);
  do{
    str.push_back(c);
    is.get(c);
    c = tolower(c);
  }while(c >= 'a' && c <= 'z');
  str.push_back('z' + 1);
  mass.reserve(str.size());
  TNode* suff_v = &head;
  TNode* last_v = &dime;
  TNode* help = nullptr;
  unsigned suff_len = 0, last_list = 0, itter = 0;
  short suff_ich = -1;
  for(short i = 0; i < ALFASIZE; ++i){
    dime.child[i] = &head;
    head.child[i] = nullptr;
  }
  dime.list = head.list = head.first = 0;
  head.lenght = 1;
  head.suffix = &dime;
  /* основной цикл */
  for(unsigned i = 0; i < str.size(); ++i){
    //std::cout << str[i] << endl;
    last_v = &dime;
    /* пока позиция на ребре */
    if(suff_len && str[i] == str[(suff_v->child[suff_ich]->first)+suff_len]){
      ++suff_len;
    }else{
      /* если же второе, то создаеем 2 вершины (лист и развилку) и
         переходим по суффиксной ссылке */
      while(suff_len){
        //развилка:
        help = (TNode*)malloc(sizeof(TNode));
        mass.push_back(help);
        memset(help->child, 0, ALFASIZE*sizeof(TNode*));
        help->child[str[suff_v->child[suff_ich]->first + suff_len] - 'a'] = suff_v->child[suff_ich];
        suff_v->child[suff_ich]->lenght-=suff_len;
        help->first = suff_v->child[suff_ich]->first;
        suff_v->child[suff_ich]->first+=suff_len;
        help->lenght = suff_len;
        suff_v->child[suff_ich] = help;
        help->list = 0;
        last_v->suffix = help;
        last_v = help;
        //лист:
        help = (TNode*)malloc(sizeof(TNode));
        mass.push_back(help);
        help->first = i;
        help->lenght = str.size() - i;
        help->list = ++last_list;
        last_v->child[str[i] - 'a'] = help;
        //std::cout << "list:" << (char)('a' + suff_ich) << str[suff_v->child[suff_ich]->first] << '\n';
        //std::cout << "listt:" << str[i] << str[help->first] << '\n';
        //тикаем по ссылке дальше:
        help = suff_v;
        suff_v = suff_v->suffix;
        itter = 0;
        // гоняем по счетчику:
        while(suff_v->child[str[help->child[suff_ich]->first + itter]-'a']->lenght < suff_len){
          //std::cout << suff_v->child[str[help->child[suff_ich]->first + itter]-'a']->lenght << ' ' << suff_len << '\n';
          suff_v = suff_v->child[str[help->child[suff_ich]->first + itter]-'a'];
          suff_len-=suff_v->lenght;
          itter+=suff_v->lenght;
        }
        if(suff_v->child[str[help->child[suff_ich]->first + itter]-'a']->lenght == suff_len){
          suff_v = suff_v->child[str[help->child[suff_ich]->first + itter]-'a'];
          suff_len = 0;
          last_v->suffix = suff_v;
          break;
        }else{
          suff_ich = str[help->child[suff_ich]->first + itter]-'a';
        }
      }
      if(!suff_len){
      while(1){
          if(!suff_v->child[str[i] - 'a']){
            help = (TNode*)malloc(sizeof(TNode));
            mass.push_back(help);
            help->list = ++last_list;
            help->lenght = str.size() - i;
            help->first = i;
            suff_v->child[str[i] - 'a'] = help;
            //std::cout << "listtt:" << str[i] << str[help->first] << '\n';
            suff_v = suff_v->suffix;
          }else{
            //std::cout << str[suff_v->child[str[i]-'a']->first] << suff_v->child[str[i] - 'a']->first << ' ' << suff_v->child[str[i] - 'a']->lenght << '\n';
            suff_ich = str[i] - 'a';
            ++suff_len;
            break;
          }
        }
      }
    }
    if(suff_len && suff_len == suff_v->child[suff_ich]->lenght){
      suff_v = suff_v->child[suff_ich];
      suff_len = 0;
    }
  }
}

int main(){
  while(cin.peek() != EOF){
    SufTree tree(std::cin);
    tree.StatFind(std::cin);
    //std::cout  << tree.GetStr() << endl;
  }
  return 0;
}
