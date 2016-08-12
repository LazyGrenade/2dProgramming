#include "Source.h"
#include <string>
#include <vector>
#include <iostream>

Source::Source()
{
}


Source::~Source()
{
}

enum Key {
	U, D, L, R, //up, down, left, right
	P, // punch
	K // kick
};

typedef std::vector<Key> Keys;
typedef std::pair<Keys, std::string> Combo;
typedef std::vector<Combo> Combos;

bool match(const Keys& input, const Keys& move, int threshold = 1)
{
	int input_size = input.size();
	int move_size = move.size();

	if (input_size < move_size)
		return false;

	int mismatch = 0;
	int j = input_size - 1;
	for (int i = move_size - 1; i >= 0; --i, --j)
	{
		if (input[j] != move[i])
			++mismatch;
		else
			mismatch = 0;

		if (mismatch >= threshold)
			return false;

		if (mismatch >= threshold)
			return false;
	}
	return true;
}

//bool match(const keys& input, const keys& move, int threshold)
//{
//	int input_size = input.size();
//	int move_size = move.size();
//
//	if (input_size < move_size)
//		return false;
//
//	int j = input_size - 1;
//	for (int i = move_size - 1; i >= 0; --i, --j)
//	{
//		if (input[j] != move[i])
//			return false;
//	}
//	return true;
//}

int main()
{
	const Keys r_fireball = { D, R, P };
	const Keys r_dragonpunch = { R, D, R, P };
	const Keys r_hurricanekick = { D, R, K };

	const Combos comboList = {
		Combo(r_fireball, "FireBall"),
		Combo(r_dragonpunch, "DragonPunch"),
		Combo(r_hurricanekick, "HurricaneKick")
	};

	Keys input_history;

	char input;
	do {
		std::cin >> input;


		switch (input)
		{
		case 'w':
			input_history.push_back(Key::U);
			break;
		case 'a':
			input_history.push_back(Key::L);
			break;
		case 's':
			input_history.push_back(Key::D);
			break;
		case 'd':
			input_history.push_back(Key::R);
			break;
		case 'p':
			input_history.push_back(Key::P);
			break;
		case 'k':
			input_history.push_back(Key::K);
			break;
		}
	} while (input != 0);

	if (match(input_history, r_dragonpunch, 1))
	{
		std::cout << "SHOYUUUUKEN!!" << std::endl;
	}
	else if (match(input_history, r_fireball, 1))
	{
		std::cout << "HADOOOOKEN!!" << std::endl;
	}
	else if (match(input_history, r_hurricanekick, 1))
	{
		std::cout << "I'D LIKE SOME POUND CAKE!!" << std::endl;
	}
}


