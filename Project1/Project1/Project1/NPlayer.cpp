#include "NPlayer.h"
#include <iostream>
#include <memory>
#include <functional>

using namespace std;

#define ASSERT(condition, message)\
condition ? condition \
			: throw std::runtime_error(message)

#define PLAYER_BIND_STATE(func) \
	std::bind(&Player::func, this, placeholders::_1)

void Player::CreateFSM()
{
	fsm["idle"] = PLAYER_BIND_STATE(idleState);
	fsm["walk"] = PLAYER_BIND_STATE(walkState);
	fsm["run"] = PLAYER_BIND_STATE(runState);
}


void Player::Update(char op)
{
	if (fsm.find(state) == fsm.end())
		throw runtime_error("state no found");
	string new_state = fsm[state](op);
	state = new_state.empty() ? state : new_state;

}

string Player::idleState(char op)
{
	cout << "idle" << endl;
	if (op == 'f' || op == 'F')
		return "walk";

	return "";
}

string Player::walkState(char op)
{
	cout << "walk" << endl;

	if (op == 'F')
		return "run";
	else if (op == 'f')
		return "";
	else
		return "idle";
}

string Player::runState(char op)
{
	cout << "Bolt!!" << endl;
	if (op != 'F')
		return "walk";

	return "";
}
