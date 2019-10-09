#include <iostream>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

#define BASE 10000 //берем кратным 10 для ускоренного чтения и записи чила, если поменяем на другое, то все функции будут работать кроме чтения и записи
#define MAX_I 25000 // максимальное кол-во разрядов нашего числа(100000/NUM_NUMS)
#define NUM_NUMS 4 // 4 буквы для представления одного разряда нашего числа при чтении и записи

class BigInteger{
public:
  BigInteger(const BigInteger& obj);
  BigInteger();
  BigInteger(const unsigned int s1, const unsigned int s2);
  friend const BigInteger operator+(const BigInteger& left, const BigInteger& right);
  friend const BigInteger operator-(const BigInteger& left, const BigInteger& right);
  bool operator==(const BigInteger& right);
  friend const BigInteger operator*(const BigInteger& left, const BigInteger& right);
  friend const BigInteger operator/(const BigInteger& left, const BigInteger& right);
  friend const BigInteger operator^(const BigInteger& left, const BigInteger& right);
  const BigInteger& operator=(const BigInteger& obj);
  bool operator>(const BigInteger& right);
  bool operator<(const BigInteger& right);
  friend istream& operator>>(istream& is, BigInteger& obj);
  friend ostream& operator<<(ostream& os, const BigInteger& obj);

private:
  std::vector<unsigned int> data;
  bool flag;
  unsigned int amount;
};

BigInteger::BigInteger(const BigInteger& obj){
  amount = obj.amount;
  data.reserve(amount);
  for(unsigned i = 0; i < amount; ++i){
    data[i] = obj.data[i];
  }
  flag = obj.flag;
}

const BigInteger& BigInteger::operator=(const BigInteger& obj){
  amount = obj.amount;
  data.reserve(amount);
  for(unsigned i = 0; i < amount; ++i){
    data[i] = obj.data[i];
  }
  flag = obj.flag;
  return *this;
}

unsigned int fast_atoi(char* buffer){
  unsigned val = 0;
  for(short unsigned i = 0; i < NUM_NUMS; i++){
    val = val*10 + (*buffer++ - '0');
  }
  return val;
}

BigInteger::BigInteger(){
  data.reserve(2);
  data[0] = 0;
  amount = 1;
  flag = 0;
}

BigInteger::BigInteger(const unsigned int s1, const unsigned int s2){
  if(s1 > s2){
    data.reserve(s1);
  }else{
    data.reserve(s2);
  }
  flag = 0;
  data[0] = 0;
  amount = 1;
}

bool BigInteger::operator==(const BigInteger& right){
  if(amount != right.amount){
    return 0;
  }
  for(unsigned int i = 0; i < amount; ++i){
    if(data[i] != right.data[i]){
      return 0;
    }
  }
  return 1;
}

bool BigInteger::operator>(const BigInteger& right){
  if(amount < right.amount){
    return 0;
  }else if(amount > right.amount){
    return 1;
  }
  for(unsigned int i = amount - 1; i > 0; --i){
    if(data[i] < right.data[i]){
      return 0;
    }else if(data[i] > right.data[i]){
      return 1;
    }
  }
  if(data[0] <= right.data[0]){
    return 0;
  }
  return 1;
}

bool BigInteger::operator<(const BigInteger& right){
  if(amount > right.amount){
    return 0;
  }else if(amount < right.amount){
    return 1;
  }
  for(unsigned int i = amount - 1; i > 0; --i){
    if(data[i] > right.data[i]){
      return 0;
    }else if(data[i] < right.data[i]){
      return 1;
    }
  }
  if(data[0] >= right.data[0]){
    return 0;
  }
  return 1;
}

ostream& operator<<(ostream& os, const BigInteger& obj){
  if(obj.flag){
    os << "Error";
    return os;
  }
  unsigned zeros;
  cout << obj.data[obj.amount - 1];
  if(obj.amount == 1){
    return os;
  }
  for(unsigned int i = obj.amount-2; i > 0; --i){
    if(obj.data[i] == 0){
      for(unsigned int j = 0; j < NUM_NUMS; j++){
        std::cout << '0';
      }
      continue;
    }
    zeros = obj.data[i] * 10;
    while(zeros < BASE){
      std::cout << '0';
      zeros*=10;
    }
    os << obj.data[i];
  }
  if(obj.data[0] == 0){
    for(unsigned int j = 0; j < NUM_NUMS; j++){
      std::cout << '0';
    }
    return os;
  }
  zeros = obj.data[0] * 10;
  while(zeros < BASE){
    std::cout << '0';
    zeros*=10;
  }
  os << obj.data[0];
  return os;
}

