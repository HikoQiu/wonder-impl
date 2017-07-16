#include <iostream>
#include "slice.h"

using namespace std;

int main(int argc, char* argv[]) {
    Slice s = Slice("Hello");
    cout << "Str len: " << s.size() << " data: " << s.data() << endl;

    Slice s1 = Slice("Hello");
    cout << (s == s1) << endl;
    cout << (s != s1) << endl;

    string str = "Hello";
    cout << "Str len: " << str.size() << " data: " << str.data() << endl;
    return 0;
}
