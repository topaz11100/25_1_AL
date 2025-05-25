/*
2020203090
한용옥
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
using namespace std;

/*
배열 저장 순서 0:^ 1:< 2:v 3:> 북, 서, 남, 동
*/

const vector<string> paint{ "A+", "B+", "C+", "D+",
							"A-", "B-", "C-", "D-",
							"A*", "B*", "C*", "D*",
							"A/", "B/", "C/", "D/" };
bool paint_pair(const string& a, const string& b)
{
	return a[0] == b[0] && a[1] != b[1];
}
enum direction
{// 상태 저장 순서
	N = 0,
	W = 1,
	S = 2,
	E = 3
};

class piece
{
public:
	piece(int i, const vector<int>& num)
	{
		number = i;
		shift_count = 0;
		for (int i = 0; i < 4; i += 1)
		{
			state.push_back(paint[num[i]]);
		}
	}
	int get_number() const { return number; }
	int get_shift_count() const { return shift_count; }
	string get_state(const direction& num) const
	{
		return state[num];
	}
	void shift()
	{//반시계 회전
		shift_count = (shift_count + 1) % 4;
		string end = state[0];
		for (int i = 0; i < 2;i += 1)
		{
			state[i] = state[i + 1];
		}
		state[4] = end;
	}
private:
	int number;
	int shift_count;
	vector<string> state;
};

vector<piece*> piece_vec;
vector<vector<piece*>> puzzle;

pair<int, int> num_to_XY(int num)
{
	int row = num / 3;
	int col = num % 3;
	return pair<int, int>{row, col};
}

bool promising(const vector<vector<piece*>>& puzzle,
			   piece* target, int num)
{
	//들어가는 퍼즐 조각 - target 기준
	//target의 좌표
	int row = num / 3, col = num % 3;
	//처음은 그냥 통과
	if (num == 0) return true;
	//왼쪽만 보면 되는 경우
	else if (num == 1 || num == 2)
	{
		piece* compare = puzzle[row][col - 1];
		return paint_pair(target->get_state(direction::W), compare->get_state(direction::E));
	}
	//위쪽만 보면 되는 경우
	else if (num == 3 || num == 6)
	{
		piece* compare = puzzle[row - 1][col];
		return paint_pair(target->get_state(direction::N), compare->get_state(direction::S));
	}
	//나머지는 왼쪽, 위쪽 다 봐야함
	else
	{
		piece* compare1 = puzzle[row][col - 1];
		piece* compare2 = puzzle[row - 1][col];
		return paint_pair(target->get_state(direction::W), compare1->get_state(direction::E)) &&
			   paint_pair(target->get_state(direction::N), compare2->get_state(direction::S));
	}
}

void input_process()
{
	string prompt = "퍼즐 모양 입력\n"
					"북, 서, 남, 동 반시계 방향으로 맞는 그림 번호 입력\n"
				    " 0 : A+   1 : B+   2 : C+   3 : D+\n"
				    " 4 : A-   5 : B-   6 : C-   7 : D-\n"
				    " 8 : A*   9 : B*  10 : C*  11 : D*\n"
				    "12 : A/  13 : B/  14 : C/  15 : D/\n";
	cout << prompt;

	vector<vector<int>> input_result;

	for (int i = 0; i < 9; i += 1)
	{
		cout << i << " 번째 퍼즐 입력 = ";
		vector<int> temp;
		int t;
		for (int _ = 0; _ < 4; _ += 1)
		{
			cin >> t;
			temp.push_back(t);
		}
		input_result.push_back(temp);
	}
	for (int i = 0; i < 9; i += 1)
	{
		piece_vec.push_back(new piece{ i, input_result[i]});
	}
}

void print_puzzle(const vector<piece*>& v)
{
	cout << "\n퍼즐 풀이\n(a,b) = (퍼즐 번호, 반시계 90도 회전한 횟수)\n\n";

	for (int row = 0; row < 3; ++row)
	{
		// 첫 줄: 북쪽 방향 출력
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << "--- " << p->get_state(N) << "  --- ";
		}
		cout << "\n";

		// 둘째 줄: 넘버 출력
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << "|         | ";
		}
		cout << "\n";

		// 셋째 줄: 서쪽 동쪽
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << p->get_state(W) << " (" << p->get_number() << "," << p->get_shift_count() << ") " << p->get_state(E) << " ";
		}
		cout << "\n";

		// 넷째 줄: 시프트 수
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << "|         | ";
		}
		cout << "\n";

		// 다섯째 줄: 남쪽
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << "--- " << p->get_state(S) << "  --- ";
		}
		cout << "\n";
	}
}

int main()
{
	input_process();
	print_puzzle(piece_vec);
	
	for (auto p : piece_vec)
		delete p;

	return 0;
}