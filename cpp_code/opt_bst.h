#include "include.h"

class Bintree
{
public:
    int data;
    Bintree* left;
    Bintree* right;
};

int A_recur(vector<vector<double>>& A, const vector<double>& P, int i, int j)
{
    //점화식
    //A[i][j+1] = min_{k=i}^{k=j}{A[i][k] + A[k+1][j+1]} + sum_{t=i}^{t=j}{P[t]}
    double temp = __DBL_MAX__, p = 0;
    int result = 0;
    for (int t = i; t < j; t += 1)
    {
        int comp = A[i][t] + A[t + 1][j];
        if (comp < temp)
        {
            temp = comp;
            result = t;
        }
        p += P[t];
    }
    A[i][j] = temp + p;
    return result;
}

void opt_bst(vector<vector<double>>& A, const vector<double>& P, vector<vector<int>>& K)
{
    int n = P.size();

    //초기 조건
    A = vector<vector<double>>(n + 1, vector<double>(n + 1, 0));
    K = vector<vector<int>>(n, vector<int>(n, -1));
    for (int i = 0; i < n; i += 1)
    {
        A[i][i + 1] = P[i];
        K[i][i] = i;
    }

    //대각방향으로 점화식 전개
    for (int d = 2; d < n + 1; d += 1)
    {
        for (int t = 0; t < n - (d - 1); t += 1)
        {
            K[t][t + d - 1] = A_recur(A, P, t, t + d);
        }
    }
}

Bintree* make_opt_bst(const vector<vector<int>>& K, int start, int end)
{
    if (start > end) return nullptr;
    int k = K[start][end];
    if (k == -1) return nullptr;
    Bintree* root = new Bintree;
    root->data = k;
    root->left = make_opt_bst(K, start, k - 1);
    root->right = make_opt_bst(K, k + 1, end);
    return root;
}