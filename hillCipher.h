#ifndef HILL_CIPHER_HEADER
#define HILL_CIPHER_HEADER

#include "matrix.h"
#include <chrono>
#include <random>
#include <string>

using std::string;
using std::uniform_int_distribution;

/**
 * Use reversekey as a pointer since it should be destroyed in some instances =
   NULL, should be erased from memory before doing so.
 * Modular arithmetic operations work in the same way as primary matrix
   arithmetic operations, but with mod 26 applied to each operation.
 * rng: random generation engine used to generate equal-probability random
   values
 * The algorithm is to multiply every sequence of characters in plain text by
 the key, which we achieved in one operation by fitting all strings in one
 matrix with size of columns equal to key size and number of rows equal to
 string size divided by key size
 **/
class hillCipher {
private:
  enum { ALPHABETS = 26 };
  matrix<int> key, *reverseKey;
  int coprimeTo26[12], inverse[ALPHABETS];
  std::mt19937 rng;

  void fixReverseKey();
  void damageReverseKey();

  matrix<int> mulWithMod(const matrix<int> &mat1,
                         const matrix<int> &mat2) const;
  int determinantWithMod(const matrix<int> &mat) const;
  matrix<int> adjugateWithMod(const matrix<int> &mat) const;

  void rowAddition(const int &mulRow, const int &additionRow,
                   const int &mulVal);

public:
  hillCipher(const int &size = 3);
  ~hillCipher();

  void generateRandomKey(const int &size = 3);
  bool isValidKey(const matrix<int> &key) const;
  bool setKey(const matrix<int> &key);

  const matrix<int> &getKey() const;
  const matrix<int> &getReverseKey();

  string encrypt(const string &plainText, char dummyLetter = 'a') const;
  string decrypt(const string &cipherText);
};

#endif
