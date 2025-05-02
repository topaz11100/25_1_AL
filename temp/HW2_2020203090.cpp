#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <numeric>
#include <unordered_set>

using namespace std;

/*
vector 의 원소는 pair{학번순서, 점수}
입력 함수는 pair{점수배열, 그룹 수} 반환
*/

using snum_score = pair<int, int>;

void print(ostream& out, const vector<snum_score>& vec)
{
    for (const auto& v : vec)
    {
        out << v.first << "(" << v.second << ") ";
    }
    out << endl;
}

//점수 내림차순 정렬
bool compare_score(const snum_score& a, const snum_score& b)
{
    return a.second > b.second;
}

int add_score(int sum, const snum_score& p)
{
    return sum + p.second;
}

pair<vector<snum_score>, int> input()
{
    int n, k, temp;
    cin >> n >> k;
    vector<snum_score> score;
    for (int i = 0; i < n; i += 1)
    {
        cin >> temp;
        score.push_back(pair{ i + 1, temp });
    }
    return pair{score, k};
}

pair<int, vector<vector<snum_score>>> method_1(const vector<snum_score>& score, int k)
{
    //값이 차이나면 다른 데이터 그룹 간주, diff에 저장
    vector<snum_score> diff;
    for (int i = 1; i < score.size(); i += 1)
    {
        int score_diff = score[i - 1].second - score[i].second;
        if (score_diff == 0) continue;
        diff.push_back(snum_score{ i, score_diff });
    }

    //diff에서 k-1개 그룹으로 병합
    //상위 k개 정렬
    int k_1 = diff.size() > k - 1 ? k - 1 : diff.size();
    partial_sort(diff.begin(), diff.begin() + k_1, diff.end(), compare_score);

    //k-1개 분할 인덱스 저장
    vector<int> start_index{ 0 };
    for (int i = 0; i < k_1; i += 1)
        start_index.push_back(diff[i].first);

    //더 필요하면(이 상황은 같은 점수 배열을 나눠야하므로 어디가 분할되던 최대는 불변이다)
    if (k > start_index.size())
        for (int i = 0; i < score.size() && start_index.size() < k; i += 1)
            if (find(start_index.begin(), start_index.end(), i) == start_index.end())
                start_index.push_back(i);

    //정렬
    sort(start_index.begin(), start_index.end());
    start_index.push_back(score.size());

    //결과 생성 및 반환
    int sum = accumulate(diff.begin(), diff.begin() + k_1, 0, add_score);
    vector<vector<snum_score>> result;
    for (int i = 1; i < start_index.size(); i += 1)
        result.push_back(vector<snum_score>(score.begin() + start_index[i - 1], score.begin() + start_index[i]));

    return pair{ sum, result };
}

/*
pair<int, vector<vector<snum_score>>> method_2(const vector<snum_score>& score, int k)
{

}
*/

int main()
{
    //입력 처리 및 정렬
    auto data = input();
    //print(cout, data.first);
    stable_sort(data.first.begin(), data.first.end(), compare_score);

    //메소드 1, 2
    pair<int, vector<vector<snum_score>>> result_method_1 = method_1(data.first, data.second);
    //pair<int, vector<vector<snum_score>>> result_method_2 = method_2(data.first, data.second);

    //출력 처리
    cout << result_method_1.first << endl;
    //cout << result_method_2.first << endl;

    ofstream f_m1{ "Partition1.txt" };
    for (const auto& score : result_method_1.second)
        print(f_m1, score);

    /*
    ofstream f_m2{ "Partition2.txt" };
    for (const auto& score : result_method_2.second)
        print(f_m2, score);
    */
    return 0;
}
