#pragma once

#include <string>
#include <vector>
#include <map>
#include "stack.h"

// a+(b+c)*d-4
// abc+d*+4-

template <typename ValT = double>
class Arithmetic {
private:
	std::string expression;
	std::vector<std::string> lexemes;
	std::vector<std::string> postfix;
	std::map<std::string, ValT> var;

	int getPriority(const std::string& op) const {
		if (op == "+" || op == "-") return 1;
		if (op == "*" || op == "/") return 2;
		if (op == "~" || op == "#") return 3;
		return 0;
	}

	bool isOperator(const std::string& op) const {
		return (op == "+" || op == "-" || op == "*" || op == "/" || op == "~" || op == "#");
	}

	bool isUnarn(const std::string& op) const {
		return (op == "~" || op == "#");
	}

public:
	Arithmetic(const std::string expr) : expression(expr) {

	}

	void lexm() {
		lexemes.clear();
		std::string cur = "";

		for (int i = 0; i < expression.length(); i++) {
			char x = expression[i];

			if (isspace(x)) {
				continue;
			}
			
			if (x == '(' || x == ')' || x == '-' || x == '+' || x == '*' || x == '/') {

				if (!cur.empty()) {
					lexemes.push_back(cur);
					cur = "";
				}

				std::string op(1, x);

				if (x == '-' || x == '+') {
					if (lexemes.empty() || lexemes.back() == "(" || isOperator(lexemes.back())) {
						op = (x == '-') ? "~" : "#";
					}
				}
				lexemes.push_back(op);
			}

			else {
				cur += x;
			}
		}

		if (!cur.empty()) {
			lexemes.push_back(cur);
		}
	}

	bool isCorrect() const {
		if (lexemes.empty()) return false;

		Stack<std::string> bracketStack(10);

		for (size_t i = 0; i < lexemes.size(); i++) {
			std::string cur = lexemes[i];

			if (cur == "(") {
				bracketStack.push(cur);
				if (i + 1 < lexemes.size() && lexemes[i + 1] == ")") return false;
			}

			else if (cur == ")") {
				if (bracketStack.isEmpty()) return false;
				bracketStack.pop();
			}

			if (isOperator(cur)) {
				if (i == 0 && !isUnarn(cur)) return false;
				if (i == lexemes.size() - 1) return false;

				if (!isUnarn(cur)) {
					if (i > 0 && lexemes[i - 1] == "(") return false;
				}

				if (i + 1 < lexemes.size() && lexemes[i + 1] == ")") return false;

				if (i + 1 < lexemes.size() && isOperator(lexemes[i + 1])) {
					if (!isUnarn(lexemes[i + 1])) return false;
				}
			}

			if (cur == "(" && i > 0) {
				std::string prev = lexemes[i - 1];
				if (!isOperator(prev) && prev != "(") return false;
			}

			if (cur == ")" && (i + 1) < lexemes.size()) {
				std::string next = lexemes[i + 1];
				if (!isOperator(next) && next != ")") return false;
			}
		}

		return bracketStack.isEmpty();
	}


	void OPZ() {
		postfix.clear();
		Stack<std::string> opstack(10);

		for (int i = 0; i < lexemes.size(); i++) {
			std::string cur = lexemes[i];

			//если число или переменная
			if (!isOperator(cur) && cur != ")" && cur != "(") {
				postfix.push_back(cur);
			}

			else if (cur == "(") {
				opstack.push(cur);
			}

			else if (cur == ")") {
				while (!opstack.isEmpty() && opstack.look() != "(") {
					postfix.push_back(opstack.pop());
				}
				opstack.pop();
			}

			//если оператор
			else if (isOperator(cur)) {
				while (!opstack.isEmpty() && getPriority(opstack.look()) >= getPriority(cur)) {
					postfix.push_back(opstack.pop());
				}
				opstack.push(cur);
			}
		}

		while (!opstack.isEmpty()) {
			postfix.push_back(opstack.pop());
		}
	}

	std::vector<std::string> getPostfix() const {
		return postfix;
	}

	void setVariable(const std::string& name, ValT value) {
		var[name] = value;
	}

	ValT calculate() {
		if (postfix.empty()) throw std::runtime_error("Empty postfix expression");

		Stack<ValT> valstack(100);

		for (size_t i = 0; i < postfix.size(); i++) {
			std::string cur = postfix[i];

			if (isOperator(cur)) {

				if (isUnarn(cur)) {
					if (valstack.getSize() < 1) throw std::runtime_error("Insufficient operands");
					ValT operand = valstack.pop();
					valstack.push((cur == "~") ? -operand : operand);
				}

				else {
					if (valstack.getSize() < 2) throw std::runtime_error("Error in the OPZ structure");
					ValT right = valstack.pop();
					ValT left = valstack.pop();
					ValT res = 0;

					if (cur == "+") res = left + right;
					else if (cur == "-") res = left - right;
					else if (cur == "*") res = left * right;
					else if (cur == "/") {
						if (right == 0) throw std::runtime_error("Division by zero");
						res = left / right;
					}
					valstack.push(res);
				}
			}

			else if (var.find(cur) != var.end()) {
				valstack.push(var[cur]);
			}

			else {
				ValT num;
				try {
					if constexpr (std::is_same_v<ValT, int>) num = std::stoi(cur);
					else if constexpr (std::is_same_v<ValT, float>) num = std::stof(cur);
					else num = std::stod(cur);
				}
				catch (const std::invalid_argument&) {
					throw std::runtime_error("Unknown variable or invalid number: " + cur);
				}
				catch (const std::out_of_range&) {
					throw std::runtime_error("Number out of range: " + cur);
				}
				valstack.push(num);
			}

		}

		if (valstack.getSize() != 1) {
			throw std::runtime_error("Error in the OPZ structure");
		}

		return valstack.pop();
	}

};