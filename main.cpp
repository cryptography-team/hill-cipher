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
    cout << "\t1- Encrypt a plain text using the current key.\n";
    cout << "\t2- Decrypt";
    cout << "\t3- Get key";
    cout << "\t4- Get reverse key";
    cout << "\t5- Set key";
    cout << "\t6- Set random key";
    cout << "\t7- Exit the program.\n";
    cout << "\n\tYour choice is [1, 7]: " << flush;
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
    cout << "\n\tPress any key to continue..." << endl;
#ifdef __unix__
    system("read");
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
