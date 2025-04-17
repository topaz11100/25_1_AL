#include "include.h"

template <typename T>
void floyd_min_D(const vector<vector<T>>& input, vector<vector<int>>& D, vector<vector<int>>& Node)
{
    D = input;
    int n = input.size();
    Node = vector<vector<int>>(n, vector<int>(n, -1));
    for (int k = 0; k < n; k += 1)
        for (int i = 0; i < n; i += 1)
            for (int j = 0; j < n; j += 1)
                if (D[i][k] + D[k][j] < D[i][j])
                {
                    D[i][j] = D[i][k] + D[k][j];
                    Node[i][j] = k;
                }
}

void short_path_i_to_j(const vector<vector<int>>& Node, int i, int j, vector<int>& path)
{
    if (Node[i][j] == -1) return;
    int k = Node[i][j];
    short_path_i_to_j(Node, i, k, path);
    path.push_back(k);
    short_path_i_to_j(Node, k, j, path);
}