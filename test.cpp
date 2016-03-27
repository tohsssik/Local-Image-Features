#include<iostream>
#include<vector>
using namespace std;

template<typename T>
void fun(vector<T>& data, T& t)
{
    data.push_back( T(t) );
}

struct A
{
    int d;
};

int main()
{
    vector<A> d;
    A a;
    a.d = 4;
    fun(d, a);
    cout<<d.size()<<endl;
}