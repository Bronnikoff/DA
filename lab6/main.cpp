#include <iostream>
#include <cstring>

using namespace std;

#define BASE 10000
#define MAX_I 100000

class BigInteger{
public:
  BigInteger();
  BigInteger(char* buffer);
  BigInteger(const BigInteger& orig);
  BigInteger(istream& is);
  BigInteger(const unsigned int s1, const unsigned int s2);
  void add(unsigned short int r);
  bool newsize(unsigned int si);
  const BigInteger& operator+(const BigInteger& left, const BigInteger& right);
  const BigInteger& operator-(const BigInteger& left, const BigInteger& right);
  bool operator==(const BigInteger& left, const BigInteger& right);
  const BigInteger& operator*(const BigInteger& left, const BigInteger& right);
  const BigInteger& operator/(const BigInteger& left, const BigInteger& right);
  const BigInteger& operator^(const BigInteger& left, const BigInteger& right);
  bool operator>(const BigInteger& left, const BigInteger& right);
  bool operator<(const BigInteger& left, const BigInteger& right);

private:
  unsigned short int* data;
  unsigned int size;
  bool flag;
};

BigInteger::BigInteger(){
  flag = 0;
  size = 1;
  data = malloc(sizeof(unsigned short int));
}
