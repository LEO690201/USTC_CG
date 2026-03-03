#pragma once  // 用来防止头文件被多次包含

// 动态数组类 DArray 的接口声明
class DArray {
public:
	DArray(); // 默认构造函数
	DArray(int nSize, double dValue = 0); // 创建并初始化一个带默认值的数组
	DArray(const DArray& arr); // 拷贝构造函数
	~DArray(); // 析构函数

	void Print() const; // 打印数组中的元素

	int GetSize() const; // 获取数组大小
	void SetSize(int nSize); // 设置数组大小

	const double& GetAt(int nIndex) const; // 获取指定下标处的元素
	void SetAt(int nIndex, double dValue); // 设置指定元素的值

	double& operator[](int nIndex); // 重载运算符 '[]'
	const double& operator[](int nIndex) const; // 重载运算符 '[]'

	void PushBack(double dValue); // 在数组末尾添加一个新元素
	void DeleteAt(int nIndex); // 删除指定下标处的元素
	void InsertAt(int nIndex, double dValue); // 在指定下标处插入一个新元素

	DArray& operator = (const DArray& arr); // 重载赋值运算符 '='

private:
	double* m_pData; // 指向数组内存的指针
	int m_nSize; // 数组大小

private:
	void Init(); // 初始化数组
	void Free(); // 释放数组内存
};
