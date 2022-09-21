#include "ComplexNum.h"

ComplexNum::ComplexNum(float Re, float Im) {
    this->Re = Re;
    this->Im = Im;
}

float ComplexNum::GetIm() {
    return Im;
}

float ComplexNum::GetRe() {
    return Re;
}

void ComplexNum::SetIm(float Im) {
    this->Im = Im;
}

void ComplexNum::SetRe(float Re) {
    this->Re = Re;
}

float ComplexNum::GetAbs() {
    return sqrt(Re * Re + Im * Im);
}

ComplexNum ComplexNum::operator+(ComplexNum cn) {
    return ComplexNum(this->Re + cn.Re, this->Im + cn.Im);
}

ComplexNum ComplexNum::operator+(float n) {
    return ComplexNum(this->Re + n, this->Im);
}

ComplexNum ComplexNum::operator-(ComplexNum cn) {
    return ComplexNum(this->Re - cn.Re, this->Im - cn.Im);
}

ComplexNum ComplexNum::operator*(ComplexNum cn) {
    return ComplexNum(this->Re * cn.Re - this->Im * cn.Im, this->Re * cn.Im + cn.Re * this->Im);
}

ComplexNum ComplexNum::operator*(float n) {
    return ComplexNum(n * this->Re, n * this->Im);
}