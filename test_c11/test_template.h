#pragma once
#include <vector>

template  <typename T>
class CA
{
public:
	void push(T t)
	{
			_vec.push_back(t);
	}
	void push2(T t)
	{
		_vec.push_back(t);
	}

	void push3(T t);

private:
	vector<T> _vec;
};

template <typename T>
inline void CA<T>::push3(T t)
{
	_vec.push_back(t);
}

template <class T>
class CB
{
public:
	void push(T t)
	{
		_vec.push_back(t);
	}
	void push2(T t)
	{
		_vec.push_back(t);
	}

	void push3(T t)
	{
		_vec.push_back(t);
	}

private:
	vector<T> _vec;
};