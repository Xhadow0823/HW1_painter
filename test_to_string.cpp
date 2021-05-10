// test_to_string.cpp
#include <cstdio>
#include <string>

int main (void){
    float f = 5.0;

    fprintf(stdout, "num= %s\n", std::to_string(f).substr(0, 3).c_str());

    return 0;
}