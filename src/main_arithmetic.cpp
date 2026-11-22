#include <iostream>
#include <string>
#include <set>
#include "arithmetic.h"

int main() {
	std::cout << "enther the expression: " << std::endl;

	std::string expr;
	std::getline(std::cin, expr);

	Arithmetic<double> calc(expr);
	//1
	calc.lexm();
	//2
	if (!calc.isCorrect()) {
		std::cerr << "expression is not correct" << std::endl;
		return 1;
	}
	//3
	calc.OPZ();

	//4
	std::set<std::string> unVars;
	for (int i = 0; i < expr.length(); i++) {
		char x = expr[i];
		if (isalpha(x)) {
			std::string varn(1, x);
			unVars.insert(varn);
		}
	}

	if (!unVars.empty()) {
		for (const auto& varn : unVars) {
			double value;
			std::cout << "value of a variable" << std::endl;
			std::cin >> value;
			calc.setVariable(varn, value);
		}
	}

	//5
	double result = calc.calculate();
	std::cout << "result: " << result << std::endl;

	return 0;
}
