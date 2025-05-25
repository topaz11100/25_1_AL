/*
2020203090
�ѿ��
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
using namespace std;

/*
�迭 ���� ���� 0:^ 1:< 2:v 3:> ��, ��, ��, ��
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
{// ���� ���� ����
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
	{//�ݽð� ȸ��
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
	//���� ���� ���� - target ����
	//target�� ��ǥ
	int row = num / 3, col = num % 3;
	//ó���� �׳� ���
	if (num == 0) return true;
	//���ʸ� ���� �Ǵ� ���
	else if (num == 1 || num == 2)
	{
		piece* compare = puzzle[row][col - 1];
		return paint_pair(target->get_state(direction::W), compare->get_state(direction::E));
	}
	//���ʸ� ���� �Ǵ� ���
	else if (num == 3 || num == 6)
	{
		piece* compare = puzzle[row - 1][col];
		return paint_pair(target->get_state(direction::N), compare->get_state(direction::S));
	}
	//�������� ����, ���� �� ������
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
	string prompt = "���� ��� �Է�\n"
					"��, ��, ��, �� �ݽð� �������� �´� �׸� ��ȣ �Է�\n"
				    " 0 : A+   1 : B+   2 : C+   3 : D+\n"
				    " 4 : A-   5 : B-   6 : C-   7 : D-\n"
				    " 8 : A*   9 : B*  10 : C*  11 : D*\n"
				    "12 : A/  13 : B/  14 : C/  15 : D/\n";
	cout << prompt;

	vector<vector<int>> input_result;

	for (int i = 0; i < 9; i += 1)
	{
		cout << i << " ��° ���� �Է� = ";
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
	cout << "\n���� Ǯ��\n(a,b) = (���� ��ȣ, �ݽð� 90�� ȸ���� Ƚ��)\n\n";

	for (int row = 0; row < 3; ++row)
	{
		// ù ��: ���� ���� ���
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << "--- " << p->get_state(N) << "  --- ";
		}
		cout << "\n";

		// ��° ��: �ѹ� ���
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << "|         | ";
		}
		cout << "\n";

		// ��° ��: ���� ����
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << p->get_state(W) << " (" << p->get_number() << "," << p->get_shift_count() << ") " << p->get_state(E) << " ";
		}
		cout << "\n";

		// ��° ��: ����Ʈ ��
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = v[row * 3 + col];
			cout << "|         | ";
		}
		cout << "\n";

		// �ټ�° ��: ����
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