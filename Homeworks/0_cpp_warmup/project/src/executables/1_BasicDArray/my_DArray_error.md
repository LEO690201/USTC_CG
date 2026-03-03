你这个 `src/executables/1_BasicDArray/DArray.cpp` 目前问题很多，而且不少是**会直接崩溃/编译不过/内存错误**的级别。下面我按“作业要求”逐条对照，并按函数详细指出不妥和错误。

---

## 一、先给结论（按作业要求对照）

你的实现当前**不满足**以下关键要求：

1. **动态内存管理正确性**（new/delete、无泄漏、无越界）❌
2. **构造/拷贝构造/析构/赋值运算符正确实现** ❌
3. **各种边界情况不崩溃**（main.cpp 的测试会触发）❌
4. **接口语义正确**（`SetSize/PushBack/DeleteAt/InsertAt/operator=` 等）❌
5. **代码规范和鲁棒性**（库代码里 `exit(0)`、返回悬空/非法引用等）❌

`#pragma once` 在头文件里是好的，这一条你是满足的。✅

---

## 二、逐函数详细问题清单

> 以下“错误级别”仅供你快速分优先级：
> - **致命**：编译错误 / UB / 高概率崩溃
> - **严重**：逻辑错误导致结果错误或内存泄漏
> - **一般**：风格、鲁棒性不足

### 1) `Init()`

```cpp
void DArray::Init() {
	for (int i=0; i < m_nSize; i++)
	{
		m_pData[i] = 0.0f;
	}
	m_nSize = 0;
}
```

- **致命**：`m_nSize` 和 `m_pData` 在进入 `Init()` 前未初始化，你却拿 `m_nSize` 做循环上界并写 `m_pData[i]`，这是未定义行为（随机内存写）。
- **正确做法**：`Init()` 应仅做“安全初始状态”设置：`m_pData = nullptr; m_nSize = 0;`，不应访问未知内存。

---

### 2) `DArray(int nSize, double dValue)`

```cpp
Init();
if (nSize <=0 ) { ... exit(0); }
else {
	m_nSize = nSize;
	for(...) m_pData[i]=dValue;
}
```

- **致命**：从未 `new` 分配数组，直接写 `m_pData[i]`，必然非法写。
- **严重**：`nSize <= 0` 直接 `exit(0)`，库类不应终止整个程序；而且 `nSize==0` 本应允许（空数组）。
- **要求不符**：没有正确体现 `new[]` 的使用。

---

### 3) 拷贝构造 `DArray(const DArray& arr)`

```cpp
Init();
if (arr.m_nSize <= 0 || arr.m_nSize > m_nSize) ...
```

- **致命**：`arr.m_nSize > m_nSize` 判断没有意义（此时 `m_nSize` 通常是 0），导致几乎所有非空拷贝都 `exit`。
- **致命**：同样未分配 `m_pData` 就写入。
- **严重**：`arr.m_nSize <= 0` 不应报错；拷贝空数组是合法场景。

---

### 4) `Free()`

```cpp
void DArray::Free() {
	m_nSize = 0;
}
```

- **致命**：没有 `delete[] m_pData`，100% 内存泄漏。
- **严重**：未把指针置空，后续可能悬空指针。

---

### 5) `SetSize(int nSize)`

```cpp
Init();
m_nSize = nSize;
for (...) m_pData[i]=0;
```

- **致命**：再次未分配内存就写 `m_pData[i]`。
- **严重**：`SetSize` 不应直接 `Init()`（会丢失旧数据语义混乱），也没 `Free()` 旧内存，设计错误。
- **严重**：未处理 `nSize<0`。

---

### 6) `GetAt(int nIndex) const`

```cpp
if (...) { ... return 0; }
```

- **致命（编译层面）**：返回类型是 `const double&`，`return 0;` 是返回对临时量的引用，非法/悬空引用。
- **严重**：越界处理方式不合理（打印 ERROR 后继续返回引用）。

---

### 7) `SetAt(int nIndex, double dValue)`

- **一般/严重**：仅打印错误，不中断；若上层依赖修改成功，会产生静默逻辑错误。
- 更稳妥方式：`assert` 或抛异常，至少保证调试时可定位。

