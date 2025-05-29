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
	//처음은 그냥 통과
	if (num == 0) return true;

	//들어가는 퍼즐 조각 - target 기준
	//target의 좌표
	size_t row = num / 3, col = num % 3;

	//왼쪽만 보면 되는 경우
	if (num == 1 || num == 2)
	{
		piece* compare = puzzle[row][col - 1];
		return piece_pair_promising(target->get_state(W), compare->get_state(E));
	}
	//위쪽만 보면 되는 경우
	else if (num == 3 || num == 6)
	{
		piece* compare = puzzle[row - 1][col];
		return piece_pair_promising(target->get_state(N), compare->get_state(S));
	}
	//나머지는 왼쪽, 위쪽 다 봐야함
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
	//해 완성되면 탈출
	if (puzzle_num == 9) return true;

	//남은 조각들에 대해(비트 마스크 사용)
	for (int p_i = 0; p_i < PIECE_COUNT; p_i += 1)
		if (remain_piece & (1 << p_i))
		{
			//조각 promising 검사 후 대입
			piece* now = &piece_arr[p_i];
			size_t row = puzzle_num / 3, col = puzzle_num % 3;
			//회전 고려(회전 4번해서 원 상태로 만듬)
			for (int shift = 0; shift < 4; shift += 1)
			{
				if (puzzle_piece_promising(now, puzzle_num))
				{
					//해 할당
					puzzle[row][col] = now;
					//해당하는 해 하나만 보임
					int mask = remain_piece & ~(1 << p_i);
					if (backtracking(puzzle_num + 1, mask)) return true;
					//상태 원복 (여기 오면 들어간 곳이 해가 없다는 뜻이므로)
					puzzle[row][col] = nullptr;
				}
				now->shift();
			}
		}

	//여기로 오면 해가 없는것
	return false;
}

void input_process()
{
	string prompt =
		"퍼즐 모양 입력\n"
		"\n"
		"입력 조건\n"
		"1. 조각은 9개이고 각 조각은 4개의 그림을 가지므로\n"
		"   9*4 행렬 입력\n"
		"\n"
		"2. 각 항은 (그림종류)(분할기호) 로 이뤄짐 예시 A*\n"
		"   위 아래로 나뉘어진 그림의 위 = +, 아래 = -\n"
		"   좌우로 나뉘어진 그림의 좌 = /, 우 = *\n"
		"\n"
		"3. 열 순서는 북, 서, 남, 동 반시계 방향\n"
		"\n"
		"4. 줄 바꿈된 행렬 텍스트를 한 번에 입력\n"
		"\n"
		"<입력예시 - ppt 첫 번째 퍼즐 - 아래 전체를 복붙>\n"
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
	std::cout << "\n퍼즐 \n(a,b) = (퍼즐 번호, 반시계 90도 회전한 횟수)\n\n";

	const piece* p;
	array<char, 2> s;

	for (int row = 0; row < 3; ++row)
	{
		// 첫 줄: 북쪽 방향
		for (int col = 0; col < 3; ++col)
		{
			p = puzzle[row][col];
			s = p->get_state(N);
			std::cout << "--- " << s[0] << s[1] << "  --- ";
		}
		std::cout << "\n";

		// 둘째 줄: 위쪽 여백
		for (int col = 0; col < 3; ++col)
			std::cout << "|         | ";
		std::cout << "\n";

		// 셋째 줄: 서쪽, 번호/시프트, 동쪽
		for (int col = 0; col < 3; ++col)
		{
			p = puzzle[row][col];
			s = p->get_state(W);
			std::cout << s[0] << s[1] << " (" << p->get_number() << "," << p->get_shift_count() << ") ";
			s = p->get_state(E);
			std::cout << s[0] << s[1] << " ";
		}
		std::cout << "\n";

		// 넷째 줄: 아래쪽 여백
		for (int col = 0; col < 3; ++col)
			std::cout << "|         | ";
		std::cout << "\n";

		// 다섯째 줄: 남쪽 방향
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
	std::cout << "\n입력 퍼즐 표시\n";
	input_print();
	
	int init = 511;
	if (backtracking(0, init))
	{
		std::cout << "\n퍼즐 풀이\n";
		print_puzzle();
	}
	else
	{
		std::cout << "\n해가 없다\n" << endl;
	}

	
	return 0;
}