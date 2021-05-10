// Color.cpp
#include <string>

#ifndef _ColorS_
#define _ColorS_
struct Color{
    float r=0.0, g=0.0, b=0.0, a=1.0;
    Color(){}
    Color(float r, float g, float b):r(r), g(g), b(b){}
    Color(float r, float g, float b, float a):r(r), g(g), b(b), a(a){}
};
#endif