#pragma once
#include "include.h"

unsigned long long recur_combi(int n, int k)
{
    if (k == 0 || k == n || n == 0) return 1;
    return recur_combi(n - 1, k) + recur_combi(n - 1, k - 1);
}

unsigned long long bottom_up_combi(int n_arg, int k_arg)
{
    vector<vector<unsigned long long>> C(n_arg + 1, vector<unsigned long long>(k_arg + 1));
    for (int n = 0; n <= n_arg; n+=1)
    {
        for (int k = 0; k <= min(n, k_arg); k+=1)
        {
            if (k == 0 || k == n) C[n][k] = 1;
            else C[n][k] = C[n - 1][k] + C[n - 1][k - 1];
        }
    }
    return C[n_arg][k_arg];
}

vector<vector<unsigned long long>> C(101, vector<unsigned long long>(101));

unsigned long long top_down_combi(int n, int k)
{
    if (k == 0 || k == n) return 1;
    else if (C[n][k]) return C[n][k]; 
    return C[n][k] = top_down_combi(n - 1, k) + top_down_combi(n - 1, k - 1);
}

