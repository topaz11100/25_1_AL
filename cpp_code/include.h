#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_set>

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