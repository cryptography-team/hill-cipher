#include "hillCipher.h"
#include "matrix.h"
#include <iostream>

using namespace std;

int main() {
  hillCipher tool;
  string plain = "Muns And Yasser";
  string cipher;
  matrix<int> key(3, 3);
  int size;

  matrix<int> reversekey(3, 3);
  cout
      << "  ================================================================\n";
  cout << "\tHill Cipher tool was initiated with a random key.\n";
  cout << "\tHope it will be a good interface. Use it as you wish :)\n";
  while (true) {
    cout
        << "  "
           "================================================================\n";
    cout << "\tChoose one of the following options:\n";
    cout << "\t1- Encrypt a plain text using the current key\n";
    cout << "\t2- Decrypt a cipher text using the current key\n";
    cout << "\t3- Get the current key\n";
    cout << "\t4- Get the current reverse key\n";
    cout << "\t5- Set a custom key\n";
    cout << "\t6- Set a random key\n";
    cout << "\t7- Exit the program\n";
    cout << "\n\tYour choice is [1-7]: " << flush;
    int choice;
    cin >> choice;
    if (choice == 7)
      break;
    switch (choice) {
    case 1:
      char ch;
      cout << "\tThe last plain text was \"" << plain
           << "\".. Continue with this plain text? (Y/n): " << flush;
      cin >> ch;
      if (ch == 'n') {
        cout << "\tSet your plain text (in one line):" << endl;
        cin.ignore();
        getline(cin, plain);
      }
      cout << "\tSet your dummy letter that will be used in case the plain "
           << "text was not long enough: " << flush;
      char dummyLetter;
      cin >> dummyLetter;
      cipher = tool.encrypt(plain, dummyLetter);
      cout << "\tThis is the corresponding cipher text:\n\t" << cipher;
      break;
    case 2:
      cout << "\tThe last cipher text was \"" << cipher
           << "\".. Continue with the last cipher text? (Y/n): " << flush;
      cin >> ch;
      if (ch == 'n') {
        cout << "\tSet your cipher text (in one line):" << endl;
        cin.ignore();
        getline(cin, cipher);
      }
      plain = tool.decrypt(cipher);
      if (plain.empty() && !cipher.empty())
        cout << "\tNot a valid cipher.. Cipher's size is not divisible by "
             << "matrix size (add some dummy letters)\n";
      if (cipher.empty() || !plain.empty())
        cout << "\tThis is the corresponding plain text:\n\t" << plain;
      break;
    case 3:
      cout << tool.getKey();
      break;
    case 4:
      cout << tool.getReverseKey();
      break;
    case 5:
      int size;
      cout << "\tSet your key size: " << flush;
      cin >> size;
      key = matrix<int>(size, size);
      reversekey = matrix<int>(size, size);
      cout << "\tEnter your " << size << 'x' << size
           << " key (one row on each line):" << endl;
      for (int i = 0; i < size; i++) {
        cout << '\t' << flush;
        for (int j = 0; j < size; j++)
          cin >> key(i, j);
      }
      if (!tool.setKey(key))
        cout << "\tSad.. Not a valid key.. This key will not be set.";
      else
        cout << "\tPerfecto.. The key has been set.";
      break;
    case 6:
      cout << "\tSet your key size: " << flush;
      cin >> size;
      tool.generateRandomKey(size);
      cout << "\tThis random key has been set:\n" << tool.getKey();
      break;
    default:
      cout << "\tPlease enter a valid option...";
    }
    cout << endl;
#ifdef __unix__
    system("read -p \"Press return key to continue...\"");
    system("clear");
#else
    system("pause");
    system("cls");
#endif
  }
  cout
      << "  ================================================================\n";
  cout << "\tSad to see you go :(\n";
  return 0;
}
