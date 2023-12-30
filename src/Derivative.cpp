#include "cmath"

float Derivative_1realize(float A, float deltaX){
    return (float)(cos(A + deltaX) - cos(A))/deltaX;
}


float Derivative_2realize(float A, float deltaX){
    return (float)(cos(A + deltaX) - cos(A - deltaX))/(2.f*deltaX);
}