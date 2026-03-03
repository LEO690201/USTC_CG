#include <map>
#include <iostream>
#include <string>

# if 0
int main()
{
    std::map <int, std::string> mapPerson;  // map 对象是一个模版类，需要关键字和存储对象两个模版参数
    typedef std::map<int, std::string> MAP_INI_STRING;  // 对模版进行类型定义使其使用方便
    MAP_INI_STRING person;// 定义一个对象，使用类型定义的模版类

    mapPerson.insert(std::pair <int, std::string> (1,"hgl"));
    mapPerson.insert(std::pair <int, std::string> (2,"xxy"));
    mapPerson.insert(std::map <int, std::string> ::value_type(3,"xxy")); 
    // value_type 是 map 的内部类型，用来表示键值对
    // 也可以使用 std::pair<int, std::string> 作为 value_type 的别名

    // 遍历 map 对象
    std::map <int, std::string> ::iterator it;  
    // 定义一个迭代器对象，iterator是map的迭代器类型，it是迭代器对象的名字
        std::map <int, std::string> ::iterator itEnd; // 定义一个迭代器对象，用于遍历结束的位置
        it = mapPerson.begin();
        itEnd = mapPerson.end();
        while (it != itEnd)
        {
            std::cout << it -> first << " " << it -> second << std::endl;
            it++;
        }
    
    // 反向迭代
    std::map <int, std::string> ::reverse_iterator rit;
    for (rit = mapPerson.rbegin(); rit != mapPerson.rend(); rit++)
    {
        std::cout << rit -> first << " " << rit -> second << std::endl;
    }

    // map中元素的查找
    std:: map<int,std::string> ::iterator l_it;
    l_it=mapPerson.find(112);
    if(l_it==mapPerson.end())
    {
        std::cout << "we do not find 112" << std::endl;
    }
    else
    {
        std::cout << "we find 112, the value is " << l_it -> second << std::endl;
    }

    // map中元素的删除
    std::map<int, std::string> ::iterator del_it;
    del_it=mapPerson.find(112);  // mapPerson：是指map对象，find：是查找元素的函数
    if(del_it!=mapPerson.end())
    {
        mapPerson.erase(del_it);  // erase：是删除元素的函数，del_it：是要删除元素的迭代器
        std::cout << "we delete 112" << std::endl;
    }
    else
    {
        std::cout << "we do not find 112, so we can not delete it" << std::endl;
    }

    return 0;
}
# endif

// swap 用法：
# if 0

using namespace std;

int main()
{
    map<int, int> m1,m2,m3;
    map<int, int> :: iterator m1_Iter;
    m1.insert(pair<int,int> (1,10));
    m1.insert(pair<int,int> (2,20));
    m1.insert(pair<int,int> (3,30));
    m2.insert(pair<int,int> (10,100));
    m2.insert(pair<int,int> (20,200));
    m2.insert(pair<int,int> (30,300));
    cout << "the original map m1 is: " ;
    for(m1_Iter = m1.begin(); m1_Iter!=m1.end();m1_Iter++)
    {
        cout << " " << m1_Iter -> second;
        cout << "." << endl;
    }
    m1.swap(m2);  // swap：是交换两个map对象的函数，m1和m2是要交换的两个map对象
    cout << "the map m1 after swap is: " ;
    for(m1_Iter = m1.begin();m1_Iter!=m1.end();m1_Iter++)
    {
        cout << " " << m1_Iter -> second;
        cout << "." << endl;
    }
    cout << "the map m2 after swap is: " ;
    for(m1_Iter = m2.begin();m1_Iter!=m2.end();m1_Iter++)
    {
        cout << " " << m1_Iter -> second;
        cout << "." << endl;
    }
    swap(m1,m3);  // swap：是交换两个map对象的函数，m1和m3是要交换的两个map对象
    cout << "the map m1 after swap is: " ;
    for(m1_Iter = m1.begin();m1_Iter!=m1.end();m1_Iter++)
    {
        cout << " " << m1_Iter -> second;
        cout << "." << endl;
    }
    return 0;
}

# endif

// sort 用法：
# if 0
using namespace std;
int main()
{
    map<int,int>m1;
    map<int,int> ::iterator m1_Iter;
    m1.insert(pair<int,int> (1,20));
    m1.insert(pair<int,int> (4,40));
    m1.insert(pair<int,int> (3,60));
    m1.insert(pair<int,int> (2,50));
    m1.insert(pair<int,int> (6,40));
    m1.insert(pair<int,int> (7,30));
    cout << "the original map m1 is: " << endl;
    for(m1_Iter = m1.begin(); m1_Iter!=m1.end();m1_Iter++)
    {
        cout << m1_Iter -> first << " " << m1_Iter -> second << endl;
    }
}

# endif
