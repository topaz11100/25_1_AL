교재에 정의된 $P[v_i](A)$ 로 부터 경로 복원하는 법

$$
D[v_i](A) = min_{v_k \in A} {(G[i][k] + D[v_k][A - v_k])}
$$

위에서 선택된 $v_k$에 대해 $P[v_i](A) = k$ 이므로
$P[v_i](A) = k$ 의 의미 : $v_i \rightarrow v_1$ 로 가는 경로 중 $A$ 의 점을 한 번만 지나는 최단 경로에서 $v_i$ 의 다음 점은 $v_k$ 다
따라서 $v_0,V-v_0$ 을 $P$ 에 넣고
나온 결과 $v_k$ 와 다음 단계 집합 $V-v_0-v_k$ 를 다시 넣는 식으로 경로를 추적할 수 있다
의사코드는 아래와 같다

```python
def path(V, P):
    path, A, vi = [v0], V - v0, v0
    while A:
        vi = P[vi](A)
        path.append(vi)
        A, vi = A - vi, vi
    return path
```

무향그래프의 TSP는 유향그래프의 TSP보다 두 배 빠른데
이 때 점화식을 제시하라

무향그래프는 대칭(`G[i][j] == G[j][i]`)이므로 정답인 해밀턴 경로는 두 개 생성된다.
따라서 부분 정답도 두 개 씩 생성되므로 대칭을 없애는 것이 핵심이다

$$
D(A, v_k)
$$





















