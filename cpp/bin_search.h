#pragma once
#include "include.h"

template<typename T>
int bin_search(const vector<T>& v, const T& t, int low, int high)
{
    if (low > high) return -1;
    int mid = (low + high) / 2;
    if (t == v[mid]) return mid;
    else if (t > v[mid]) return bin_search(v, t, mid + 1, high);
    else if (t < v[mid]) return bin_search(v, t, low, mid - 1);
}