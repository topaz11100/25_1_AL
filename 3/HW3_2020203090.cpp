#include <iostream>
#include <array>
#include <string>
#include <sstream>

using namespace std;

constexpr size_t N = 0;
constexpr size_t W = 1;
constexpr size_t S = 2;
constexpr size_t E = 3;

constexpr size_t PUZ_ROW = 3;
constexpr size_t PUZ_COL = 3;

constexpr size_t PIECE_COUNT = 9;
constexpr size_t PAINT_COUNT = 16;
constexpr size_t PAINT_TYPE = 2;
constexpr size_t PAINT_CONTAIN = 4;
constexpr size_t SHIFT_TYPE = 4;

class piece
{
public:

	piece() :shift_count{ 0 } {}

	void set_number(int n) { number = n; }

	void set_state(size_t s_idx, char first, char second)
	{
		for (size_t shift = 0; shift < SHIFT_TYPE; shift += 1)
		{
			size_t col = (s_idx + shift) % PAINT_CONTAIN;
			state[shift][col][0] = first;
			state[shift][col][1] = second;
		}
	}

	int get_number() const { return number; }
	size_t get_shift_count() const { return shift_count; }
	const array<char, PAINT_TYPE>& get_state(size_t direction) const
	{
		return state[shift_count][direction];
	}

	void shift() { shift_count = (shift_count + 1) % SHIFT_TYPE; }
private:
	int number;
	size_t shift_count;
	array<array<array<char, PAINT_TYPE>, PAINT_CONTAIN>, SHIFT_TYPE> state;
};

array<piece, PIECE_COUNT> piece_arr;
array<array<piece*, PUZ_COL>, PUZ_ROW> puzzle;

bool piece_pair_promising(const array<char, PAINT_TYPE>& a, const array<char, PAINT_TYPE>& b)
{
	if (a[0] != b[0]) return false;
	bool check = (a[1] == '+' && b[1] == '-') ||
				 (a[1] == '-' && b[1] == '+') ||
				 (a[1] == '*' && b[1] == '/') ||
				 (a[1] == '/' && b[1] == '*');
	return check;
}

bool puzzle_piece_promising(piece* target, int num)
{
	//ó���� �׳� ���
	if (num == 0) return true;

	//���� ���� ���� - target ����
	//target�� ��ǥ
	size_t row = num / 3, col = num % 3;

	//���ʸ� ���� �Ǵ� ���
	if (num == 1 || num == 2)
	{
		piece* compare = puzzle[row][col - 1];
		return piece_pair_promising(target->get_state(W), compare->get_state(E));
	}
	//���ʸ� ���� �Ǵ� ���
	else if (num == 3 || num == 6)
	{
		piece* compare = puzzle[row - 1][col];
		return piece_pair_promising(target->get_state(N), compare->get_state(S));
	}
	//�������� ����, ���� �� ������
	else
	{
		piece* compare1 = puzzle[row][col - 1];
		piece* compare2 = puzzle[row - 1][col];
		return piece_pair_promising(target->get_state(W), compare1->get_state(E)) &&
			   piece_pair_promising(target->get_state(N), compare2->get_state(S));
	}
}

bool backtracking(int puzzle_num, int remain_piece)
{
	//�� �ϼ��Ǹ� Ż��
	if (puzzle_num == 9) return true;

	//���� �����鿡 ����(��Ʈ ����ũ ���)
	for (int p_i = 0; p_i < PIECE_COUNT; p_i += 1)
		if (remain_piece & (1 << p_i))
		{
			//���� promising �˻� �� ����
			piece* now = &piece_arr[p_i];
			size_t row = puzzle_num / 3, col = puzzle_num % 3;
			//ȸ�� ���(ȸ�� 4���ؼ� �� ���·� ����)
			for (int shift = 0; shift < 4; shift += 1)
			{
				if (puzzle_piece_promising(now, puzzle_num))
				{
					//�� �Ҵ�
					puzzle[row][col] = now;
					//�ش��ϴ� �� �ϳ��� ����
					int mask = remain_piece & ~(1 << p_i);
					if (backtracking(puzzle_num + 1, mask)) return true;
					//���� ���� (���� ���� �� ���� �ذ� ���ٴ� ���̹Ƿ�)
					puzzle[row][col] = nullptr;
				}
				now->shift();
			}
		}

	//����� ���� �ذ� ���°�
	return false;
}

