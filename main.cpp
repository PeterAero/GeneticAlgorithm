#include <iostream>
using namespace std;


int calc_add(int a, int b){

    return a + b;

}

int main(int argc, char *argv[])
{
    int a, b, c = 0;
    a = 3;
    b = 4;
    c = calc_add(a, b);
    std::cout << "Result is: " << c << std::endl;
    return 0;
}
