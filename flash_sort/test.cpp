#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int main() {
  char* a = new char[20];
  strcpy(a, "abcdefg;hij");
  string b(a);
  cout << &a << endl;
  cout << sizeof a << endl;
  cout << a << endl;
  cout << &b << endl;
  cout << sizeof b << endl;
  cout << b << endl;
  strcpy(a, "dsfd");
  cout << &a << endl;
  cout << sizeof a << endl;
  cout << a << endl;
  cout << &b << endl;
  cout << sizeof b << endl;
  cout << b << endl;
  b = "dfsdfsdf";
  cout << &b << endl;
  cout << b << endl;
  return 0;
}
