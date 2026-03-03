#pragma once

#include <cassert>

// DArray 类的实现
#include "DArray.h"
#include <iostream>
#include <cassert>

template<class DataType>

class DArray {
public:
	DArray(); // 默认构造函数
	DArray(int nSize, double dValue = 0); // 创建并初始化一个带默认值的数组
	DArray(const DArray& arr); // 拷贝构造函数
	~DArray(); // 析构函数

	void Print() const; // 打印数组中的元素

	int GetSize() const; // 获取数组大小
	void SetSize(int nSize); // 设置数组大小

	const DataType& GetAt(int nIndex) const; // 获取指定下标处的元素
	void SetAt(int nIndex, DataType dValue); // 设置指定元素的值

	DataType& operator[](int nIndex); // 重载运算符 '[]'
	const DataType& operator[](int nIndex) const; // 重载运算符 '[]'

	void PushBack(DataType dValue); // 在数组末尾添加一个新元素
	void DeleteAt(int nIndex); // 删除指定下标处的元素
	void InsertAt(int nIndex, DataType dValue); // 在指定下标处插入一个新元素

	DArray& operator = (const DArray& arr); // 重载赋值运算符 '='

private:
	DataType* m_pData; // 指向数组内存的指针
	int m_nSize; // 数组大小
	int m_nMax;

private:
	void Init(); // 初始化数组
	void Free(); // 释放数组内存
	void Reserve(int nSize); // 分配足够的内存
};

template<class DataType>
// 默认构造函数
DArray<DataType>::DArray() {
	Init();
}

template<class DataType>
// 分配足够的内存，更新m_nMax
void DArray<DataType>::Reserve(int nSize) { 
	assert(nSize >= 0);
	if (nSize <= m_nMax){
		return;
	}
	double* m_new = new double [nSize];
	for(int i=0; i < m_nSize; i++)
	{
		m_new[i] = m_pData[i];
	}
	delete[] m_pData;
	m_pData = m_new;
	m_nMax = nSize;
}

template<class DataType>
// 创建并初始化一个带默认值的数组
DArray<DataType>::DArray(int nSize, double dValue) {
	// 待实现
	Init();
	assert (/*nSize <= m_nMax &&*/ nSize >= 0);
	Reserve(nSize);
	m_nSize = nSize;
	for (int i = 0; i < m_nSize; i++)
	{
		m_pData[i] = dValue;
	}
}


template<class DataType>
DArray<DataType>::DArray(const DArray& arr) {
	// 待实现
	Init();
	// assert(arr.m_nSize <= m_nMax);
	//	m_nSize = arr.m_nSize;
	Reserve(arr.m_nSize);
	m_pData = new double [/*m_nSize*/arr.m_nSize];
	for(int i=0; i < /*m_nSize*/arr.m_nSize; i++)
	{
		m_pData[i] = arr[i];
	}
}

template<class DataType>
// 析构函数
DArray<DataType>::~DArray() {
	Free();
}

template<class DataType>
// 打印数组中的元素
void DArray<DataType>::Print() const {
	// 待实现
	for(int i=0; i < m_nSize; i++)
	{
		std::cout << m_pData[i] << " ";
	}
	std::cout << std::endl;
}

template<class DataType>
// 初始化数组
void DArray<DataType>::Init() {
	// 待实现
	m_pData = nullptr;
	m_nSize = 0;

	m_nMax = 0;
}

template<class DataType>
// 释放数组内存
void DArray<DataType>::Free() {
	// 待实现
	delete[] m_pData;
	Init();
}

template<class DataType>
// 获取数组大小
int DArray<DataType>::GetSize() const {
	// 待实现
	return m_nSize; // 你应当返回正确的值
}

template<class DataType>
// 设置数组大小
void DArray<DataType>::SetSize(int nSize) {
	// 待实现
	# if 0
	assert(nSize <= m_nMax);
	if(nSize > m_nSize)
	{
		double *m_new;
		m_new = new double[nSize];
		for(int i=0; i < m_nSize; i++)
		{
			m_new[i] = m_pData[i];
		}
		for(int i=m_nSize; i < nSize; i++)
		{
			m_new[i] = 0;
		}
		delete[] m_pData;
		m_pData = m_new;
		m_nSize = nSize;
	}
	if(nSize = m_nSize)
	{
		return;
	}
	if(nSize < m_nSize)
	{
		double *m_new;
		m_new = new double[nSize];
		for(int i=0; i < nSize; i++)
		{
			m_new[i] = m_pData[i];
		}
		delete[] m_pData;
		m_pData = m_new;
		m_nSize = nSize;
	}
	# endif
	assert(nSize >= 0);
	if (nSize > m_nMax) {
		int newMax = (m_nMax > 0) ? m_nMax : 1;
		while (newMax < nSize) {
			newMax *= 2;
		}
		Reserve(newMax);
	}
	if (nSize > m_nSize) {
		for (int i = m_nSize; i < nSize; i++)
		{
			m_pData[i] = 0;
		}
	}
	m_nSize = nSize;
}

