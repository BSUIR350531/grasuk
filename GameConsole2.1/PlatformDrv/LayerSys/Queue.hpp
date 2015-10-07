/********************************************************************
	Created:	2014/12/27   20:39:12
	Filename: 	GameConsole2.1\PlatformDrv\LayerSys\Queue.hpp

	Author:		SlavaGrasiuk
	
	purpose:	Static queue template
*********************************************************************/

#ifndef QUEUE_H_
#define QUEUE_H_
	
template <typename QueueDatType, unsigned char Size> class queue {
	QueueDatType arr[Size], init_dat;
	unsigned char end;
public:
	queue(const QueueDatType dat);
	void push(const QueueDatType dat);
	QueueDatType pop();
	bool IsEmty();
	void clear();
};

template <typename QueueDatType, unsigned char Size> 
queue<QueueDatType, Size>::queue(const QueueDatType dat) {
	for(end = 0; end < Size; arr[end] = dat, end++);
	init_dat = dat;
	end = 0;
}

template <typename QueueDatType, unsigned char Size> 
void queue<QueueDatType, Size>::push(const QueueDatType dat) {
	if( end < Size) {
		arr[end++] = dat;
	}
}

template <typename QueueDatType, unsigned char Size> 
QueueDatType queue<QueueDatType, Size>::pop() {
	QueueDatType res = *arr;
	for(unsigned char i = 1; i < end; arr[i-1] = arr[i], i++);
	arr[--end] = init_dat;
	return res;
}

template <typename QueueDatType, unsigned char Size> 
bool queue<QueueDatType, Size>::IsEmty() {
	return !end ? true : false;
}

template <typename QueueDatType, unsigned char Size> 
void queue<QueueDatType, Size>::clear() {
	for(end = 0; end<Size; arr[end] = init_dat, end++);
	end = 0;
}

#endif /* QUEUE_H_ */