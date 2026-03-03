// DArray 类的实现
# include "DArray.h"
# include <iostream>
# include <cassert>

// 默认构造函数
DArray::DArray() {
	Init();
}

// 创建并初始化一个带默认值的数组
DArray::DArray(int nSize, double dValue) {
	// 待实现
	Init();
	if (nSize < 0 )
	{
		std::cout << "Invalid size!" << std::endl;
		return;
	}
	else
	{
		m_nSize = nSize;
		m_pData = new double[m_nSize];
		for(int i=0;i<m_nSize;i++)
		{
			m_pData[i]=dValue;
		}
	}
}

DArray::DArray(const DArray& arr) {   // 把 arr 拷贝到 this 里
	// 待实现
	# if 0
	Init();
	if (arr.m_nSize <= 0 || arr.m_nSize > m_nSize)
	{
		std::cout << "Invalid size!" << std::endl;
		exit(0);
	}
	else
	{
		m_nSize = arr.m_nSize;
		for (int i = 0; i < m_nSize; i++)
		{
			m_pData [i] = arr.m_pData[i];
		}
	}
	# endif
	Init();
	m_nSize = arr.m_nSize;
	if (m_nSize > 0)
	{
		m_pData = new double[m_nSize];
		for (int i=0;i < m_nSize; i++)
		{
			m_pData[i] = arr[i];
		}
	}	
}

// 析构函数
DArray::~DArray() {
	Free();
}

// 打印数组中的元素
void DArray::Print() const {
	// 待实现
	for (int i=0; i<m_nSize; i++)
	{
		std::cout<<m_pData[i]<<" ";
	}
	std::cout << std::endl;
}

// 初始化数组
void DArray::Init() {
	// 待实现
	# if 0
	for (int i=0; i < m_nSize; i++)
	{
		m_pData[i] = 0.0f;
	}
	m_nSize = 0;
	# endif
	m_pData = nullptr;
	m_nSize = 0;
}

// 释放数组内存
void DArray::Free() {
	// 待实现
	m_nSize = 0;
	delete[] m_pData;
	m_pData = nullptr;
}

// 获取数组大小
int DArray::GetSize() const {
	// 待实现
	return m_nSize; // 你应当返回正确的值
}

// 设置数组大小
void DArray::SetSize(int nSize) {
	// 待实现
	# if 0
	m_nSize = nSize;
	for (int i=0;i<nSize;i++)
	{
		m_pData[i]=0;
	}
	# endif
	if (nSize < 0)
	{
		return;  // 或者抛出异常
	}
	if (nSize == m_nSize)
	{
		return;  // 大小未改变，直接返回
	}
	if (nSize == 0)
	{
		Free();
		return;  // 设置大小为0，释放内存
	}
	double* pNew = new double[nSize];

	int nCopy = (nSize < m_nSize) ? nSize : m_nSize;
	for(int i=0;i<nCopy;i++)
	{
		pNew[i] = m_pData[i];
	}
	for(int i=nCopy;i<nSize;i++)
	{
		pNew[i] = 0.0;
	}
	delete[] m_pData;
	m_pData = pNew;
	m_nSize = nSize;
}

// 获取指定下标处的元素
const double& DArray::GetAt(int nIndex) const {
	// 待实现
	# if 0
	if (nIndex > m_nSize-1 || nIndex < 0 )
	{
		std::cout << "ERROR" << std::endl;
		return 0;
	}
	else
	{
		return m_pData[nIndex];
	}
	# endif
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// 设置指定元素的值
void DArray::SetAt(int nIndex, double dValue) {
	// 待实现
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = dValue;
}

// 重载运算符 '[]'
const double& DArray::operator[](int nIndex) const {
	// 待实现
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
	// static double ERROR; // 你应当删除这一行
	// return ERROR; // 你应当返回正确的值
}

// 在数组末尾添加一个新元素
void DArray::PushBack(double dValue) {
	// 待实现
	# if 0
	m_nSize+=1;
	m_pData[m_nSize-1]=dValue;
	# endif
	# if 0
	m_nSize+=1;
	SetSize(m_nSize);
	m_pData[m_nSize-1] = dValue;
	# endif
	int oldSize = m_nSize;
	SetSize(m_nSize + 1);
	m_pData[oldSize] = dValue;
}

// 删除指定下标处的元素
void DArray::DeleteAt(int nIndex) {
	// 待实现
	# if 0
	if(nIndex < 0 || nIndex >= m_nSize)
	{
		std::cout << "ERROR" << std::endl;
		exit(0);
	}
	for(int i = nIndex+1;i<m_nSize-1;i++)
	{
		m_pData[i]=m_pData[i+1];
	}
	m_nSize-=1;
	delete[] m_pData;
	# endif
	assert(nIndex >= 0 && nIndex < m_nSize);
	if(m_nSize == 1)
	{
		Free();
		return;
	}
	double *pNew = new double[m_nSize-1];
	for(int i = 0; i < nIndex; ++i)
	{
		pNew[i] = m_pData[i];
	}
	for(int i = nIndex + 1; i < m_nSize; ++i)
	{
		pNew[i - 1] = m_pData[i];
	}
	delete[] m_pData;
	m_pData = pNew;
	m_nSize -= 1;
}

// 在指定下标处插入一个新元素
void DArray::InsertAt(int nIndex, double dValue) {
	// 待实现
	# if 0
	if(nIndex < 0 || nIndex >= m_nSize+1)
	{
		std::cout << "ERROR" << std::endl;
		exit(0);
	}
	m_nSize+=1;
	for(int i = m_nSize - 2; i >= nIndex; i--)
	{
		m_pData[i+1]=m_pData[i];
	}
	m_pData[nIndex]=dValue;
	# endif
	assert(nIndex >= 0 && nIndex <= m_nSize);
	double* pNew = new double[m_nSize + 1];
	for(int i = 0; i < nIndex; ++i)
	{
		pNew[i] = m_pData[i];
	}
	pNew[nIndex] = dValue;
	for(int i = nIndex; i < m_nSize; ++i)
	{
		pNew[i + 1] = m_pData[i];
	}
	delete[] m_pData;
	m_pData = pNew;
	m_nSize += 1;
}

// 重载赋值运算符 '='
DArray& DArray::operator = (const DArray& arr) {
	// 待实现
	# if 0
	m_pData = new double(arr.GetSize());
	m_nSize = arr.GetSize();
	for(int i=0;i<m_nSize;i++)
	{
		m_pData[i] = arr[i];
	}
	*this=*m_pData;
	return *this;
	# endif
	if (this == &arr)
	{
		return *this; // 自赋值，直接返回
	}
	double *pNew = nullptr;
	if (arr.m_nSize > 0)
	{
		pNew = new double[arr.m_nSize];
		for (int i=0;i < arr.m_nSize; i++)
		{
			pNew[i] = arr.m_pData[i];
		}
		delete[] m_pData;
		m_pData = pNew;
		m_nSize = arr.m_nSize;
		return *this;
	}
	// this是C++中的隐含指针，指向当前对象
}




