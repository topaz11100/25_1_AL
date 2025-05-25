/*
학번 : 2020203090
이름 : 한용옥
언어 : `c++20`
컴파일러 및 IDE : Microsoft Visual Studio Community 2022 버전 17.13.6
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <ranges>
#include <numeric>
#include <limits>
#include <iomanip>
#include <functional>

using namespace std;
using student = pair<int, int>;

void method1(vector<student> s_v, int k)
{
    //내림차순 정렬
    ranges::sort(s_v, greater(), &student::second);

    //연속 값의 차이를 diff에 저장
    //가르는 인덱스는 그룹의 끝 원소 인덱스 + 1
    vector<student> diff;
    for (int i = 0; i < s_v.size() - 1; i += 1)
    {
        int s_v_diff = s_v[i].second - s_v[i + 1].second;
        diff.push_back(student{ i + 1, s_v_diff });
    }

    //그룹 차이 최대화이므로 상위 k_1개 정렬
    int k_1 = k - 1 < diff.size() ? k - 1 : diff.size();
    ranges::partial_sort(diff, diff.begin() + k_1, greater(), &student::second);

    //k_1개 분할 인덱스 저장
    vector<int> end_idx{};
    for (int i = 0; i < k_1; i += 1)
        end_idx.push_back(diff[i].first);

    //경계조건 추가, 분할 인덱스 정렬
    end_idx.push_back(0);
    end_idx.push_back(s_v.size());
    sort(end_idx.begin(), end_idx.end());

    //결과 생성 및 반환
    int sum = accumulate(diff.begin(), diff.begin() + k_1, 0,
                         [](int acc, const student& p) { return acc + p.second; });
    //학번 오름차순 정렬
    for (int i = 0; i < end_idx.size() - 1; i += 1)
        ranges::sort(s_v.begin() + end_idx[i],
                     s_v.begin() + end_idx[i + 1],
                     less(), &student::first);
    //출력부
    cout << sum << endl;
    ofstream out{ "Partition1.txt" };
    for (int i = 0; i < end_idx.size() - 1; i += 1)
    {
        for (int j = end_idx[i]; j < end_idx[i + 1]; j += 1)
            out << s_v[j].first << "(" << s_v[j].second << ") ";
        out << endl;
    }
}

//누적합 배열 이용, [start, end] 구간 분산 구함
double var(int start, int end, const vector<long long>& sum, const vector<long long>& sum2)
{
    if (start == end) return 0;
    long long size = (long long)(end - start + 1);
    long long before_s = start > 0 ? sum[start - 1] : 0;
    long long before_s2 = start > 0 ? sum2[start - 1] : 0;
    long long upper = size * (sum2[end] - before_s2) - (sum[end] - before_s) * (sum[end] - before_s);
    return (double)upper / (double)(size * size);
}

//메모이제이션
void method2_recur(vector<vector<double>>& V,
                   int k, int i,
                   vector<vector<int>>& T,
                   const vector<long long>& sum, const vector<long long>& sum2)
{
    //계산되어있으면 바로 종료
    if (V[k][i] >= 0) return;

    //점화식
    double result = numeric_limits<double>::infinity();
    int opt = 0;
    for (int t = k - 1; t < i; t += 1)
    {
        //점화식 적용전 해당 값이 계산되어야함
        method2_recur(V, k - 1, t, T, sum, sum2);
        double temp = V[k - 1][t] + var(t + 1, i, sum, sum2);
        if (temp <= result)
        {
            result = temp;
            opt = t;
        }
    }
    //배열 값 갱신
    T[k][i] = opt;
    V[k][i] = result;
}

void track_opt_idx(const vector<vector<int>>& T, int k, int i, vector<int>& result)
{
    if (k <= 0) return;
    //재귀추적(구간 정렬 위해 구간 끝 원소의 다음 인덱스 저장)
    int opt = T[k][i];
    result.push_back(opt + 1);
    track_opt_idx(T, k - 1, opt, result);
}

void method2(vector<student> s_v, int k)
{
    //내림차순 정렬
    ranges::sort(s_v, greater(), &student::second);

    int size = s_v.size();
    //분산 계산 위한 누적합 배열 초기화
    vector<long long> sum(size);
    vector<long long> sum2(size);
    sum[0] = (long long)s_v[0].second;
    sum2[0] = (long long)(s_v[0].second * s_v[0].second);
    for (int i = 1; i < size; i += 1)
    {
        sum[i]  = sum[i - 1] + (long long)s_v[i].second;
        sum2[i] = sum2[i - 1] + (long long)(s_v[i].second * s_v[i].second);
    }

    //최적값 배열
    vector<vector<double>> V(k, vector<double>(size, -1));
    //인덱스 추적 배열
    vector<vector<int>> T(k, vector<int>(size, -1));
    //경곗값 처리 (k=0은 분할 안하므로 [0, i]의 분산)
    for (int i = 0; i < size; i += 1)
        V[0][i] = var(0, i, sum, sum2);

    //동적계획법 메모이제이션 구현
    method2_recur(V, k - 1, size - 1, T, sum, sum2);

    //출력부
    //분산 최소 출력(소수점 3자리표현 - 반올림)
    cout << fixed << setprecision(3) << V[k - 1][size - 1] << endl;
    //학번 순 그룹 짓기
    //최적 인덱스 저장
    vector<int> opt_idx{ size, 0 };
    track_opt_idx(T, k - 1, size - 1, opt_idx);
    sort(opt_idx.begin(), opt_idx.end());
    //학번 오름차순 정렬
    for (int i = 0; i < opt_idx.size() - 1; i += 1)
        ranges::sort(s_v.begin() + opt_idx[i],
                     s_v.begin() + opt_idx[i + 1],
                     less(), &student::first);
    //파일 출력
    ofstream out{ "Partition2.txt" };
    for (int i = 0; i < opt_idx.size() - 1; i += 1)
    {
        for (int j = opt_idx[i]; j < opt_idx[i + 1]; j += 1)
            out << s_v[j].first << "(" << s_v[j].second << ") ";
        out << endl;
    }
}

int main()
{
    //입력 처리
    int n, k, temp;
    cin >> n >> k;
    vector<student> s_v;
    for (int i = 0; i < n; i += 1)
    {
        cin >> temp;
        s_v.push_back(pair{ i + 1, temp });
    }

    //함수 실행
    method1(s_v, k);
    method2(s_v, k);

    return 0;
}