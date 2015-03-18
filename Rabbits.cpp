#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <exception>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <ctime>

using namespace std;

typedef pair <int, int> pii;
typedef long long int64;

class cell{
public:
	bool populated;
	bool modifed;
	void setNew(bool populated, bool modifed) {
		cell::populated = populated;
		cell::modifed = modifed;
	}
	cell() {
		cell::populated = false;
		cell::modifed = false;
	}
	void refresh() {
		modifed = true;
		populated = !populated;
	}
	
};

int sizeX, sizeY, numCells, finalStep, borned = 0, died = 0, alive = 0, commonBorned = 0, commonDied = 0, stableStep = -1;
char usersAnswer;
vector <vector <cell> > field;
vector <vector <cell> > newField;
vector <pii> freeCells;
set <pii> freeCellsUser;

int inputReader(int currentInt) {
	try
	{
		if (!(cin >> currentInt)) throw "\nIncorrect input, try again!\n";
	}
	catch (char *str_exception)
	{
		cout << str_exception << endl;
		currentInt = -1;
		while (cin.get() != '\n') cin.clear();
	}
	if (currentInt > 1000000) currentInt = -1;
	return currentInt;
}

void fieldRefresh(int x, int y, bool firstTime) {
	if (firstTime) {
		field.resize(x);
		for (int i = 0; i < (int)field.size(); ++i) field[i].resize(y);
	}
	else {
		newField.resize(x);
		for (int i = 0; i < (int)newField.size(); ++i) newField[i].resize(y);
		for (int i = 0; i < min((int)newField.size(), (int)field.size()); ++i)
		for (int j = 0; j < min((int)newField.size(), (int)field.size()); ++j) {
			newField[i][j] = field[i][j];
		}
		field.clear();
		field = newField;
		newField.clear();
	}
}

void fieldShift() {
	newField.resize((int)field.size());
	for (int i = 0; i < (int)newField.size(); ++i) newField[i].resize((int)field[i].size());
	for (int i = 0; i < (int)field.size(); ++i)
	for (int j = 0; j < (int)field[i].size(); ++j) {
		if (field[i][j].populated) {
			newField[i + 10][j + 10] = field[i][j];
		}
	}
	field = newField;
	newField.clear();

}

void borderEditor() {
	int x, y;
	bool shift = false;
	for (int i = 0; i < (int)field.size(); ++i)
	for (int j = 0; j < (int)field[i].size(); ++j) {
		if (field[i][j].populated) {
			x = max(x, i);
			y = max(y, j);
			if (!i || !j) shift = true;
		}
	}
	x += 20;
	y += 20;
	fieldRefresh(x, y, false);
	if (shift) fieldShift();
}

int lookAround(int i, int j) {
	int ans = 0;
	for (int i1 = -1; i1 <= 1; ++i1)
	for (int j1 = -1; j1 <= 1; ++j1) {
		if (!j1 && !i1) continue;
		if (i + i1 < 0 || j + j1 < 0) continue;
		if (i + i1 >= (int)field.size() || j + j1 >= (int)field[i].size()) continue;
		if (!field[i + i1][j + j1].modifed && field[i + i1][j + j1].populated || field[i +i1][j + j1].modifed && !field[i + i1][j +j1].populated) ++ans;
	}
	return ans;
}

int gcd(int x, int y) {
	while (y) {
		x %= y;
		swap(x, y);
	}
	return x;
}

void lifeImitation() {
	for (int i = 0; i < (int)field.size(); ++i)
	for (int j = 0; j < (int)field[i].size(); ++j) {
		if ((!i || !j || i == (int)field.size() - 1 || j == (int)field[i].size() - 1) && field[i][j].populated)
		{
			borderEditor();
			lifeImitation();
			return;
		}
		int aroundPopulation = lookAround(i, j);
		if (aroundPopulation == 2) continue;
		if (aroundPopulation == 3) {
			if (!field[i][j].populated) {
				field[i][j].refresh();
				borned++;
				commonBorned++;
			}
			continue;
		}
		if (field[i][j].populated) {
			field[i][j].refresh();
			died++;
			commonDied++;
		}
	}
	int curAlive = 0;
	for (int i = 0; i < (int)field.size(); ++i)
	for (int j = 0; j < (int)field[i].size(); ++j) {
		if (field[i][j].populated) curAlive++;
		field[i][j].modifed = false;
	}
	alive = curAlive;
}

