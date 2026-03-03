#include "PolynomialMap.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

using namespace std;

PolynomialMap::PolynomialMap(const PolynomialMap& other) 
{
    m_Polynomial = other.m_Polynomial;
}

PolynomialMap::PolynomialMap(const string& file) 
{
    ReadFromFile(file);
}

PolynomialMap::PolynomialMap(const double* cof, const int* deg, int n) 
{
    for (int i = 0; i < n; ++i) 
	{
        m_Polynomial[deg[i]] = cof[i]; // map 会自动处理重复的键，这里直接覆盖或更新
    }
    compress();
}

PolynomialMap::PolynomialMap(const vector<int>& deg, const vector<double>& cof) 
{
    assert(deg.size() == cof.size());
    for (size_t i = 0; i < deg.size(); ++i) 
	{
        m_Polynomial[deg[i]] = cof[i];
    }
    compress();
}

double PolynomialMap::coff(int i) const 
{
    auto it = m_Polynomial.find(i);
    if (it != m_Polynomial.end()) 
	{
        return it->second;
    }
    return 0.0;
}

double& PolynomialMap::coff(int i) 
{
    // 如果该项不存在，map会自动创建一个值为默认值(0.0)的项
    return m_Polynomial[i];
}

void PolynomialMap::compress() 
{
    auto it = m_Polynomial.begin();
    while (it != m_Polynomial.end()) 
	{
        if (std::abs(it->second) < 1e-10) 
		{ // 去除系数极小的项（接近0）
            it = m_Polynomial.erase(it);
        } 
		else 
		{
            ++it;
        }
    }
}

PolynomialMap PolynomialMap::operator+(const PolynomialMap& right) const {
    PolynomialMap result(*this);
    for (const auto& pair : right.m_Polynomial) {
        result.m_Polynomial[pair.first] += pair.second;
    }
    result.compress();
    return result;
}

PolynomialMap PolynomialMap::operator-(const PolynomialMap& right) const {
    PolynomialMap result(*this);
    for (const auto& pair : right.m_Polynomial) {
        result.m_Polynomial[pair.first] -= pair.second;
    }
    result.compress();
    return result;
}

PolynomialMap PolynomialMap::operator*(const PolynomialMap& right) const {
    PolynomialMap result;
    for (const auto& p1 : this->m_Polynomial) {
        for (const auto& p2 : right.m_Polynomial) {
            int newDeg = p1.first + p2.first;
            double newCoff = p1.second * p2.second;
            result.m_Polynomial[newDeg] += newCoff;
        }
    }
    result.compress();
    return result;
}

PolynomialMap& PolynomialMap::operator=(const PolynomialMap& right) {
    if (this != &right) {
        m_Polynomial = right.m_Polynomial;
    }
    return *this;
}

void PolynomialMap::Print() const {
    bool first = true;
    if (m_Polynomial.empty()) {
        cout << "0" << endl;
        return;
    }

    // map 默认按 key (指数) 升序排列，如果需要降序输出，可以使用反向迭代器
    for (auto it = m_Polynomial.begin(); it != m_Polynomial.end(); ++it) {
        if (!first) {
            cout << " ";
        }
        cout << "deg=" << it->first << " cof=" << it->second;
        first = false;
    }
    cout << endl;
}

bool PolynomialMap::ReadFromFile(const string& file) {
    m_Polynomial.clear();
    ifstream infile(file);
    if (!infile.is_open()) {
        cerr << "Error: Cannot open file " << file << endl;
        return false;
    }

    int n;
    if (!(infile >> n)) return false; // 读取第一行的项数

    int deg;
    double coff;
    // 假设文件格式是：
    // N
    // deg1 coff1
    // deg2 coff2
    // ...
    for (int i = 0; i < n; ++i) {
        if (infile >> deg >> coff) {
             m_Polynomial[deg] = coff;
        }
    }
    
    compress();
    return true; 
}