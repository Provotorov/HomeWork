#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int64;

class groupNum {
public:
	int group; // the group of numbers unites same numbers, it appeares when one of the operation was made
	int64 num; // meaning of current number
	groupNum(int group, int64 num) { // definition of the object
		groupNum::group = group;
		groupNum::num = num;
	}
};

class operations {
public:
	char operation; // defines which operation is used
	int indexLeft; // left number
	int indexRight; // right number
	int priority; // defines priority of operation
	operations(char operation, int indexLeft, int indexRight, int priority) { //definition of the object
		operations::operation = operation;
		operations::indexLeft = indexLeft;
		operations::indexRight = indexRight;
		operations::priority = priority;
	}
};

char inSeq[10000];
vector <char> seq, quates;
vector <groupNum> nums;
vector <operations> oper;
bool test = true;
int group = 0, lastIndex = 0, curPriority = 0, maxPriority = 0;

void init() {
	seq.clear();
	quates.clear();
	nums.clear();
	oper.clear();
	test = true;
	group = 0;
	curPriority = 0;
	maxPriority = 0;
	for (int i = 0; i < 10000; ++i) inSeq[i] = 0;
}

int expFinder(int64 num) {
	int ret = 0;
	while (num) {
		++ret;
		num /= 10;
	}
	return ret;
}

int64 converter(int curPos, bool dir) { //converts 2 numbers, which is diveded by some operation, from char to integer
	int64 ret = 0, pow = 1;
	if (!dir) {
		while (true) {
			curPos--;
			ret += ((int64)seq[curPos] - (int64)'0') * pow;
			pow *= 10;
			if (curPos - 1 == -1) return ret;
			if (seq[curPos - 1] > '9' || seq[curPos - 1] < '0') return ret;
			if (expFinder(ret) == 15) {
				test = false;
				return ret;
			}
		}
	}
	if (dir) {
		while (true) {
			curPos++;
			ret += ((int64)seq[curPos] - (int64)'0');
			if (curPos + 1 == (int)seq.size()) return ret;
			if (seq[curPos + 1] > '9' || seq[curPos + 1] < '0') return ret;
			ret *= 10;
			if (expFinder(ret) == 15) {
				test = false;
				return ret;
			}
		}
	}
	return ret;
}

void groupNumsRefresh(int curGroup, int64 curNum) {
	for (int i = 0; i < (int)nums.size(); ++i)
	if (nums[i].group == curGroup) nums[i].num = curNum;
}

void groupRefresh(int curGroup, int oldGroup) {
	for (int i = 0; i < (int)nums.size(); ++i)
	if (nums[i].group == oldGroup) nums[i].group = curGroup;
}

