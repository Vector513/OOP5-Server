#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <QString>

class Complex
{
private:
    double re = 0;
    double im = 0;

public:
    Complex() = default;
    Complex(double re);
    Complex(double re, double im);

    ~Complex() = default;

    double getRe() const;
    double getIm() const;

    double mod() const;
    double arg() const;
    Complex pow(double exponent) const;

    friend Complex pow(const Complex& base, double exponent);
    friend Complex sqrt(const Complex& base);

    friend std::ostream& operator<<(std::ostream& output, const Complex& complex);
    friend std::istream& operator>>(std::istream& input, Complex& complex);

    friend QString& operator<<(QString& output, const Complex& complex);

    Complex& operator=(const Complex& other);

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const double coef) const;
    friend Complex operator*(const double coef, const Complex& other);
    Complex operator*(const Complex& other) const;
    Complex operator/(const double coef) const;
    Complex operator/(const Complex& other) const;
    Complex operator-() const;

    Complex& operator+=(const Complex& other);
    Complex& operator-=(const Complex& other);
    Complex& operator*=(const double coef);
    Complex& operator*=(const Complex& other);
    Complex& operator/=(const double coef);
    Complex& operator/=(const Complex& other);

    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;

    bool operator>(const Complex& other) const;
    bool operator>=(const Complex& other) const;
    bool operator<(const Complex& other) const;
    bool operator<=(const Complex& other) const;
};

#endif // COMPLEX_H
