我已经按“接口正确性 + 极端情况 + 效率 + 编程规范”逐条审查了 `src/executables/2_EfficientDArray/DArray.cpp`。下面是该文件中**不妥/错误点清单**（每条含位置、问题、影响、修复建议）：

1. **`m_nMax` 从未初始化（致命）**  
   - 位置：`Init()`、所有依赖 `m_nMax` 的函数（构造、`SetSize`、`PushBack`、`InsertAt`、拷贝/赋值等）。  
   - 问题：`Init()` 只设置了 `m_pData` 与 `m_nSize`，没有给 `m_nMax` 赋初值。  
   - 影响：后续 `assert(nSize <= m_nMax)` 等判断基于未定义值，行为未定义（UB）。  
   - 建议：在 `Init()` 中设置 `m_nMax = 0;`，并通过 `Reserve()` 管理容量。

2. **`Reserve(int)` 在头文件声明但未实现（设计与作业目标不符）**  
   - 位置：`DArray.h` 声明、`DArray.cpp` 无定义。  
   - 问题：高效动态数组应基于“容量(capacity)”扩容，当前实现完全没用 `Reserve`。  
   - 影响：不符合“EfficientDArray”的核心要求。  
   - 建议：实现 `Reserve`，并在扩容时按倍增策略（如 `max(1, 2*m_nMax)`）分配。

3. **带参构造函数错误使用 `m_nMax` 且容量逻辑缺失**  
   - 位置：`DArray::DArray(int nSize, double dValue)`。  
   - 问题：先 `assert(nSize <= m_nMax)`（此时 `m_nMax` 未初始化）；再 `new double[nSize]` 但不更新容量。  
   - 影响：断言无意义/可能错误触发；容量状态不一致。  
   - 建议：先校验 `nSize>=0`，再 `Reserve(nSize)`，最后设置 `m_nSize=nSize` 并填充值。

4. **拷贝构造函数同样依赖未初始化容量，且未维护容量语义**  
   - 位置：`DArray::DArray(const DArray& arr)`。  
   - 问题：`assert(arr.m_nSize <= m_nMax)` 仍用未初始化 `m_nMax`；只按 size 分配。  
   - 影响：潜在 UB；容量信息缺失。  
   - 建议：`Reserve(arr.m_nSize)`，复制数据，`m_nSize = arr.m_nSize`。

5. **`SetSize` 出现赋值写成比较（严重逻辑 bug）**  
   - 位置：`if(nSize = m_nSize)`。  
   - 问题：把 `==` 写成 `=`。  
   - 影响：会改写 `nSize`，并且多数情况下条件恒真，后续逻辑被短路，导致缩容/扩容行为错误。  
   - 建议：改为 `if (nSize == m_nSize) return;`。

6. **`SetSize` 未检查负数尺寸**  
   - 位置：`SetSize(int nSize)`。  
   - 问题：没有 `nSize >= 0` 校验。  
   - 影响：`new double[nSize]` 对负数会转成超大无符号值，可能异常或崩溃。  
   - 建议：先判断 `nSize < 0` 并处理（`assert` 或抛异常）。

7. **`GetAt` 断言条件写反且排除了下标 0（致命）**  
   - 位置：`assert(nIndex > 0 && nIndex >= m_nSize);`。  
   - 问题：正确条件应为 `0 <= nIndex && nIndex < m_nSize`。你这里完全反了。  
   - 影响：合法下标常失败；非法下标反而可能通过后越界访问。  
   - 建议：改为 `assert(nIndex >= 0 && nIndex < m_nSize);`。

8. **`SetAt` 与 `GetAt` 同样断言反了**  
   - 位置：`assert(nIndex > 0 && nIndex >= m_nSize);`。  
   - 影响：同上，导致合法赋值失败、非法越界写入风险高。  
   - 建议：同第7条。

