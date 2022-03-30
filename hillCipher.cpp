#include "hillCipher.h"

void hillCipher::fixReverseKeyIfDamaged() {}

void hillCipher::damageReverseKey() {}

hillCipher::hillCipher(const int &size) {}

hillCipher::~hillCipher() {}

void hillCipher::generateRandomKey() {}

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
