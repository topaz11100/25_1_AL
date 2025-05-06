### 3. 보조정리 (분산 갱신 식)

#### (a) 원소 **추가**

집합 $A$의 크기를 $|A|=m$, 평균을 $\mu_A$, 분산을 $V_A$라 하자.
새 원소 $x$를 추가하여 $A^+=A\cup{x}$를 만들면 $|A^+|=m+1$이고 새로운 평균은

$$
\mu_{A^+} \;=\; \frac{m\mu_A + x}{m+1}.
$$

모든 원소에 대한 제곱 편차 합(총 제곱오차, $\text{SSE}$)은

$$
\text{SSE}_{A^+}
= \sum_{y\in A^+} (y-\mu_{A^+})^2
= \sum_{y\in A}(y-\mu_{A^+})^2 + (x-\mu_{A^+})^2.
$$

평균 이동량 $\mu_{A^+}-\mu_A = \dfrac{x-\mu_A}{m+1}$ 을 이용하여
$\sum_{y\in A}(y-\mu_{A^+})^2 = \text{SSE}*A + m(\mu_A-\mu*{A^+})^2$ 임을 계산하면

$$
\text{SSE}_{A^+}
= mV_A + m\Bigl(\tfrac{x-\mu_A}{m+1}\Bigr)^2 + (x-\mu_{A^+})^2
= mV_A + \frac{(x-\mu_A)^2}{m+1}.
$$

따라서

$$
V_{A^+} 
= \frac{\text{SSE}_{A^+}}{|A^+|}
= \frac{m}{m+1}V_A \;+\; \frac{(x-\mu_A)^2}{m+1}.
$$

#### (b) 원소 **제거**

집합 $B$의 크기를 $|B|=n(!\ge 2)$, 평균을 $\mu_B$, 분산을 $V_B$라 하고
원소 $x\in B$를 제거하여 $B^-=B\setminus{x}$를 만들면 $|B^-|=n-1$이다.
새 평균은

$$
\mu_{B^-} \;=\; \frac{n\mu_B - x}{n-1}.
$$

마찬가지로 $\text{SSE}_{B^-}$를 전개하면

$$
\text{SSE}_{B^-}
= \text{SSE}_B - (x-\mu_B)^2 - (n-1)(\mu_B-\mu_{B^-})^2,
$$

여기서 $\mu_B-\mu_{B^-} = \dfrac{x-\mu_B}{,n-1,}$이므로

$$
\text{SSE}_{B^-}
= nV_B - (x-\mu_B)^2 - \frac{(x-\mu_B)^2}{\,n-1\,}
= nV_B - \frac{n}{\,n-1\,}(x-\mu_B)^2.
$$

따라서

$$
V_{B^-}
= \frac{\text{SSE}_{B^-}}{|B^-|}
= \frac{n}{\,n-1\,}V_B \;-\; \frac{(x-\mu_B)^2}{\,n-1\,}.
$$

이 두 식을 이용하면 교환 분할 후 목적 함수 변화량 $\Delta L$을 식 (1)로 바로 계산할 수 있다.
