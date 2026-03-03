#include <iostream>
#include <vector>
#include <algorithm> // <algorithm> 头文件提供了算法的支持

using namespace std;
# if 0
int main()
{
    // 构造函数示例
    # if 0

    // 1. 默认构造函数，创建一个空的 vector
    vector<int> v1; 
    cout << "v1 size: " << v1.size() << ", capacity: " << v1.capacity() << endl;
    // capacity() 返回当前分配的存储空间大小，而 size() 返回实际存储的元素数量

    // 2.指定大小，值初始化
    vector<int> v2(5);  // 创建一个包含 5 个元素的 vector，默认值为 0
    cout << "v2: ";
    for(int x:v2)
    {
        cout << x<< " ";
        cout<<endl;
    }

    // 3.指定大小，指定初始值
    vector<int> v3(5,10); // 创建一个包含 5 个元素的 vector，初始值为 10
    cout << "v3: ";
    for(int x:v3)
    {
        cout << x<< " ";
        cout<<endl;
    }

    // 4.复制构造
    vector<int> v4(v3);
    cout << "v4: (copy of v3) ";
    for(int x:v4)
    {
        cout << x<< " ";
        cout<<endl;
    }

    // 5.区间构造
    int arr[] = {1,2,3,4,5};
    vector<int> v5(arr,arr+3);
    cout << "v5: (from array) ";
    for(int x:v5)
    {
        cout << x<< " ";
        cout<<endl;
    }

    // 6.移动构造
    vector<int> v6 = std::move(v5);  // 移动构造函数，与复制构造不同，v5 的资源被转移到 v6，v5 变为空
    cout << "v6: (move from v5) ";
    for(int x:v6)
    {
        cout << x<< " ";
        cout<<endl;
    }

    // 列表初始化
    vector<int> v7 = {1,2,3,4,5};
    cout << "v7: (list initialization) ";
    for(int x:v7)
    {
        cout << x<< " ";
        cout<<endl;
    }

    # endif
    // 元素增删操作
    # if 0
    // 增加操作
    vector<int> v = {10,20,30};
    v.push_back(40); // 在末尾添加元素,使用push_back()
    auto it = v.begin()+2; // auto 关键字让编译器自动推断 it 的类型，这里 it 是一个指向 vector<int> 的迭代器，指向第三个元素（值为 30）
    v.insert(it,25); // 在指定位置插入元素,使用insert()
    v.insert(v.begin(),2,5); // 在开头插入两个 5
    for(int x: v)
    {
        cout << x << " ";
    }
    cout<<endl;

    // 删除操作
    v.pop_back(); // 删除末尾元素,使用pop_back()
    v.erase(v.begin()+1); // 删除指定位置的元素,使用erase()
    v.erase(v.begin(),v.begin()+2); // 删除一个范围内的元素,
    for(int x: v)
    {
        cout << x << " ";
    }
    cout<<endl;
    v.clear(); // 清空 vector 中的所有元素,使用clear()
    return 0;
    # endif

    // 访问和遍历方式
    # if 0

    vector<int> v={100,200,300};

    cout << "v[0]: " << v[0] << endl; // 使用下标访问元素
    cout << "v.at(1): " << v.at(1) << endl; // 使用 at() 方法访问元素，提供边界检查
    cout << "v.front(): " << v.front() << endl; // 访问第一个元素
    cout << "v.back(): " << v.back() << endl; // 访问最后一个元素
    cout << "Forward: ";
    for(auto it = v.begin();it!= v.end();++it) // 使用迭代器正向遍历
    {
        cout << *it << " ";  // *it 表示迭代器指向的元素，加*号是解引用操作，获取迭代器指向的元素值
    }
    cout << endl;

    cout << "Reverse: ";
    for(auto it = v.rbegin();it!= v.rend();++it) // 使用反向迭代器逆向遍历
    {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Range-based for loop: ";
    for(int x: v) // 使用范围-based for 循环遍历
    {
        cout << x << " ";
    }
    cout << endl;
    return 0;

    # endif

    // 容量管理
    # if 0

    vector<int> v;
    cout << "Initial size: " << v.size() << ", capacity: " << v.capacity() << endl;

    // reserve: 预分配内存，减少频繁的内存分配，提高性能，不改变size
    v.reserve(10);
    cout << "After reserve: " << v.size() << ", capacity: " << v.capacity() << endl;

    for(int i=0;i<8;i++)
    {
        v.push_back(i);
        cout << "After push_back(" << i << "): size: " << v.size() << ", capacity: " << v.capacity() << endl;
    }

    v.reserve(5); // 预分配内存，减少频繁的内存分配，提高性能，不改变size
    cout << "After reserve(5): size: " << v.size() << ", capacity: " << v.capacity() << endl;

    v.shrink_to_fit(); // 请求减少容量以适应当前大小，可能会释放未使用的内存，但不保证一定会减少容量
    cout << "After shrink_to_fit: size: " << v.size() << ", capacity: " << v.capacity() << endl;

    for(int x: v)
    {
        cout << x << " ";
    }
    cout << endl;
    return 0;

    # endif




    vector<Point> points;
    points.push_back(Point(3, 4));
    points.push_back(Point(1, 2));
    points.push_back(Point(2, 1));

    // 排序
    sort(points.begin(), points.end());

    // 遍历输出
    cout << "排序后的点集:" << endl;
    for (const auto& p : points) {
        cout << "(" << p.x << ", " << p.y << ")" << endl;
    }

    return 0;
    
}

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    
    // 重载 < 用于 sort
    bool operator<(const Point& other) const {
        return x != other.x ? x < other.x : y < other.y;
    }
};
# endif