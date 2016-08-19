#include <iostream>
#include <string>
#include <functional>
#include <map>

using namespace std;

enum States {
	DEFAULT,	// stay in the current state
	IDLE,
	WALK,
	RUN
};

States idle(char op)
{
	cout << "idle" << endl;
	if (op == 'f' || op == 'F')
		return States::WALK;

	return DEFAULT;
}

States walk(char op)
{
	cout << "walk" << endl;

	if (op == 'F')
		return RUN;
	else if (op == 'f')
		return DEFAULT;
	else
		return IDLE;
}

States run(char op)
{
	cout << "Bolt!!" << endl;
	if (op != 'F')
		return WALK;
	else
		return DEFAULT;
}


int main()
{
	char op;
	States state = States::IDLE;

	// typedef the function object to make it easier to read
	typedef std::function<States(char)> StateFunc;
	// typedef the map to make it easier to read
	typedef std::map<States, StateFunc> StateFuncMap;

	// Registering all functions in the FSM
	StateFuncMap fsm;
	fsm.emplace(IDLE, idle);
	fsm.emplace(WALK, walk);
	fsm.emplace(RUN, run);

	do {
		cin >> op;

		States new_state = fsm[state](op);

		state = new_state == DEFAULT ? state : new_state;

	} while (op != 'q');

	return 0;
}