#include <iostream>
#include "D:\PECE\B+\src\B+T.h"
using namespace std;
int main()
try{
    BPTree BT(3,2);
    BT.Insert(1, "f");
    BT.Insert(2, "fd");
    BT.Insert(3, "d");
    BT.Insert(4, "THe4");
    BT.Insert(5,"his5");
    BT.Insert(6, "ff6");
    BT.Insert(7, "fd7");
    cout << "1" << endl;
    string d = BT.GetValue(3);
    cout << d << endl;
    BT.Delete(3);
    BT.GetValue(3);
    getchar();
    getchar();
    getchar();
    return 1;
}
catch(runtime_error& e){
    cout << e.what() << endl;
    getchar();
    getchar();
}