/********************************************************************
	Created:	2015/09/20   13:25:59
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Stack.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Static stack template
*********************************************************************/

#ifndef STACK_H_
#define STACK_H_

template <typename StackDatType, uint8_t Size>
class stack {
	StackDatType arr[Size];
	uint8_t top;

public:
	void push(const StackDatType dat);
	StackDatType pop();
	StackDatType pick();
	bool IsEmpty();
	stack() : top(0) { }
};

template <typename StackDatType, uint8_t Size>
void stack<StackDatType, Size>::push(const StackDatType dat) {
	if(top < Size) {
		arr[top++] = dat;
	}
}

template <typename StackDatType, uint8_t Size>
StackDatType stack<StackDatType, Size>::pop() {
	StackDatType ret = arr[top-1];
	top--;
	return ret;
}

template <typename StackDatType, uint8_t Size>
StackDatType stack<StackDatType, Size>::pick() {
	return arr[top-1];
}

template <typename StackDatType, uint8_t Size>
bool stack<StackDatType, Size>::IsEmpty() {
	return !top ? true : false;
}

#endif /* STACK_H_ */