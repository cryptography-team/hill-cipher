#include "hillCipher.h"
#include "matrix.h"
#include <iostream>

using namespace std;

int main() {
  hillCipher tool;
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
    cout << "\t5- Set a key\n";
    cout << "\t6- Set a random key\n";
    cout << "\t7- Exit the program\n";
    cout << "\n\tYour choice is [1-7]: " << flush;
    int choice;
    cin >> choice;
    if (choice == 7)
      break;
    switch (choice) {
    case 1:

      break;
    case 2:

      break;
    // Other cases here...
    default:
      cout << "\tPlease enter a valid number..." << endl;
    }
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