int main() {
	cout << "Type \"e\" (without quates) if You want to exit." << endl;
	while (true) {
		init();
		cout << "Input expression: " << endl;
		gets(inSeq);
		if (inSeq[0] == 'e') return 0;
		if (inSeq[0] > '9' || inSeq[0] < '0' && inSeq[0] != '(') {
			cout << "Incorrect input!" << endl;
			continue;
		}
		for (int i = 0; i < 1000; ++i) {
			if (inSeq[i] == ' ' || inSeq[i] == '\0') continue;
			if ((inSeq[i] > '9' || inSeq[i] < '0') && (inSeq[i] != '+' && inSeq[i] != '-' && inSeq[i] != '/' && inSeq[i] != '*' && inSeq[i] != '%') && (inSeq[i]) != '(' && (inSeq[i] != ')')) {
				bool test = false;
				cout << "Incorrect input!" << endl;
				break;
			}
			if (inSeq[i] != '(' && inSeq[i] != ')') seq.push_back(inSeq[i]);
			else quates.push_back(inSeq[i]);
			if (inSeq[i] == '+' || inSeq[i] == '-' || inSeq[i] == '/' || inSeq[i] == '*' || inSeq[i] == '%') quates.push_back(inSeq[i]);
		}
		if (seq[(int)seq.size() - 1] > '9' || seq[(int)seq.size() - 1] < '0' && seq[(int)seq.size() - 1] != ')') {
			cout << "Incorrect input!" << endl;
			continue;
		}
		for (int i = 1; i < (int)seq.size() - 1; ++i) {
			if ((seq[i] == '*' || seq[i] == '-' || seq[i] == '+' || seq[i] == '%' || seq[i] == '/') && (seq[i - 1] < '0' || seq[i - 1] > '9' || seq[i + 1] > '9' || seq[i + 1] < '0')) {
				test = false;
				cout << "Incorrect input!" << endl;
				break;
			}
			if (seq[i] == '*' || seq[i] == '-' || seq[i] == '+' || seq[i] == '%' || seq[i] == '/') {
				if (!group) {
					groupNum num1(group, converter(i, false));
					++group;
					nums.push_back(num1);
				}
				groupNum num2(group, converter(i, true));
				++group;
				nums.push_back(num2);
				operations curOperation(seq[i], (int)nums.size() - 2, (int)nums.size() - 1, 0);
				oper.push_back(curOperation);
			}
		}
		if (!(int)oper.size()) {
			cout << "No operations!" << endl;
			continue;
		}
		int curOper = 0;
		for (int i = 0; i < (int)quates.size(); ++i) {
			if (quates[i] == '(') {
				curPriority++;
				maxPriority = max(curPriority, maxPriority);
			}
			if (quates[i] == ')') {
				curPriority--;
			}
			if (quates[i] == '+' || quates[i] == '-' || quates[i] == '/' || quates[i] == '*' || quates[i] == '%') {
				oper[curOper].priority = curPriority;
				++curOper;
			}
		}
		if (curPriority != 0) {
			cout << "Incorrect input!" << endl;
			continue;
		}
		if (!test) continue;
		for (int j = maxPriority; j >= 0; --j) {
			if (!test) break;
			for (int i = 0; i < (int)oper.size(); ++i) {
				if (oper[i].operation == '*' && oper[i].priority == j) {
					if (expFinder(nums[oper[i].indexLeft].num) + expFinder(nums[oper[i].indexRight].num) >= 15) {
						test = false;
						cout << "Nnumbers are too big to calculate!" << endl;
						break;
					}
					nums[oper[i].indexLeft].num *= nums[oper[i].indexRight].num;
					int newGroup = nums[oper[i].indexLeft].group;
					groupRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexRight].group);
					groupNumsRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexLeft].num);
				}
				if (oper[i].operation == '/' && oper[i].priority == j) {
					if (!nums[oper[i].indexRight].num) {
						cout << "Incorrect operation! (Z / 0)" << endl;
						test = false;
						continue;
					}
					nums[oper[i].indexLeft].num /= nums[oper[i].indexRight].num;
					int newGroup = nums[oper[i].indexLeft].group;
					groupRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexRight].group);
					groupNumsRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexLeft].num);
				}
				if (oper[i].operation == '%' && oper[i].priority == j) {
					nums[oper[i].indexLeft].num %= nums[oper[i].indexRight].num;
					int newGroup = nums[oper[i].indexLeft].group;
					groupRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexRight].group);
					groupNumsRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexLeft].num);
				}
			}
			for (int i = 0; i < (int)oper.size(); ++i) {
				if (oper[i].operation == '+' && oper[i].priority == j) {
					if (expFinder(nums[oper[i].indexLeft].num) == 15 || expFinder(nums[oper[i].indexRight].num) == 15) {
						test = false;
						cout << "Numbers are too big to calculate!" << endl;
						break;
					}
					nums[oper[i].indexLeft].num += nums[oper[i].indexRight].num;
					int newGroup = nums[oper[i].indexLeft].group;
					groupRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexRight].group);
					groupNumsRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexLeft].num);
				}
				if (oper[i].operation == '-' && oper[i].priority == j) {
					nums[oper[i].indexLeft].num -= nums[oper[i].indexRight].num;
					int newGroup = nums[oper[i].indexLeft].group;
					groupRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexRight].group);
					groupNumsRefresh(nums[oper[i].indexLeft].group, nums[oper[i].indexLeft].num);
				}
			}
		}
		if (!test) continue;
		cout << "Answer: " << nums[0].num << endl;
	}
	return 0;
}