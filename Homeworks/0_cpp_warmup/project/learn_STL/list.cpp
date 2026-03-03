#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

// 构造函数示例
# if 0
int main()
{
    list<int> l1;
    cout << "l1 size: " << l1.size() << endl;

    list<int> l2(5, 10); // 5个元素，每个元素的值为10
    cout << "l2 size: " << l2.size() << endl;

    list<int> l3(l2);    // 指定大小和初始值的构造函数
    cout << "l3 size: " << l3.size() << endl;

    int arr[]={1, 2, 3, 4, 5};
    list<int> l4(arr, arr + 5); // 从数组构造列表
    cout << "l4 size: " << l4.size() << endl;

    list<int> l5={4,5,6};
    cout << "l5 size: " << l5.size() << endl;
    for(int x:l5)
    {
        cout << x << " ";
    }
    return 0;
}
# endif

// 元素增删操作
# if 0
int main()
{
    list<int> l = {10,20,30};
    l.push_front(5);
    l.push_back(40);
    cout << "List after push_front and push_back: ";
    for(int x:l)
    {
        cout << x << " ";
    }
    // 任意位置插入元素
    auto it = l.begin();
    advance(it, 2); // 将迭代器移动到第三个元素位置
    l.insert(it, 25); // 在第三个元素位置插入25
    cout << "\nList after insert: ";
    for(int x:l)    {
        cout << x << " ";
    }
    // 任意位置插入相同的n个元素
    it=l.begin();
    advance(it,2); // 将迭代器移动到第三个元素位置
    l.insert(it,3,15); // 在第三个元素位置插入3个15
    cout << "\nList after insert n elements: ";
    for(int x:l)    {
        cout << x << " ";
    }
    
    // 插入一个向量范围内的元素
    vector<int> v = {50, 60, 70};
    l.insert(l.end(), v.begin(), v.end()); // 在列表末尾插入向量中的元素
    cout << "\nList after inserting vector elements: ";
    for(int x:l)    {
        cout << x << " ";
    }

    // 删除元素
    l.pop_front(); // 删除第一个元素
    l.pop_back();  // 删除最后一个元素
    cout << "\nList after pop_front and pop_back: ";
    for(int x:l)    {
        cout << x << " ";
    }

    // 删除指定位置的元素
    it = l.begin();
    advance(it,2);
    l.erase(it); // 删除第三个元素
    cout << "\nList after erase: ";
    for(int x:l)    {
        cout << x << " ";
    }

    cout << endl;
    return 0;

}

# endif


# if 1

int main()
{
    // 多个元素赋值
    list<int> l; // 创建一个包含5个元素的列表
    l.assign(5, 100); // 将列表中的元素赋值为100
    
}

# endif