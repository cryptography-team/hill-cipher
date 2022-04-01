#include "hillCipher.h"

void hillCipher::fixReverseKey() {
  if (reverseKey != NULL)
    return;
  reverseKey = new matrix<int>(key.getRows(),
                                key.getRows());
  *reverseKey = adjugateWithMod(key);
  *reverseKey *= inverse[determinantWithMod(key)];
  *reverseKey %= ALPHABETS;
}

void hillCipher::damageReverseKey() { 
   if(!reverseKey)
        return;
  delete reverseKey; reverseKey = NULL; }

matrix<int> hillCipher::mulWithMod(const matrix<int> &mat1,
                                   const matrix<int> &mat2) const {
  int size1 = mat1.getRows(), size2 = mat1.getCols(), size3 = mat2.getCols();
  matrix<int> res(size1, size3, 0);
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size3; j++)
      for (int k = 0; k < size2; k++) {
        res(i, j) += mat1(i, k) * mat2(k, j) % ALPHABETS;
        if (res(i, j) >= ALPHABETS)
          res(i, j) -= ALPHABETS;
      }
  return res;
}

int hillCipher::determinantWithMod(const matrix<int> &mat) const {
  int size = mat.getRows();
  if (size == 1)
    return mat(0, 0);
  matrix<int> tmp(size - 1, size - 1);
  for (int i = 1; i < size; ++i)
    for (int j = 1; j < size; ++j)
      tmp(i - 1, j - 1) = mat(i, j);
  int det = 0;
  for (int i = 0, sign = 1; i < size; i++, sign *= -1) {
    det += sign * mat(0, i) * determinantWithMod(tmp) % ALPHABETS;
    if (det < 0)
      det += ALPHABETS;
    else if (det >= ALPHABETS)
      det -= ALPHABETS;
    if (i != size - 1) {
      for (int j = 1; j < size; j++)
        tmp(j - 1, i) = mat(j, i);
    }
  }
  return det;
}

matrix<int> hillCipher::adjugateWithMod(const matrix<int> &mat) const {
  int size = mat.getRows();
  matrix<int> adj(size, size);
  for (int i = 0; i < size; ++i) {
    matrix<int> tmp(size - 1, size - 1);
    for (int j = 0, jj = 0; j < size; ++j)
      if (j != i) {
        for (int k = 1; k < size; ++k)
          tmp(jj, k - 1) = mat(j, k);
        ++jj;
      }
    for (int j = 0; j < size; ++j) {
      adj(i, j) = (i + j) & 1 ? -1 : 1;
      adj(i, j) *= determinantWithMod(tmp);
      if (adj(i, j) < 0)
        adj(i, j) += ALPHABETS;
      if (j != size - 1) {
        for (int k = 0, kk = 0; k < size; ++k)
          if (k != i) {
            tmp(kk, j) = mat(k, j);
            ++kk;
          }
      }
    }
  }
  return adj.transpose();
}

hillCipher::hillCipher(const int &size)
    : rng(std::chrono::steady_clock::now().time_since_epoch().count()),
      key(size, size),reverseKey(NULL) {
  for (int i = 0, j = 1; i < 12; i++, j += 2) {
    if (j == 13)
      j += 2;
    coprimeTo26[i] = j;
  }
  for (int k = 0; k < ALPHABETS; k++) {
    inverse[k] = -1;
    if (!(k % 2) || k == 13)
      continue;
    for (int j = 0; j < ALPHABETS; j++) {
      if (k * j % ALPHABETS == 1) {
        inverse[k] = j;
        break;
      }
    }
  }
  generateRandomKey(size); // initialize key matrix and generate it randomly
}

hillCipher::~hillCipher() {
  if (!reverseKey)
    delete reverseKey;
}

void hillCipher::generateRandomKey(const int &size) {
    damageReverseKey();
  key = matrix<int>(size, size, 0);
  for (int i = 0; i < size; i++) {
    key(i, i) = coprimeTo26[uniform_int_distribution<int>(0, 11)(rng)];
  }
  for (int i = 0; i < ALPHABETS; i++) {
    rowAddition(uniform_int_distribution<int>(0, size - 1)(rng),
                uniform_int_distribution<int>(0, size - 1)(rng),
                uniform_int_distribution<int>(0, ALPHABETS - 1)(rng));
  }
}

void hillCipher::rowAddition(int mulRow, int additionRow, int mulVal) {
  int len = key.getRows();
  if (mulVal == 0 || mulRow == additionRow)
    return;
  for (int i = 0; i < len; i++) {
    key(additionRow, i) += (key(mulRow, i) * mulVal) % ALPHABETS;
    if (key(additionRow, i) >= ALPHABETS)
      key(additionRow, i) -= ALPHABETS;
  }
}

bool hillCipher::isValidKey(const matrix<int> &key) const {
  int det = determinantWithMod(key);
  return inverse[det] != -1;
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
  fixReverseKey();
  return *reverseKey;
}

string hillCipher::encrypt(const string &plainText, char dummyLetter) const {
  if (plainText.empty())
    return "";
  int len = plainText.size(), matSize = key.getRows(), numOfChars = 0;
  for (const char &c : plainText)
    numOfChars += ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
  matrix<int> textMat((numOfChars + matSize - 1) / matSize, matSize);
  int j = 0, k = 0;
  for (int i = 0; i < len; ++i) {
    char c = plainText[i];
    if ('A' <= c && c <= 'Z')
      c -= 'A';
    else if ('a' <= c && c <= 'z')
      c -= 'a';
    else
      continue;
    textMat(j, k) = c;
    ++k;
    if (k == matSize) {
      ++j;
      k = 0;
    }
  }
  if (k != 0) {
    dummyLetter -= 'a';
    while (k < matSize) {
      textMat(j, k) = dummyLetter;
      ++k;
      ++numOfChars;
    }
  }
  textMat = mulWithMod(textMat, key);
  string cipherText(numOfChars, 'A');
  j = 0;
  k = 0;
  for (int i = 0; i < numOfChars; ++i) {
    cipherText[i] += textMat(j, k);
    ++k;
    if (k == matSize) {
      ++j;
      k = 0;
    }
  }
  return cipherText;
}

string hillCipher::decrypt(const string &cipherText) {
  if (cipherText.empty())
    return "";
  int size = cipherText.size();
  int len = key.getRows();
  if (size % len)
    return "";
  string res;
  fixReverseKey();
  matrix<int> plain(size / len, len);
  for (int i = 0; i < size; i += len) {
    for (int j = 0; j < len; j++)
      plain(i / len, j) = cipherText[i + j] - 'A';
  }
  plain = mulWithMod(plain, *reverseKey);
  int rows = plain.getRows(), cols = plain.getCols();
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      res += plain(i, j) + 'a';
  return res;
}
