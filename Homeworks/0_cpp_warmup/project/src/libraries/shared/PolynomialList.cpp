#include "PolynomialList.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

PolynomialList::PolynomialList(const PolynomialList& other) // 这个函数用于：拷贝构造一个多项式列表
{
    // TODO
    m_Polynomial = other.m_Polynomial;
}

PolynomialList::PolynomialList(const string& file) // 从文件中读取多项式列表
{
    // TODO
    ReadFromFile(file);
}

PolynomialList::PolynomialList(const double* cof, const int* deg, int n) // 从系数数组和对应的指数数组中构造一个多项式列表
{
    // TODO
    for(int i=0;i<n;i++)
    {
        AddOneTerm(Term(deg[i],cof[i]));
    }
}

PolynomialList::PolynomialList(const vector<int>& deg, const vector<double>& cof) // 从系数向量和对应的指数向量中构造一个多项式列表
{
    // TODO
    auto n = min(deg.size(), cof.size());
    for(auto i = 0;i < n;i++)
    {
        AddOneTerm(Term(deg[i], cof[i]));
    }
}

double PolynomialList::coff(int i) const // 获取指数为i的项的系数
{
    // TODO、
    for(auto it = m_Polynomial.begin();it != m_Polynomial.end();it++)
    {
        if(it->deg == i)
        {
            return it->cof;
        }
    }
    # if 0
    for(auto&term : m_Polynomail)
    {
        if(term.deg == i)
        {
            return term.cof;
        }
    }
    # endif
    return 0.; // you should return a correct value
}

double& PolynomialList::coff(int i) // 获取指数为i的项的系数的引用，以便修改
{
    // TODO
    for (auto it = m_Polynomial.begin();it != m_Polynomial.end();it++)
    {
        if(it->deg == i)
        {
            return it->cof;
        } 
    }
    # if 1
    return AddOneTerm(Term(i,0.0)).cof;
    # endif
}

void PolynomialList::compress() // 将多项式列表中的同类项合并，并按照指数从高到低排序
{
    // TODO
    // 排序
    m_Polynomial.sort([](const Term& a, const Term& b)
    {
        return a.deg > b.deg;
    });  // 这里是按照指数从高到低排序的lambda表达式，sort函数会根据这个表达式来比较两个元素的大小，从而进行排序，这里的a.deg > b.deg表示如果a的指数大于b的指数，则a应该排在b的前面，也就是从高到低排序
    // 合并同类项
    if(m_Polynomial.empty())  // 如果多项式列表为空，则直接返回
    {
        return;
    }
    auto it = m_Polynomial.begin();  // 指向当前元素的迭代器
    auto next_it = it;  // 指向下一个元素的迭代器
    next_it++;          // 指向下一个元素
    while (next_it != m_Polynomial.end())
    {
        if(it->deg == next_it->deg)
        {
            it->cof += next_it->cof;
            next_it = m_Polynomial.erase(next_it);  // 删除下一个元素
        }
        else
        {
            if(abs(it->cof) < 1e-9)
            {
                it = m_Polynomial.erase(it);
                next_it = it;
                if(next_it != m_Polynomial.end())
                {
                    ++next_it; 
                }
            }
            else
            {
                it = next_it;
                ++next_it;
            }
        }
    }
    if(it != m_Polynomial.end() && abs(it->cof) < 1e-9)
    {
        m_Polynomial.erase(it);
    }
}

PolynomialList PolynomialList::operator+(const PolynomialList& right) const // 多项式列表的加法运算符重载
{
    // TODO
    PolynomialList result = *this; // 拷贝一份当前元素
    for (const auto& term : right.m_Polynomial)
    {
        result.AddOneTerm(term);
    }
    return result; // you should return a correct value
}

PolynomialList PolynomialList::operator-(const PolynomialList& right) const // 多项式列表的减法运算符重载
{
    // TODO
    PolynomialList result = *this;
    for (const auto& term : right.m_Polynomial) 
    {
        Term negTerm = term;
        negTerm.cof = -negTerm.cof;
        result.AddOneTerm(negTerm);
    }    
    return result; // you should return a correct value
}

PolynomialList PolynomialList::operator*(const PolynomialList& right) const // 多项式列表的乘法运算符重载
{
    // TODO
    PolynomialList result;
    for (const auto& term1 : this->m_Polynomial) 
    {
        for (const auto& term2 : right.m_Polynomial) 
        {
            Term newTerm;
            newTerm.deg = term1.deg + term2.deg;
            newTerm.cof = term1.cof * term2.cof;
            result.AddOneTerm(newTerm);
        }
    }
    result.compress(); // 可以根据 Terms 内部实现决定是否需要显式调用
    return result;
}

PolynomialList& PolynomialList::operator=(const PolynomialList& right) // 多项式列表的赋值运算符重载
{
    // TODO
    if(this != &right)
    {
        m_Polynomial = right.m_Polynomial;
    }
    return *this;
}

void PolynomialList::Print() const // 打印多项式列表的内容，按照指数从高到低的顺序输出每一项的系数和指数
{
    // TODO
    bool first = true;
    if (m_Polynomial.empty()) {
        cout << "0" << endl;
        return;
    }

    for (const auto& term : m_Polynomial) {
        if (!first) {
            if (term.cof >= 0) cout << " + ";
            else cout << " "; // 负号会随数字输出
        }
        cout << term.cof << "x^" << term.deg;
        first = false;
    }
    cout << endl;
}

bool PolynomialList::ReadFromFile(const string& file) // 从文件中读取多项式列表的内容，返回是否成功读取
{
    // TODO
    ifstream ifs(file);  // 打开文件
    if(! ifs.is_open())
    {
        return false; // 打开文件失败
    }
    m_Polynomial.clear(); // 清空当前多项式列表

    // 读取第一行的header
    string header;  
    int numTerms;  // 读取第一行的项数 
    if(!(ifs >> header >> numTerms))
    {
        return false;
    }
    if(header != "P")
    {
        return false; // 文件格式错误
    }
    int deg;  // 读取每一项的指数
    double cof;  // 读取每一项的系数

    // 循环读取指定数量的项
    for(int i=0;i< numTerms;i++)
    {
        if(ifs >> deg >> cof)
        {
            AddOneTerm(Term(deg, cof)); // 将读取的项添加到多项式列表中(term是个list的元素，包含指数和系数)
        }
        else{
            break;
        }
    }
    return true; // you should return a correct value
}

PolynomialList::Term& PolynomialList::AddOneTerm(const Term& term) // 将一个项添加到多项式列表中，如果已经存在同指数的项，则将它们合并
{
    // TODO
    for(auto it = m_Polynomial.begin();it != m_Polynomial.end();it++)
    {
        if(it -> deg == term.deg)   // term.deg == it->deg，说明已经存在同指数的项，将它们合并，
        // 这里的term.list是一个list的元素，包含指数和系数，it->deg是list中元素的指数，it->cof是list中元素的系数
        {
            it->cof += term.cof;
            return *it;
        }
        if(it->deg < term.deg)
        {
            auto newIt = m_Polynomial.insert(it, term);   // term.deg > it->deg，说明新项的指数比当前项的指数大，需要将新项插入到当前项之前
            // insert函数会在it之前插入一个新的元素，值为term，并返回一个指向新元素的迭代器
            return *newIt;
        }
    }
    // 如果所有项的指数都小于新项的指数，则将新项添加到末尾
    m_Polynomial.push_back(term);
    return m_Polynomial.back();
}




