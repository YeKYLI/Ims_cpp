#include <iostream>
using namespace std;

// 函数指针
int (*functionPointer) (int a, int b);

//函数一
int fun1(int a, int b)
{
    cout << a + b << endl;
}

//函数二
int fun2(int a, int b)
{
    cout << 2 * a + b << endl;
}

int main()
{
    
    functionPointer = &fun1;

    (*functionPointer)(3 , 4);

    functionPointer = fun2;

    (*functionPointer)(3 , 4);

}