9. **`operator[]` 上界判断越界 1 位**  
   - 位置：两个 `operator[]`。  
   - 问题：使用 `nIndex <= m_nSize`，应是 `< m_nSize`。  
   - 影响：允许访问 `m_pData[m_nSize]`（越界）。  
   - 建议：改为 `assert(nIndex >= 0 && nIndex < m_nSize);`。

10. **`InsertAt` 无法在空数组 `InsertAt(0, x)`（与 main 测试冲突）**  
   - 位置：`assert(nIndex >= 0 && nIndex <= m_nSize - 1 ...)`。  
   - 问题：当 `m_nSize=0` 时，条件变成 `nIndex<=-1`，`InsertAt(0,...)` 必失败。  
   - 影响：`main.cpp` 第一段 `a.InsertAt(0, 2.1);` 在 Debug 下直接断言失败。  
   - 建议：插入合法范围应为 `0 <= nIndex <= m_nSize`。

11. **`InsertAt` 不允许尾插位置 `nIndex==m_nSize`**  
   - 位置：同上。  
   - 问题：标准插入语义通常支持尾部插入。  
   - 影响：接口可用性差，和常见容器行为不一致。  
   - 建议：允许 `nIndex == m_nSize`。

12. **`operator=` 未处理自赋值（致命潜在 UB）**  
   - 位置：`DArray::operator=`。  
   - 问题：先 `delete[] m_pData`，再通过 `arr[i]` 读源对象；若 `arr` 就是 `*this`，读取已释放内存。  
   - 影响：自赋值崩溃/数据损坏。  
   - 建议：先 `if (this == &arr) return *this;`，或使用“先分配复制再释放”策略（copy-and-swap 更稳）。

13. **`operator=` 异常安全差**  
   - 位置：`operator=`。  
   - 问题：先释放旧内存，再 `new`；若 `new` 抛异常，对象已丢失原数据。  
   - 影响：对象可能处于不完整状态。  
   - 建议：先分配新内存并复制成功后再替换旧内存（强异常安全）。

14. **所有增删改操作都“每次重新分配”——效率不达标**  
   - 位置：`PushBack`、`InsertAt`、`DeleteAt`、`SetSize`。  
   - 问题：每次操作都 `new[]/delete[]`，时间复杂度退化（连续 `PushBack` 变 O(n²)）。  
   - 影响：与“EfficientDArray”目标相违背。  
   - 建议：维护 `m_nMax` 容量，只有 size 超过 capacity 才扩容；扩容倍增。

15. **`DeleteAt` 对 size=1 时虽可工作，但容量管理缺失**  
   - 位置：`DeleteAt`。  
   - 问题：删除后强制分配新数组 `m_nSize-1`，即使只是逻辑删除也触发重分配。  
   - 影响：低效、频繁内存抖动。  
   - 建议：只做元素搬移并 `--m_nSize`；必要时按策略缩容，而不是每次缩容。

16. **`assert` 被当作完整输入校验机制使用（发布版失效）**  
   - 位置：全文件多处。  
   - 问题：`assert` 在 Release 下通常被移除。  
   - 影响：非法输入在发布版不受保护，越界风险暴露。  
   - 建议：课程作业可保留 assert，但关键接口建议补充运行时保护（或明确定义前置条件并保证调用方）。

17. **局部变量命名风格不规范**  
   - 位置：多处 `double *m_new;`。  
   - 问题：`m_` 前缀通常保留给成员变量，局部变量不应使用。  
   - 影响：可读性差，易与成员混淆。  
   - 建议：改成 `newData`/`pNewData` 等。

18. **代码重复较多，缺少复用（可维护性问题）**  
   - 位置：`SetSize/PushBack/InsertAt/DeleteAt` 都手写分配+拷贝。  
   - 问题：重复逻辑易引入不一致 bug。  
   - 建议：把扩容、搬移等抽到 `Reserve`/辅助逻辑中统一维护。

---

