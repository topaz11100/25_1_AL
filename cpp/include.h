#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>

using namespace std;

template<typename T>
void print_vector(const vector<T>& v)
{
    for (const T& x : v)
    {
        cout << x << " ";
    }
    cout << endl;
}