void input_process()
{
	string prompt =
		"���� ��� �Է�\n"
		"\n"
		"�Է� ����\n"
		"1. ������ 9���̰� �� ������ 4���� �׸��� �����Ƿ�\n"
		"   9*4 ��� �Է�\n"
		"\n"
		"2. �� ���� (�׸�����)(���ұ�ȣ) �� �̷��� ���� A*\n"
		"   �� �Ʒ��� �������� �׸��� �� = +, �Ʒ� = -\n"
		"   �¿�� �������� �׸��� �� = /, �� = *\n"
		"\n"
		"3. �� ������ ��, ��, ��, �� �ݽð� ����\n"
		"\n"
		"4. �� �ٲ޵� ��� �ؽ�Ʈ�� �� ���� �Է�\n"
		"\n"
		"<�Է¿��� - ppt ù ��° ���� - �Ʒ� ��ü�� ����>\n"
		"A* B* B/ C*\n"
		"B/ A* C/ C*\n"
		"C* A/ B/ B*\n"
		"A* C* B/ B*\n"
		"D+ C/ A* B/\n"
		"D+ A/ C/ D-\n"
		"D- A* A/ C/\n"
		"A* B/ C* D-\n"
		"D+ A/ A* B*\n"
		"\n"
		"\n";

	std::cout << prompt;

	string input;
	istringstream iss;
	string token;
	
	for (size_t p = 0; p < PIECE_COUNT; p += 1)
	{
		getline(cin, input);
		iss.clear();
		iss.str(input);
		piece_arr[p].set_number(p);

		for (size_t i = 0; i < PAINT_CONTAIN; i += 1)
		{
			iss >> token;
			piece_arr[p].set_state(i, token[0], token[1]);
		}
	}
}

void print_puzzle()
{
	std::cout << "\n���� \n(a,b) = (���� ��ȣ, �ݽð� 90�� ȸ���� Ƚ��)\n\n";

	const piece* p;
	array<char, 2> s;

	for (int row = 0; row < 3; ++row)
	{
		// ù ��: ���� ����
		for (int col = 0; col < 3; ++col)
		{
			p = puzzle[row][col];
			s = p->get_state(N);
			std::cout << "--- " << s[0] << s[1] << "  --- ";
		}
		std::cout << "\n";

		// ��° ��: ���� ����
		for (int col = 0; col < 3; ++col)
			std::cout << "|         | ";
		std::cout << "\n";

		// ��° ��: ����, ��ȣ/����Ʈ, ����
		for (int col = 0; col < 3; ++col)
		{
			p = puzzle[row][col];
			s = p->get_state(W);
			std::cout << s[0] << s[1] << " (" << p->get_number() << "," << p->get_shift_count() << ") ";
			s = p->get_state(E);
			std::cout << s[0] << s[1] << " ";
		}
		std::cout << "\n";

		// ��° ��: �Ʒ��� ����
		for (int col = 0; col < 3; ++col)
			std::cout << "|         | ";
		std::cout << "\n";

		// �ټ�° ��: ���� ����
		for (int col = 0; col < 3; ++col)
		{
			p = puzzle[row][col];
			s = p->get_state(S);
			std::cout << "--- " << s[0] << s[1] << "  --- ";
		}
		std::cout << "\n";
	}
}

void input_print()
{
	for (size_t r = 0; r < PUZ_ROW; r += 1)
		for (size_t c = 0; c < PUZ_COL; c += 1)
		{
			puzzle[r][c] = &piece_arr[PUZ_COL * r + c];
		}

	print_puzzle();

	for (size_t r = 0; r < PUZ_ROW; r += 1)
		for (size_t c = 0; c < PUZ_COL; c += 1)
		{
			puzzle[r][c] = nullptr;
		}
}

int main()
{
	input_process();
	std::cout << "\n�Է� ���� ǥ��\n";
	input_print();
	
	int init = 511;
	if (backtracking(0, init))
	{
		std::cout << "\n���� Ǯ��\n";
		print_puzzle();
	}
	else
	{
		std::cout << "\n�ذ� ����\n" << endl;
	}

	
	return 0;
}