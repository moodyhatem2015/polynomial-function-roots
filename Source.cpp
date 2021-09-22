#include<iostream>
#include<cstring>
#include<math.h>
#pragma warning (disable : 4996)

using namespace std;
int noOfTermS(char* func);
void getCoeffWPow(char* func, int noOfTerms, double* coeff, double* power);
void doubleCoeff(char* term, int t, double* coeff);
void doublePower(char* term, int t, double* power);
void solution(double a, double b,int noOfTerms, int* noOfRoots, double* coeff, double* power);
void bisection(double a, double b, double ansa, double ansb, int noOfTerms, double* coeff, double* power, int f, int* noOfRoots);
void slope0solution(double a, double b, int noOfTerms, int* noOfRoots, double* coeff, double* power);
void subs(double x, int noOfTerms, double* coeff, double* power, int* noOfRoots);
int main() {
	char repeat = 'y';
	do {
		cout << "enter the equation" << endl;
		char func[500];
		cin.getline(func, 500);
		int noOfTerms;
		noOfTerms = noOfTermS(func);
		if (noOfTerms == 0 || strlen(func) == 0) {
			cout << "correctly ";continue;
		}
		double* coeff = new double[noOfTerms];
		double* power = new double[noOfTerms];
		getCoeffWPow(func, noOfTerms, coeff, power);
		while (true)
		{
			double a, b;
			cout << "enter the first limit" << endl;
			cin >> a;
			cout << "enter the second limit" << endl;
			cin >> b;
			int noOfRoots = 0;
			if (a < b) {
				solution(a, b, noOfTerms, &noOfRoots, coeff, power);
				slope0solution(a, b, noOfTerms, &noOfRoots, coeff, power);
				break;
			}
			else if (b < a) {
				solution(b, a, noOfTerms, &noOfRoots, coeff, power);
				slope0solution(b, a, noOfTerms, &noOfRoots, coeff, power);
				break;
			}
			else {
				cout << "you entered the same number" << endl;
			}
		}
		delete[]coeff;
		delete[]power;
		cout << "if you want to solve more equations enter y" << endl;
		cin >> repeat;
		cin.ignore();
	} while (repeat == 'y' || repeat == 'Y');
}
int noOfTermS(char* func) {
	int noOfTerms = 1;
	if (func[strlen(func) - 1] == ' ') {
		cout << "you can't leave space at the end of the equation" << endl;
		return 0;
	}
	if (func[0] == ' ') {
		cout << "you can't leave space at the beginning of the equation" << endl;
		return 0;
	}
	int noOfx = 0;
	for (int i = 0;i < strlen(func);i++) {//check errors
		if (func[i] == '=')
			noOfTerms--;
		if (noOfTerms == -1) {
			cout << "you can't write more than 1 =" << endl;
			return 0;
		}
		if (func[i] == 'X') {
			func[i] = 'x';
		}
		if (func[i] == '*' && (func[i + 1] == 'x' || func[i + 1] == 'X')) {
			func[i] = ' ';
		}
		if (func[i] == '^' && !(func[i - 1] == 'x' || ((func[i - 2] == 'x' && func[i - 1] == ' ')))) {
			cout << "you enter the equation not in the simplest form or you leaved more than 1 space" << endl;
			return 0;
		}
		if (!(func[i] >= '0' && func[i] <= '9') && !(func[i] == ' ' || func[i] == 'x' || func[i] == '+' || func[i] == '-' || func[i] == '=' || func[i] == '^' || func[i] == '.' )) {
			cout << "you entered worng character or the equation not in the simplest form" << endl;
			return 0;
		}	
		if ((func[i] == '=' && i != strlen(func) - 2 && i != strlen(func) - 3)) {
			cout << "you write the equation wrong porbably you put = in the middle of the equation or you leaved more than one space" << endl;
			return 0;
		}
		if (func[i] == '=' && !(func[i + 1] == '0' || (func[i + 1] == ' ' && func[i + 2] == '0'))) {
			cout << "you write the equation wrong porbably it isn't equal to zero or you leaved more than 1 space" << endl;
			return 0;
		}
		if (func[i] >= '0' && func[i] <= '9' && func[i + 1] == ' ' && func[i + 2] >= '0' && func[i + 2] <= '9') {
			cout << "you can't leave spaces between the numbers" << endl;
			return 0;
		}
		if (func[i] == '^' && func[i + 1] == ' ') {
			cout << "you can't leave space after ^" << endl;
			return 0;
		}
		if (func[i] == '^' && func[i + 1] == '+') {
			func[i + 1] = ' ';
		}
		if (func[i] == ' ' && func[i + 1] == ' ') {
			cout << "you can't leave more than 1 space" << endl;
			return 0;
		}
	}
	if (func[strlen(func) - 1] == '+' || func[strlen(func) - 1] == '-')
		noOfTerms--;
	for (int i = 1;i < strlen(func);i++) {//count terms
		if (func[i] == 'x') {
			noOfx++;
			if (noOfx == 2) {
				cout << "you can't write two x in one term" << endl;
				return 0;
			}
		}
		if (func[i] == '+' || func[i] == '-' || func[i] == '=') {
			noOfTerms++;
			noOfx = 0;
		}
		if ((func[i] == '+' && func[i + 1] == '+') || (func[i] == '+' && func[i + 1] == '-')) {
			noOfTerms--;
			func[i] = ' ';
		}
		if (func[i] == '-' && func[i + 1] == '+') {
			func[i + 1] = ' ';
		}
		if (func[i] == '-' && func[i + 1] == '-') {
			func[i] = '+';
			func[i + 1] = ' ';
		}
		if (func[i] == '-' && func[i - 1] == '^') {
			noOfTerms--;
			noOfx = 1;
		}
	}
	return noOfTerms;
}
void getCoeffWPow(char* func, int noOfTerms, double* coeff, double* power) {
	char term[15];
	int read = 0;
	for (int i = 0;i < noOfTerms;i++) {
		for (int j = 1;j < 15;j++) {
			if (j == 1)
				term[0] = func[read];
			read++;
			term[j] = func[read];
			if (func[read] == '-' && func[read - 1] == '^') {
				continue;
			}
			if (func[read] == '+' || func[read] == '-' || func[read] == '=') {
				term[j] = '\0';
				break;
			}
		}
		doubleCoeff(term, i, coeff);
		doublePower(term, i, power);
	}
}
void doubleCoeff(char* term, int t, double* coeff) {
	coeff[t] = 1;
	int dot = 0;
	if (term[0] >= '0' && term[0] <= '9') {
		coeff[t] = 0;
	}
	for (int i = 0;term[i] != 'x' && i < strlen(term);i++) {
		if (term[i + 1] >= '0' && term[i + 1] <= '9') {
			coeff[t] = 0;
		}
		if (term[i] == '.' || (term[i] <= '9' && term[i] >= '0')) {
			if (term[i] == '.') {
				dot = i;
				break;
			}
			else {
				dot = i + 1;
			}
		}
	}
	for (int i = 0;i < dot;i++) {
		if (term[i] >= '0' && term[i] <= '9') {
			coeff[t] += ((int)term[i] - (int)'0') * pow(10, dot - i - 1);
		}
	}
	for (int i = dot;term[i] != 'x' && i < strlen(term);i++) {
		if (term[i] >= '0' && term[i] <= '9') {
			coeff[t] += ((int)term[i] - (int)'0') * pow(10, dot - i);
		}
	}
	if (term[0] == '-')
		coeff[t] *= -1;
}
void doublePower(char* term, int t, double* power) {
	power[t] = 1;
	int dot = 0;
	int x = -1;
	for (int i = 0;i < strlen(term);i++) {
		if (term[i] == 'x') {
			x = i;
			break;
		}
	}
	if (x != -1) {
		for (int i = x;i < strlen(term);i++) {
			if (term[i + 1] >= '0' && term[i + 1] <= '9') {
				power[t] = 0;
			}
			if (term[i] == '.' || (term[i] <= '9' && term[i] >= '0') || term[i] == 'x') {
				if (term[i] == '.') {
					dot = i;
					break;
				}
				else {
					dot = i + 1;
				}
			}
		}
		for (int i = x + 1;i < dot;i++) {
			if (term[i] >= '0' && term[i] <= '9') {
				power[t] += ((int)term[i] - (int)'0') * pow(10, dot - i - 1);
			}
		}
		for (int i = dot;i < strlen(term);i++) {
			if (term[i] >= '0' && term[i] <= '9') {
				power[t] += ((int)term[i] - (int)'0') * pow(10, dot - i);
			}
		}
		if (term[x + 1] == '-' || (term[x + 1] = '^' && term[x + 2] == '-'))
			power[t] *= -1;
	}
	else {
		power[t] = 0;
	}
}
void solution(double a, double b, int noOfTerms, int* noOfRoots, double* coeff, double* power) {
	double step;
	step = (b - a) / 1000000;
	double ans = 0;
	for (int i = 0;a < b;a += step) {
		double oldans = ans;
		ans = 0;
		for (int j = 0;j < noOfTerms;j++) {
			ans += coeff[j] * pow(a, power[j]);
		}
		if (ans > 1000000 || ans < -1000000)
			continue;
		if (ans == 0) {
			*noOfRoots += 1;
			cout << "X" << *noOfRoots << " = " << a << endl;
			continue;
		}
		if ((oldans > 0 && ans < 0) || (oldans < 0 && ans > 0)) {
			*noOfRoots += 1;
			cout << "X" << *noOfRoots << " = ";
			bisection(a - step, a, oldans, ans, noOfTerms, coeff, power, 1, noOfRoots);
		}
	}
}
void bisection(double a, double b, double ansa, double ansb, int noOfTerms, double* coeff, double* power, int f, int* noOfRoots) {
	while (true)
	{
		double c = (a + b) / 2;
		double ansc = 0;
		for (int i = 0;i < noOfTerms;i++) {
			if (f == 1)
				ansc += coeff[i] * pow(c, power[i]);
			else if (f == 0)
				ansc += coeff[i] * power[i] * pow(c, power[i] - 1);
		}
		if (fabs(b - a) < 0.0000000000001) {
			if (f == 1) {
				if (fabs(c) < 0.0001)
					c = 0;
				cout << c << endl;
			}
			if (f == 0) {
				subs(c, noOfTerms, coeff, power, noOfRoots);
			}
			break;
		}
		if ((ansc < 0 && ansa < 0)|| (ansc > 0 && ansa > 0)) {
			ansa = ansc;
			a = c;
		}
		else if ((ansc < 0 && ansb < 0) || (ansc > 0 && ansb > 0)) {
			ansb = ansc;
			b = c;
		}
	}
}
void slope0solution(double a, double b, int noOfTerms,int* noOfRoots, double* coeff, double* power) {
	double step;
	step = (b - a) / 1000000;
	double diffans = 0;
	for (int i = 0;a < b;a += step) {
		double olddiffans = diffans;
		diffans = 0;
		for (int j = 0;j < noOfTerms;j++) {
			diffans += coeff[j] * power[j] * pow(a, power[j] - 1);
		}
		if (diffans > 10000 || diffans < -10000)
			continue;
		if (diffans == 0) {
			subs(a, noOfTerms, coeff, power, noOfRoots);
			continue;
		}
		if ((olddiffans > 0 && diffans < 0) || (olddiffans < 0 && diffans>0)) {
			bisection(a - step, a, olddiffans, diffans, noOfTerms, coeff, power, 0, noOfRoots);
		}
	}
	if (*noOfRoots == 0)
		cout << "there is no roots in this region" << endl;
}
void subs(double x, int noOfTerms, double* coeff, double* power, int* noOfRoots) {
	double ansx = 0;
	for (int i = 0;i < noOfTerms;i++)
		ansx += coeff[i] * pow(x, power[i]);
	if (fabs(ansx) < 0.000000000001) {
		if (fabs(x) < 0.00001)
			x = 0;
		*noOfRoots += 1;
		cout << "X" << *noOfRoots << " = " << x << endl;
	}
}