istream& operator>>(istream& is, BigInteger& obj){
  char c = is.peek();
  //std::cout << c << '\n';
  while(c == '0'){
    is.get(c);
    c = is.peek();
  }
  if(c == ' ' || c == '\n' || c == '\t'){
    obj.data.reserve(1);
    obj.data[0] = 0;
    obj.flag = 0;
    obj.amount = 1;
    return is;
  }
  if(c == EOF){
    return is;
  }
  string s;
  is >> s;
  char buffer[NUM_NUMS];
  obj.data.clear();
  obj.amount = 0;
  obj.data.reserve((s.size()-1)/NUM_NUMS + 1);
  for(unsigned i = s.size() - 1; ; i-=NUM_NUMS){
    if(i >= NUM_NUMS){
      memcpy(buffer, s.data() + i - NUM_NUMS + 1, NUM_NUMS);
      obj.data[obj.amount++] = fast_atoi(buffer);
    }else{
      memset(buffer, '0', NUM_NUMS);
      memcpy(buffer + NUM_NUMS - 1 - i, s.data(), i+1);
      obj.data[obj.amount++] = fast_atoi(buffer);
      obj.flag = 0;
      break;
    }
  }
  return is;
}

const BigInteger operator+(const BigInteger& left, const BigInteger& right){
  BigInteger ans(left.amount + 1, right.amount + 1);
  bool r = 0, b = (left.amount > right.amount);
  if(b){
    ans.amount = right.amount;
  }else{
    ans.amount = left.amount;
  }
  unsigned int i = 0;
  for(; i < ans.amount; ++i){
    ans.data[i] = right.data[i] + left.data[i] + r;
    r = 0;
    if(ans.data[i] >= BASE){
      ans.data[i]-=BASE;
      r = 1;
    }
  }
  if(b){
    for(; i < left.amount; ++i){
      ans.data[i] = left.data[i] + r;
      r = 0;
      if(ans.data[i] == BASE){
        ans.data[i]=0;
        r = 1;
      }
    }
    if(r){
      if(left.amount > MAX_I){
        ans.flag = 1;
        return ans;
      }
      ans.amount = left.amount + 1;
      ans.data[i] = 1;
    }else{
      ans.amount = left.amount;
    }
  }else{
    for(; i < right.amount; ++i){
      ans.data[i] = right.data[i] + r;
      r = 0;
      if(ans.data[i] == BASE){
        ans.data[i]=0;
        r = 1;
      }
    }
    if(r){
      if(right.amount > MAX_I){
        ans.flag = 1;
        return ans;
      }
      ans.amount = right.amount + 1;
      ans.data[i] = 1;
    }else{
      ans.amount = right.amount;
    }
  }
  ans.flag = 0;
  return ans;
}

const BigInteger operator-(const BigInteger& left, const BigInteger& right){
  if(left.amount < right.amount){
    BigInteger ans;
    ans.flag = 1;
    return ans;
  }
  BigInteger ans(left.amount, right.amount);
  unsigned int i = 0;
  bool r = 0;
  for(; i < right.amount; ++i){
    if(left.data[i] >= right.data[i] + r){
      ans.data[i] = left.data[i] - right.data[i] - r;
      r = 0;
    }else{
      ans.data[i] = BASE + left.data[i] - right.data[i] - r;
      r = 1;
    }
  }
  if(r && i == left.amount){
    ans.flag = 1;
    return ans;
  }
  while(r){
    if(left.data[i]){
      r = 0;
      ans.data[i] = left.data[i] - 1;
      ++i;
      break;
    }
    ans.data[i++] = BASE - 1;
  }
  while(i < left.amount){
    ans.data[i] = left.data[i];
    ++i;
  }
  for(--i; !ans.data[i] && i; --i);
  ans.amount = i + 1;
  ans.flag = 0;
  return ans;
}