---

### 8) `operator[](int) const`

```cpp
return m_pData[nIndex];
```

- **严重**：完全无边界检查，越界直接 UB。
- **接口不完整**：头文件声明了非常量版 `double& operator[](int)`，你**没有实现**，会链接错误。

---

### 9) `PushBack(double dValue)`

```cpp
m_nSize+=1;
m_pData[m_nSize-1]=dValue;
```

- **致命**：没有扩容/重分配，直接写越界。
- **严重**：空数组情况下 `m_pData` 可能是空/野指针，直接崩。

---

### 10) `DeleteAt(int nIndex)`

```cpp
for(int i = nIndex+1;i<m_nSize-1;i++)
	m_pData[i]=m_pData[i+1];
m_nSize-=1;
delete[] m_pData;
```

- **致命**：循环边界错误，应该从 `i=nIndex` 开始搬移。
- **致命**：最后 `delete[] m_pData` 直接把整块内存释放了，但没有重建新数组，也没置空。对象进入损坏状态。
- **致命**：之后析构还会再次 `delete[]`（若你修了 Free）→ double free 风险。
- **严重**：`exit(0)` 处理越界不合适。

---

### 11) `InsertAt(int nIndex, double dValue)`

```cpp
m_nSize+=1;
for(int i = nIndex; i<m_nSize; i++)
	m_pData[i+1]=m_pData[i];
```

- **致命**：未扩容直接写，越界。
- **致命**：搬移方向错误（应从后往前），否则覆盖数据。
- **严重**：循环条件会写到 `m_pData[m_nSize]`，再越界一次。
- **边界问题**：`nIndex == m_nSize`（尾插）应允许，你的判断在某些状态下不稳。

---

### 12) 赋值运算符 `operator=`

```cpp
m_pData = new double(arr.GetSize());
...
*this=*m_pData;
```

- **致命**：`new double(arr.GetSize())` 是分配**单个 double**，不是数组；应为 `new double[arr.GetSize()]`。
- **致命**：未释放原有 `m_pData`，内存泄漏。
- **致命**：未处理自赋值（`a=a`）。
- **致命（编译错误）**：`*this = *m_pData;` 右侧是 `double`，而 `operator=` 参数是 `const DArray&`，类型不匹配，基本无法编译。
- **严重**：深拷贝语义未正确实现。

---

## 三、main.cpp 会触发的典型崩溃路径

`main` 一开始就：
```cpp
DArray a;
a.InsertAt(0, 2.1);
```

默认构造后是空数组，`InsertAt` 会直接对未分配/不足空间的 `m_pData` 写入，**开局即高概率崩溃**。这与“各种情况考虑周到，不要程序崩溃”要求直接冲突。

---

## 四、风格与规范层面的不妥

1. `# include` 这种写法不常见（通常 `#include`），虽不一定错，但不规范。
2. 用 `exit(0)` 处理类内部错误，不符合封装思想，也不利于调试定位。
3. `Print`、`GetAt` 等错误处理策略不统一（有的退出，有的打印，有的直接 UB）。
4. 使用 `0.0f` 给 `double` 赋值虽可转换，但建议直接 `0.0`。

---

## 五、你接下来应优先修复的顺序（建议）

1. 先修 `Init/Free/构造/析构`，保证对象生命周期正确。
2. 修拷贝构造和 `operator=`（深拷贝 + 自赋值 + 正确 new[]/delete[]）。
3. 修 `SetSize/PushBack/DeleteAt/InsertAt`（每次变长都要重新分配并搬移数据）。
4. 统一下标越界策略（建议 `assert` 或异常）。
5. 最后用 `main.cpp` 全流程回归测试，再补极端测试（空数组、单元素、首尾插删、连续赋值链）。

---

如果你愿意，我下一步可以给你一份“**完全符合该作业要求**”的 `DArray.cpp` 参考实现（保留你当前接口，不改 `DArray.h`）。如果要我直接落地修改，请你 **toggle to Act mode**，我就可以直接帮你改并说明每一处修改原因。