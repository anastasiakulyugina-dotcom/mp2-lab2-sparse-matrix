#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class Stack {
private:
	T* pMem;
	int size;
	int top;

	void resize() {
		int newsize = size * 2;
		T* newMem = new T[newsize];
		for (int i = 0; i <= top; i++) {
			newMem[i] = pMem[i];
		}
		delete[] pMem;
		pMem = newMem;
		size = newsize;
	}
public:
	Stack(int strtsize = 10) {
		if (strtsize <= 0) {
			throw std::invalid_argument("The stack size must be greater than zero");
		}
		size = strtsize;
		pMem = new T[size];
		top = -1;
	}

	~Stack() {
		delete[] pMem;
	}

	Stack(const Stack& other) {
		size = other.size;
		top = other.top;
		pMem = new T[size];
		for (int i = 0; i <= top; i++) {
			pMem[i] = other.pMem[i];
		}
	}

	Stack& operator=(Stack other) {
		std::swap(pMem, other.pMem);
		std::swap(size, other.size);
		std::swap(top, other.top);
		return *this;
	}

	void push(const T& val) {
		if (isFull()) {
			resize();
		}
		pMem[++top] = val;
	}

	T pop() {
		if (isEmpty()) {
			throw std::runtime_error("stack is empty");
		}
		return pMem[top--];
	}

	T look() const {
		if (isEmpty()) {
			throw std::runtime_error("stack is empty");
		}
		return pMem[top];
	}

	bool isEmpty() const {
		return top == -1;
	}

	bool isFull() const {
		return top == size - 1;
	}

	int getSize() const {
		return top + 1;
	}

	void clear() {
		top = -1;
	}
};