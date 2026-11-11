#include <map>
#include <iostream>
#include <string>
using namespace std;
int main()
{
    map<string,int> A;
    A["1"] = 1;
    A["2"] = 2;
    A["3"] = 3;

    A.erase("3");
    cout << bool(A["3"]) <<" hello";
}