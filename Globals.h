#ifndef ALGEBRA_FRACTAL_GLOBALS_H
#define ALGEBRA_FRACTAL_GLOBALS_H

#include <SFML/Graphics.hpp>
#include "ComplexNum.h"

const sf::Vector2<unsigned int> kResolution = {1280, 720};

const int kMaxIterations = 100;
// ComplexNum(-1, 0)
// ComplexNum(-0.2, 0.75)
// ComplexNum(0.28, 0.0113)
const ComplexNum kC = ComplexNum(0.28, 0.0113);
const float kInf = 1e7;

const float kStartSize = 200.0f;
const int kXOffset = 45;
const int kYOffset = 45;
const int kSizeOffset = 5;
const float kMinSize = 1;

const sf::Color kBackgroundColor = sf::Color::Black;
const sf::Color kUIColor = sf::Color::White;
const sf::Color kUIBackgroundColor = sf::Color(240, 240, 240);
const sf::Color kUnselectedColor = sf::Color(100, 100, 100);
const sf::Color kErrorColor = sf::Color::Red;

const sf::Color kHintTextColor = sf::Color(150, 150, 150);
const sf::Color kTextColor = sf::Color::Black;
const sf::Color kSelectedColor = sf::Color::White;

const sf::Color kPointColor = sf::Color::Black;

#endif
