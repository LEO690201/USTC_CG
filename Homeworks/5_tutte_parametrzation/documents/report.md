# Tutte Parameterization 实验报告

## 1. 算法说明

Tutte Parameterization 是一种基于图的参数化算法，用于将盘状（拓扑同胚于圆盘）的三维网格模型展开到二维平面上。

### 1.1 边界映射 (Boundary Mapping)
为保证参数化的有效性并防止三角面片翻转，我们首先需要将网格模型的边界点映射到二维平面上的凸多边形边界。本实验实现了两种典型的凸边界映射：
- **正方形边界映射 (Square Domain)**：将边界点根据其相对参数或位置映射到 $[-1, 1] \times [-1, 1]$ 区域的正方形上。将边界点分为四段，分别贴合正方形的四条边。
- **圆形边界映射 (Circular Domain)**：基于极坐标计算，将所有边界顶点均匀或等弧长地分布在一个单位圆盘周长上（如 $r=1$）。

### 1.2 建立与求解稀疏线性方程组
内部顶点的二维参数是通过最小化一定的能量来得到的。根据 Tutte 定理，所有的内部点必须是其相邻点的凸组合：
$$ p_i = \sum_{j \in N(i)} w_{ij} p_j $$
通过重新排列：
$$ \sum_{j \in N(i)} w_{ij} (p_i - p_j) = 0 $$
其中 $w_{ij}$ 是各个边对应的权重。我们通过构建以权重为系数矩阵的拉普拉斯光顺方程（Laplace equation）$LX = B$ 进行求解。其中边界点的坐标作为边界条件固定在右端项 $B$ 中。

### 1.3 权重计算方法
常见的有三种权重策略，本实验分别实现了它们：
1. **均匀权重 (Uniform Weights)**：
   最简单的形式，直接设置边权 $w_{ij} = 1$。方程的形式变为：
   $$ p_i = \frac{1}{|N(i)|} \sum_{j \in N(i)} p_j $$
   该方法仅保证不相交，会产生较大的角度和面积畸变。
   
2. **余切权重 (Cotangent Weights)**：
   旨在减小角度形变（Conformal），权重公式为：
   $$ w_{ij} = \frac{1}{2} (\cot \alpha_{ij} + \cot \beta_{ij}) $$
   其中 $\alpha_{ij}, \beta_{ij}$ 为边 $ij$ 在两个相邻三角形上的对角。这种参数化能很好地保留局部几何特征。如果部分角度过大（钝角），它的值可能为负，偶尔回导致一些自交。

3. **Floater 权重 (Mean Value Weights)**：
   在任何情况下保证权重非负（进而参数化不发生翻转自交），并且同样具有较好的角度保持性。其公式基于顶点的局部极坐标展开。
   $$ w_{ij} = \frac{\tan(\gamma_{ij}/2) + \tan(\delta_{ij}/2)}{||v_i - v_j||} $$

通过 Eigen 的 \`SparseLU\` 进行预分解并快速求解方程组，从而获得所有内部点的 $u, v$ 纹理坐标，以此改变各个节点的拓扑和位置，渲染出参数化的最终效果。

## 2. 核心代码

实验的核心逻辑在建立边界映射（`hw5_boundary_map.cpp`）和根据不同权重求解内部节点（`hw5_param.cpp`）中。通过以下步骤实现参数化：
1. 使用 `igl::boundary_loop` 获取所有的边界顶点按顺序的索引集合。
2. 按照正方形/圆形公式构建边界顶点的参数坐标。
3. 枚举每个点 $i$ 和相连的边 $(i, j)$，依据三种算法，求出 $w_{ij}$ 并存入 Eigen 的稀疏矩阵形式（`Eigen::SparseMatrix<double>`的三元组 `Triplet` 列表）。
4. 分别对 $x$ 和 $y$ 维度的坐标建立约束并利用 `Eigen::SparseLU` / `Eigen::SimplicialLDLT` 进行求解。

## 3. Ubuntu 下的跨平台修改
原 `Framework3D/Ruzino` 主要针对 Windows 进行的开发与依赖管理：
- 在 `configure.py` 中强行设定了 `d3d12` (Direct3D 12) 的依赖下载。
- 通过引入 Python 的 `os.name` 进行操作系统判断：
  ```python
  if os.name == "nt":
      args.library = [...] # 含 d3d12
  else:
      args.library = ["openusd", "slang", "dxc", "embree"]
  ```
- 修改了其 Python 构件与下载流程，去除了不支持 Linux 的 Direct X 与 DXCompiler 环境包对于平台特定 API 的硬挂载，将框架代码与相关的 C++ 开发库支持改为了适合 Ubuntu 工具链和 GCC 环境的运行版本。
