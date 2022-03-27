#ifndef HILL_CIPHER_HEADER

#define HILL_CIPHER_HEADER

#include "matrix.h"
#include <string>

using std::string;

class hillCipher {
private:
  enum { ALPHABETS = 26 };
  matrix<int> *key, *reverseKey;
  int coprimeTo26[12], inverse[ALPHABETS];

  void fixReverseKeyIfDamaged(); // Mansour
  void damageReverseKey();       // Mansour

public:
  hillCipher(int size = 3); // Mansour: initialize key, reverseKey, coprimeTo26,
                            // inverse + generateRandomKey
  ~hillCipher();            // Mansour

  void generateRandomKey(); // Mansour
  bool isValidKey(const matrix<int> &key) const;
  bool setKey(const matrix<int> &key);

  matrix<int> getKey() const;
  matrix<int> getReverseKey();

  string encrypt(const string &plainText) const;
  string decrypt(const string &cipherText); // Mansour
};

#endif
