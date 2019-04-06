#pragma once

class Integer;
class Decimal;
class Complex;
class PowerObject;

//Tools.cpp
const Integer gcd(const Integer &lhs, const Integer &rhs);
const Integer lcm(const Integer &lhs, const Integer &rhs);

//Power.cpp
const Integer sqrt(const Integer &base);
const Decimal sqrt(const Decimal &base);
const Complex sqrt(const Complex &base);

const PowerObject sqrt(const PowerObject &base);
//const Integer sqrt2(const Integer &base);
const Integer pow(const Integer &base, const Integer &e);
const Decimal pow(const Decimal &base, const Decimal &e);
const Complex pow(const Complex &base, const Complex &e);

//Factorial.cpp
const Integer fact(const Integer &level);
const Decimal fact(const Decimal &level);
const Complex fact(const Complex &level);

//Prime.cpp
const bool isPrime(const Integer &z);