**和作业要求的对应结论**：  
- 从“功能正确性”看：当前实现有多处边界与断言错误，`main.cpp` 中首个 `InsertAt(0,...)` 就会失败。  
- 从“高效动态数组”看：当前实现基本是“每次操作都重分配”的低效版本，不满足 Efficient 的核心目标。  
- 从“编程规范与健壮性”看：存在自赋值、异常安全、未初始化成员、命名风格等问题。

如果你愿意，我下一步可以直接给你一版**符合当前接口、支持容量扩展、通过 main 测试并覆盖极端情况**的 `DArray.cpp` 修正版（并附上每处修改理由）。

```cpp

// DArray 类的实现
#include "DArray.h"
#include <iostream>
#include <cassert>

// 默认构造函数
DArray::DArray() {
	Init();
}

// 分配足够的内存，更新m_nMax
void DArray::Reserve(int nSize) { 
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

// 创建并初始化一个带默认值的数组
DArray::DArray(int nSize, double dValue) {
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

DArray::DArray(const DArray& arr) {
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

// 析构函数
DArray::~DArray() {
	Free();
}

// 打印数组中的元素
void DArray::Print() const {
	// 待实现
	for(int i=0; i < m_nSize; i++)
	{
		std::cout << m_pData[i] << " ";
	}
	std::cout << std::endl;
}

// 初始化数组
void DArray::Init() {
	// 待实现
	m_pData = nullptr;
	m_nSize = 0;

	m_nMax = 0;
}

// 释放数组内存
void DArray::Free() {
	// 待实现
	delete[] m_pData;
	Init();
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

// 获取指定下标处的元素
const double& DArray::GetAt(int nIndex) const {
	// 待实现
	assert(nIndex > 0 && nIndex >= m_nSize);
	return m_pData[nIndex];
}

// 设置指定元素的值
void DArray::SetAt(int nIndex, double dValue) {
	// 待实现
	assert(nIndex > 0 && nIndex >= m_nSize);
	m_pData[nIndex] = dValue;
}

// 重载运算符 '[]'
double& DArray::operator[](int nIndex) {
	// 待实现
	assert(nIndex >= 0 && nIndex /*<=*/ < m_nSize);
	return m_pData[nIndex];
}

// 重载运算符 '[]'
const double& DArray::operator[](int nIndex) const {
	// 待实现
	assert(nIndex >= 0 && nIndex /*<=*/ < m_nSize);
	return m_pData[nIndex];
}

// 在数组末尾添加一个新元素
void DArray::PushBack(double dValue) {
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

// 删除指定下标处的元素
void DArray::DeleteAt(int nIndex) {
	// 待实现
	assert(nIndex+1 <= m_nSize && nIndex >= 0);
	double* m_new = new double [m_nSize - 1];
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

// 在指定下标处插入一个新元素
void DArray::InsertAt(int nIndex, double dValue) {
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

// 重载赋值运算符 '='
DArray& DArray::operator = (const DArray& arr) {
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

void DArray::Reserve(int nSize) {
	assert(nSize >= 0);
	if (nSize <= m_nMax)
	{
		return;
	}
	double* m_new = new double [nSize];
	for(int i = 0;i < m_nSize; i++)
	{
		m_new[i] = m_pData[i];
	}
	delete[] m_pData;
	m_pData = m_new;
	m_nMax = nSize;
}

```

```bash

(base) hgl@hgl-XiaoXin-14-IRL8:/media/hgl/Ubuntu/中科大/CG/Homeworks/0_cpp_warmup/project/bin$ "/media/hgl/Ubuntu/中科大/CG/Homeworks/0_cpp_warmup/project/bin/2_EfficientDArray"
2.1 
2.1 3 3.1 3.2 
3 3.1 3.2 
4.1 3 3.1 3.2 
4.1 3 3.1 3.2 
4.1 3 3.1 3.2 
4.1 3 3.1 3.2 
4.1 3 3.1 3.2 
21 

22 0 0 0 0 
100 97 98 99 

```