void initFirst() {
	sizeX = 0;
	sizeY = 0;
	numCells = 0;
	finalStep = 0;
	borned = 0;
	died = 0;
	alive = 0;
	commonBorned = 0;
	commonDied = 0;
	usersAnswer = '\n';
	field.clear();
	newField.clear();
	freeCells.clear();
	freeCellsUser.clear();
}

void initSecond() {
	for (int i = 0; i < sizeX; ++i) {
		for (int j = 0; j < sizeY; ++j) {
			freeCells.push_back(pii(i, j));
		}
	}
}

int main() {
	srand((unsigned int)time(0));
	while (true)
	{
		initFirst();
		printf("Input size of the field:\nHeight: ");
		if ((sizeX = inputReader(sizeX)) < 0) continue;
		printf("Width: ");
		if ((sizeY = inputReader(sizeY)) < 0) continue;
		if (sizeX > 10000000 / sizeY) {
			printf("The size of the field is too big to calculate!\n");
			continue;
		}
		printf("Input the number of populated cells: ");
		if ((numCells = inputReader(numCells)) < 0) continue;
		if (numCells > sizeX * sizeY) {
			printf("Incorrect input, you have only %d cells!\n", sizeX*sizeY);
			continue;
		}
		initSecond();
		printf("Input the number of the final step: ");
		if ((finalStep = inputReader(finalStep)) < 0) continue;
		if (finalStep > 1000000000 / sizeY / sizeX) {
			printf("The number of the steps is too big to calculate!\n");
			continue;
		}
		printf("The size of the field is %dx%d.", sizeX, sizeY);
		printf("\nThe number of populated cells is %d.\n", numCells);
		printf("The number of the final step is %d.", finalStep);
		printf("\nWould you like to write start position of rabbits? (y/n): ");
		cin >> usersAnswer;
		while (usersAnswer != 'y' && usersAnswer != 'n') {
			printf("\nIncorrect input, try again.(Type \'y\' or \'n\'): ");
			cin >> usersAnswer;
		}
		fieldRefresh(sizeX, sizeY, true);
		if (usersAnswer == 'n') {
			for (int i = 0; i < numCells; ++i) {
				int rander = rand()*rand() % (int)(freeCells.size());
				field[freeCells[rander].first][freeCells[rander].second].setNew(true, false);
				freeCells.erase(freeCells.begin() + rander);
			}
		}
		else {
			printf("Write coordinates of populated cells in range for x [0; %d] and for y [0; %d].\nExample: 13 14", sizeX, sizeY);
			for (int i = 0; i < numCells; ++i) {
				int curX = 0, curY = 0;
				printf("\n(%d / %d):", i + 1, numCells);
				if ((curX = inputReader(curX)) < 0 || (curY = inputReader(curY)) < 0 || curX > sizeX || curY > sizeY || freeCellsUser.count(pii(curX, curY))) {
					printf("\nIncorrect input! Try again.");
					i--;
					continue;
				}
				freeCellsUser.insert(pii(curX, curY));
				field[curX][curY].setNew(true, false);
			}
		}
		int dif = -1, lastDif = -1;
		bool wasDif = true;
		for (int step = 1; step <= finalStep; ++step) {
			lifeImitation();
			if (borned == died && wasDif) {
				stableStep = step;
				wasDif = false;
			}
			if (!(step % 10) || step == 1) {
				int gcdCur = 1;
				if (died && borned) gcdCur = gcd(borned, died);
				printf("Step number: %d\n", step);
				printf("Alive rabbits: %d\n", alive);
				printf("Ratio: %d/%d\n", borned / gcdCur, died / gcdCur);
				died = 0;
				borned = 0;
			}
		}
		if (stableStep != -1) printf("Population wasn't changed from %d step.\n", stableStep);
		else printf("Population was changing all time.");
		printf("Population is changed in %d rabbits.\n", alive - numCells);
		printf("What do you want to do next?\n1.New imitation (type \'n\' in this case)\n2.Exit (type \'e\' in this case)\n");
		cin >> usersAnswer;
		while (usersAnswer != 'e' && usersAnswer != 'n') {
			printf("Incorrect input, try again (\'e\' or \'n\'): ");
			cin >> usersAnswer;
		}
		if (usersAnswer == 'n') continue;
		if (usersAnswer == 'e') return 0;
	}
	return 0;
}