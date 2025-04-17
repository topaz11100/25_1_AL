#include "include.h"
#include "bin_search.h"
#include "mysort.h"
#include "combi.h"
#include <bitset>


int main()
{
    int n = -1, k = 5;
    string a = to_string(n);
    bitset<10> bit(stoi(a));
    cout << a << " " << bit.to_string();
    return 0;
}