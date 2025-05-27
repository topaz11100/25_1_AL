/*
2020203090
�ѿ��
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
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
	if (a[0] != b[0]) return false;
	bool check = (a[1] == '+' && b[1] == '-') ||
				 (a[1] == '-' && b[1] == '+') ||
				 (a[1] == '*' && b[1] == '/') ||
				 (a[1] == '/' && b[1] == '*');
	return check;
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
		for (int i = 0; i < 3;i += 1)
		{
			state[i] = state[i + 1];
		}
		state[3] = end;
	}
private:
	int number;
	int shift_count;
	vector<string> state;
};

bool puzzle_piece_promising(piece* target, int num, const vector<vector<piece*>>& puzzle)
{
	//ó���� �׳� ���
	if (num == 0) return true;
	
	//���� ���� ���� - target ����
	//target�� ��ǥ
	int row = num / 3, col = num % 3;
	
	//���ʸ� ���� �Ǵ� ���
	if (num == 1 || num == 2)
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

vector<piece*> piece_vec;
vector<vector<piece*>> puzzle(3, vector<piece*>(3));

bool backtracking(int puzzle_num, int remain_piece)
{
	//�� �ϼ��Ǹ� Ż��
	if (puzzle_num == 9) return true;

	//���� �����鿡 ����(��Ʈ ����ũ ���)
	for (int p_i = 0; p_i < 9; p_i += 1)
		if (remain_piece & (1 << p_i))
		{
			//���� promising �˻� �� ����
			piece* now = piece_vec[p_i];
			int row = puzzle_num / 3, col = puzzle_num % 3;
			//ȸ�� ���(ȸ�� 4���ؼ� �� ���·� ����)
			for (int shift = 0; shift < 4; shift += 1)
			{
				if (puzzle_piece_promising(now, puzzle_num, puzzle))
				{
					puzzle[row][col] = now;
					//�ش��ϴ� �� �ϳ��� ����
					int mask = remain_piece & ~(1 << p_i);
					if (backtracking(puzzle_num + 1, mask)) return true;
				}
				now->shift();
			}
			puzzle[row][col] = nullptr;
		}

	//����� ���� �ذ� ���°�
	return false;
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

void print_puzzle(const vector<vector<piece*>>& grid)
{
	cout << "\n���� Ǯ��\n(a,b) = (���� ��ȣ, �ݽð� 90�� ȸ���� Ƚ��)\n\n";

	for (int row = 0; row < 3; ++row)
	{
		// ù ��: ���� ����
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = grid[row][col];
			cout << "--- " << p->get_state(N) << "  --- ";
		}
		cout << "\n";

		// ��° ��: ���� ����
		for (int col = 0; col < 3; ++col)
			cout << "|         | ";
		cout << "\n";

		// ��° ��: ����, ��ȣ/����Ʈ, ����
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = grid[row][col];
			cout << p->get_state(W) << " (" << p->get_number() << "," << p->get_shift_count() << ") " << p->get_state(E) << " ";
		}
		cout << "\n";

		// ��° ��: �Ʒ��� ����
		for (int col = 0; col < 3; ++col)
			cout << "|         | ";
		cout << "\n";

		// �ټ�° ��: ���� ����
		for (int col = 0; col < 3; ++col)
		{
			const piece* p = grid[row][col];
			cout << "--- " << p->get_state(S) << "  --- ";
		}
		cout << "\n";
	}
}

int main()
{
	input_process();

	int init = 511;
	if (backtracking(0, init)) print_puzzle(puzzle);
	else cout << "�ذ� ����" << endl;	

	for (auto p : piece_vec)
		delete p;

	return 0;
}