#include <fixedpoint.h>

#include <string>

using namespace fixedpoint;

void radixHigh(){
    number<45> n;
    number<1> n1;
}

void implicitConv(){
    using namespace std::literals;
    number<21> n;
    n = "14::124ad"s;
    n += "10::10"s;
    n = 12.58;
    n -= 54.26;
}

int main(){
    radixHigh();
    implicitConv();
    return 0;
}
