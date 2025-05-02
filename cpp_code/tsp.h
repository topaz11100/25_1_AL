#include "include.h"

/*
입력 : 그래프 인접행렬 G

점화식
D[v_i](A) = min_{v_k in A} {G[i][k] + D[v_k][A - k]}
D[v_i](NULL) = G[i][0]

방향
A를 공집합부터 V - v_0까지 키우기

문제 목표
D[v_0](V - v_0)
*/



void tsp(const vector<vector<double>>& G)
{
    unordered_set<int> V;
    vector<pair<int, unordered_set<int>>> D;
}






















