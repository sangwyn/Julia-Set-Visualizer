#ifndef ALGEBRA_FRACTAL_COMPLEXNUM_H
#define ALGEBRA_FRACTAL_COMPLEXNUM_H

#include <cmath>

class ComplexNum {
private:
    float Re, Im;
public:
    ComplexNum(float Re, float Im);

    float GetRe();
    float GetIm();
    void SetRe(float Re);
    void SetIm(float Im);

    float GetAbs();

    ComplexNum operator+(ComplexNum cn);
    ComplexNum operator+(float n);
    ComplexNum operator-(ComplexNum cn);
    ComplexNum operator*(ComplexNum cn);
    ComplexNum operator*(float n);
};


#endif