template<class DataType>
// 获取指定下标处的元素
const DataType& DArray<DataType>::GetAt(int nIndex) const {
	// 待实现
	assert(nIndex > 0 && nIndex >= m_nSize);
	return m_pData[nIndex];
}

template<class DataType>
// 设置指定元素的值
void DArray<DataType>::SetAt(int nIndex, DataType dValue) {
	// 待实现
	assert(nIndex > 0 && nIndex >= m_nSize);
	m_pData[nIndex] = dValue;
}

template<class DataType>
// 重载运算符 '[]'
DataType& DArray<DataType>::operator[](int nIndex) {
	// 待实现
	assert(nIndex >= 0 && nIndex /*<=*/ < m_nSize);
	return m_pData[nIndex];
}

template<class DataType>
// 重载运算符 '[]'
const DataType& DArray<DataType>::operator[](int nIndex) const {
	// 待实现
	assert(nIndex >= 0 && nIndex /*<=*/ < m_nSize);
	return m_pData[nIndex];
}

template<class DataType>
// 在数组末尾添加一个新元素
void DArray<DataType>::PushBack(DataType dValue) {
	// 待实现
	# if 0
	assert(m_nSize + 1 <= m_nMax);
	double* m_new;
	m_new = new double [m_nSize + 1];
	for(int i=0; i < m_nSize;i++)
	{
		m_new[i] = m_pData[i];
	}
	m_new[m_nSize] = dValue;
	delete[] m_pData;
	m_pData = m_new;
	m_nSize += 1;
	# endif
	if (m_nSize >= m_nMax) {
		Reserve(m_nMax == 0 ? 1 : m_nMax * 2);
	}
	m_pData[m_nSize] = dValue;
	m_nSize += 1;
}

template<class DataType>
// 删除指定下标处的元素
void DArray<DataType>::DeleteAt(int nIndex) {
	// 待实现
	assert(nIndex+1 <= m_nSize && nIndex >= 0);
	DataType* m_new = new DataType [m_nSize - 1];
	for(int i = 0;i < nIndex; i++)
	{
		m_new[i] = m_pData[i];
	}
	for(int i = nIndex; i < m_nSize - 1; i++)
	{
		m_new[i] = m_pData[i+1];
	}
	delete[] m_pData;
	m_pData = m_new;
	m_nSize -=1;
}


template<class DataType>
// 在指定下标处插入一个新元素
void DArray<DataType>::InsertAt(int nIndex, DataType dValue) {
	// 待实现
	# if 0
	assert(nIndex >= 0 && nIndex <= m_nSize - 1 && m_nSize + 1 <= m_nMax);
    double* m_new = new double [m_nSize + 1];
	for(int i = 0; i < nIndex; i++)
	{
		m_new[i] = m_pData[i];
	}
	m_new[nIndex] = dValue;
	for(int i = nIndex + 1; i < m_nSize + 1; i++)
	{
		m_new[i] = m_pData[i - 1];
	}
	delete[] m_pData;
	m_pData = m_new;
	m_nSize +=1;
	# endif
	assert(nIndex >= 0 && nIndex <= m_nSize);
	if(m_nSize >= m_nMax) {
		Reserve(m_nMax == 0 ? 1 : m_nMax * 2);
	}
	for(int i = m_nSize; i > nIndex; i--)
	{
		m_pData[i] = m_pData[i-1];
	}
	m_pData[nIndex] = dValue;
	m_nSize += 1;
}

template<class DataType>
// 重载赋值运算符 '='
DArray<DataType>& DArray<DataType>::operator = (const DArray& arr) {
	// 待实现
	# if 0
	assert(arr.m_nSize <= m_nMax);
	delete[] m_pData;
	double* m_new = new double [arr.m_nSize];
	for(int i=0; i < arr.m_nSize; i++)
	{
		m_new[i] = arr[i]; 
	}
	m_pData = m_new;
	m_nSize = arr.m_nSize;
	return *this;
	# endif
	
	if (this == &arr) {
		return *this;
	}
	if (arr.m_nSize > m_nMax) {
		Reserve(arr.m_nSize);
	}
	for(int i = 0; i < arr.m_nSize; i++) {
		m_pData[i] = arr.m_pData[i];
	}
	m_nSize = arr.m_nSize;
	return *this;
}