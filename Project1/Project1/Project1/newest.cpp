#include <iostream>
#include <string>

using namespace std;

void walk()
{
	cout << "walk" << endl;
}

void idle()
{
	cout << "idle" << endl;
}

void run()
{
	cout << "Bolt!!" << endl;
}


int main()
{
	char op;
	string state = "idle";


	do {
		cin >> op;

		if (state == "idle")
		{
			idle();
			if (op == 'f' || op == 'F')
				state = "walk";
		}
		else if (state == "walk")
		{
			walk();
			if (op == 'F')
				state = "run";
			else if (op == 'f')
			{
			}
			else
				state = "idle";

		}
		else if (state == "run")
		{
			run();
			if (op != 'F')
				state = "walk";
		}

	} while (op != 'q');


	return 0;
}