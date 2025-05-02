#pragma once
#include "include.h"

template <typename T>
void merge(vector<T>& v, int low, const vector<T>& a, const vector<T>& b)
{
    int a_i = 0, b_i = 0;
    while (a_i < a.size() && b_i < b.size())
    {
        if (a[a_i] <= b[b_i])
        {
            v[low] = a[a_i];
            a_i += 1;
        }
        else
        {
            v[low] = b[b_i];
            b_i += 1;
        }
        low += 1;
    }
    while (a_i < a.size()) { v[low] = a[a_i]; low += 1; a_i += 1; }
    while (b_i < b.size()) { v[low] = b[b_i]; low += 1; b_i += 1; }
}

template<typename T>
void mergesort(vector<T>& v, int low, int high)
{
    if (low == high) return;
    int mid = (low + high) / 2;
    mergesort(v, low, mid);
    mergesort(v, mid + 1, high);
    vector<T> a(v.begin() + low, v.begin() + mid + 1);
    vector<T> b(v.begin() + mid + 1, v.begin() + high + 1);
    merge(v, low, a, b);
}

template<typename T>
int quickpartition(vector<T>& v, int low, int high)
{
    int pivot = low, i = low + 1;
    for (; i <= high; i += 1)
    {
        if (v[i] < v[low])
        {
            pivot += 1;
            swap(v[i], v[pivot]);
        }
    }
    swap(v[low], v[pivot]);
    return pivot;
}

template<typename T>
void quicksort(vector<T>& v, int low, int high)
{
    if (low >= high) return;
    int pivot = quickpartition(v, low, high);
    quicksort(v, low, pivot - 1);
    quicksort(v, pivot + 1, high);
}