#include "hillCipher.h"
using std::cout;
using std::endl;
void hillCipher::fixReverseKeyIfDamaged() {}

void hillCipher::damageReverseKey() {}

hillCipher::hillCipher(const int &size) : rng(std::chrono::steady_clock::now().time_since_epoch().count()){
    key = matrix<int>(size,size); generateRandomKey(); //initialize key matrix and generate it randomlly
    reverseKey = new matrix<int>(size,size);   //initialize reverse key matrix
    for(int i = 0,j=1 ; i<12  ; i++ , j+=2){
        if(j == 13) j+= 2;
        coprimeTo26[i] = j;}

}

hillCipher::~hillCipher() {}

void hillCipher::generateRandomKey() {
    using std::uniform_int_distribution;

    int len = key.getRows();
    for(int i = 0 ; i<len ; i++){
            key(i,i) = coprimeTo26[uniform_int_distribution<int>(0,12)(rng)];}
            cout<<key<<endl <<key.determinant();
    for(int i = 0 ;i<ALPHABETS;i++){
            rowAddition(uniform_int_distribution<int>(0,len-1)(rng)
                            ,uniform_int_distribution<int>(0,len-1)(rng)
                            ,uniform_int_distribution<int>(0,ALPHABETS-1)(rng));}
}

void hillCipher :: rowAddition(int mulRow, int additionRow,int mulVal){
    int len = key.getRows();
    if(mulRow > len || additionRow > len || mulRow == additionRow) return;
        for(int i = 0 ; i<len ; i++)
           {
               key(additionRow,i) += ((key(mulRow,i)*mulVal)%ALPHABETS);
               if(key(additionRow,i) >= ALPHABETS)
                key(additionRow,i) %= ALPHABETS;
           }
}

bool hillCipher::isValidKey(const matrix<int> &key) const {
  int det = key.determinant();
  return det % 2 != 0 && det != 13;
}

bool hillCipher::setKey(const matrix<int> &key) {
  if (!isValidKey(key))
    return false;
  this->key = key;
  damageReverseKey();
  return true;
}

const matrix<int> &hillCipher::getKey() const { return key; }

const matrix<int> &hillCipher::getReverseKey() {
  fixReverseKeyIfDamaged();
  return *reverseKey;
}

string hillCipher::encrypt(const string &plainText, char dummyLetter) const {
  int len = plainText.size(), matSize = key.getRows();
  matrix<int> textMat((len + matSize - 1) / matSize, matSize);
  int j = 0, k = 0;
  for (int i = 0; i < len; ++i) {
    char c = plainText[i];
    if ('A' <= c && c <= 'Z')
      c += 'a' - 'A';
    textMat(j, k) = c - 'a';
    ++k;
    if (k == matSize) {
      ++j;
      k = 0;
    }
  }
  dummyLetter -= 'a';
  while (k < matSize) {
    textMat(j, k) = dummyLetter;
    ++k;
    ++len;
  }
  textMat *= key;
  textMat %= ALPHABETS;
  string cipherText(len, 0);
  j = 0;
  k = 0;
  for (int i = 0; i < len; ++i) {
    cipherText[i] = textMat(j, k) + 'A';
    ++k;
    if (k == matSize) {
      ++j;
      k = 0;
    }
  }
  return cipherText;
}

string hillCipher::decrypt(const string &cipherText) {}