const BigInteger operator*(const BigInteger& left, const BigInteger& right){
  unsigned i = 0;
  if(left.amount == 1 && !left.data[0]){
    return BigInteger();
  }else if(right.amount == 1 && !right.data[0]){
    return BigInteger();
  }
  BigInteger ans(left.amount + right.amount, 0);
  ans.data.assign(left.amount + right.amount, 0);
  if(MAX_I < left.amount + right.amount - 1){
    ans.flag = 1;
    return ans;
  }
  for(; i < left.amount; ++i){
    unsigned r = 0;
    for(unsigned j = 0; j < right.amount; ++j) {
      ans.data[i+j] += left.data[i] * right.data[j] + r;
      r = ans.data[i+j] /  BASE;
      ans.data[i+j]-= r * BASE;
    }
    if(r){
      ans.data[i + right.amount]+=r;
    }
  }
  i = left.amount + right.amount - 1;
  while(i &&  !ans.data[i]){
    --i;
  }
  ans.amount = i + 1;
  if(ans.amount > MAX_I){
    ans.flag = 1;
    return ans;
  }
  ans.flag = 0;
  return ans;
}

const BigInteger operator^(const BigInteger& left, const BigInteger& right){
  BigInteger ans, cur(left);
  if(left.amount == 1 && !left.data[0]){
    if(right.amount == 1 && !right.data[0]){
      ans.flag = 1;
    }
    return ans;
  }
  if((right.amount == 1 && !right.data[0]) || (left.amount == 1 && left.data[0] == 1)){
    ans.amount = 1;
    ans.data[0] = 1;
    return ans;
  }
  if(right.amount > 2){
    ans.flag = 1;
    return ans;
  }
  unsigned int i = 0;
  for(short unsigned j = right.amount - 1; j > 0; --j){
    i+=right.data[j];
    i*=BASE;
  }
  i+=right.data[0];
  ans.data[0] = 1;
  while(i && !ans.flag && !cur.flag){
    if(i & 1){
      ans = ans * cur;
    }
    cur = cur * cur;
    i>>=1;
  }
  if(cur.flag){
    ans.flag = 1;
  }
  return ans;
}

const BigInteger operator/(const BigInteger& left, const BigInteger& right){
  if(right.amount == 1 && !right.data[0]){
    BigInteger ans;
    ans.flag = 1;
    return ans;
  }
  if(right.amount > left.amount){
    return BigInteger();
  }
  BigInteger ans(left.amount - right.amount + 1, 0), bint(right.amount + 1, 0);
  BigInteger cur(right.amount + 1, 0), x, m;
  bint.data[right.amount] = 0;
  bint.amount = right.amount;
  for(unsigned i = 0; i < right.amount; ++i){
    bint.data[i] = left.data[left.amount - right.amount + i];
  }
  for(unsigned i = left.amount - right.amount; ; --i){
    //cout << bint << '\n';
    unsigned l = 0, r = BASE;
    while(l <= r){
      m.data[0] = (l + r) >> 1;
      cur = right * m;
      //std::cout << m.data[0] << ' ' << cur << '\n';
      //getchar();
      if(cur < bint || cur == bint){
        x.data[0] = m.data[0];
        l = m.data[0] + 1;
      }else{
        r = m.data[0] - 1;
      }
    }
    //std::cout << x.data[0] << ' ' << i << '\n';
    ans.data[i] = x.data[0];
    if(!i){
      break;
    }
    bint = bint - (x * right);
    //cout << bint << '\n';
    for(unsigned j = bint.amount; j > 0; --j){
      bint.data[j] = bint.data[j - 1];
    }
    bint.data[0] = left.data[i - 1];
    if(bint.data[bint.amount]){
      ++bint.amount;
    }
  }
  unsigned i = left.amount - right.amount;
  while(i && !ans.data[i]){
    --i;
  }
  ans.amount = i + 1;
  return ans;
}


int main(){
  BigInteger a, b;
  char op;
  while(1){
    if(cin.peek() == EOF){
      break;
    }
    cin >> a;
    cin.ignore(32767, '\n');
    cin >> b;
    cin.ignore(32767, '\n');
    cin >> op;
    cin.ignore(32767, '\n');
    switch (op) {
      case '+':
        cout << a + b << '\n';
        break;
      case '-':
        cout << a - b << '\n';
        break;
      case '*':
        cout << a * b << '\n';
        break;
      case '^':
        cout << (a ^ b) << '\n';
        break;
      case '/':
        cout << (a / b) << '\n';
        break;
      case '>':
        if(a > b){
          cout << "true\n";
        }else{
          cout << "false\n";
        }
        break;
      case '<':
        if(a < b){
          cout << "true\n";
        }else{
          cout << "false\n";
        }
        break;
      case '=':
        if(a == b){
          cout << "true\n";
        }else{
          cout << "false\n";
        }
        break;
    }
  }
  return 0;
}
