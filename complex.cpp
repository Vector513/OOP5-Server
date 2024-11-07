#include "complex.h"
#include <cmath>
#include <sstream>
#include <regex>

Complex::Complex(double re) : re(re) {}

Complex::Complex(double re, double im) : re(re), im(im) {}

double Complex::getRe() const {
    return re;
}

double Complex::getIm() const {
    return im;
}

double Complex::mod() const {
    return std::hypot(re, im);
}

double Complex::arg() const {
    return std::atan2(im, re);
}

Complex Complex::pow(double exponent) const {
    double r = std::pow(mod(), exponent);
    double theta = arg() * exponent;
    return Complex{ r * std::cos(theta), r * std::sin(theta) };
}

Complex pow(const Complex& base, double exponent) {
    return base.pow(exponent);
}

Complex sqrt(const Complex& base) {
    return base.pow(0.5);
}

std::ostream& operator<<(std::ostream& output, const Complex& complex) {
    output << complex.re << " + " << complex.im << "i";
    return output;
}

QString& operator<<(QString& output, const Complex& complex) {
    //output << complex.re << " + " << complex.im << "i";
    output += QString().setNum(complex.re);
    output += "+";
    output += QString().setNum(complex.im);
    output += "i";

    return output;
}

std::istream& operator>>(std::istream& input, Complex& complex) {
    std::string str;
    input >> str;

    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());

    Complex temp;

    std::regex complexRegex(R"(^([+-]?(\d*\.?\d+)?)([+-]?(\d*\.?\d+)i)?$)");
    std::smatch match;

    if (std::regex_match(str, match, complexRegex)) {
        if (match[1].matched) {
            try {
                temp.re = std::stod(match[1].str());
            }
            catch (const std::invalid_argument&) {
                temp.re = 0;
            }
        }
        if (match[3].matched) {
            try {
                temp.im = std::stod(match[3].str().substr(0, match[3].str().size() - 1));
            }
            catch (const std::invalid_argument&) {
                temp.im = 0;
            }
        }
        else if (str == "i") {
            temp.im = 1;
        }
        else if (str == "-i") {
            temp.im = -1;
        }

        complex = temp;
    }
    else {
        input.setstate(std::ios::failbit);
    }

    return input;
}

Complex& Complex::operator=(const Complex& other) {
    if (this != &other) {
        re = other.re;
        im = other.im;
    }
    return *this;
}

Complex Complex::operator+(const Complex& other) const {
    return Complex{ re + other.re, im + other.im };
}

Complex Complex::operator-(const Complex& other) const {
    return Complex{ re - other.re, im - other.im };
}

Complex Complex::operator*(const double coef) const {
    return Complex{ re * coef, im * coef };
}

Complex operator*(double coef, const Complex& other)
{
    return Complex{ other.getRe() * coef, other.getIm() * coef };
}

Complex Complex::operator*(const Complex& other) const {
    return Complex{ re * other.re - im * other.im, re * other.im + im * other.re };
}

Complex Complex::operator/(const double coef) const {
    return Complex{ re / coef, im / coef };
}

Complex Complex::operator/(const Complex& other) const {
    double denominator = other.re * other.re + other.im * other.im;
    return Complex{ (re * other.re + im * other.im) / denominator,
                   (im * other.re - re * other.im) / denominator };
}

Complex Complex::operator-() const {
    return Complex{ -re, -im };
}

Complex& Complex::operator+=(const Complex& other) {
    re += other.re;
    im += other.im;
    return *this;
}

Complex& Complex::operator-=(const Complex& other) {
    re -= other.re;
    im -= other.im;
    return *this;
}

Complex& Complex::operator*=(const double coef) {
    re *= coef;
    im *= coef;
    return *this;
}

Complex& Complex::operator*=(const Complex& other) {
    double newRe = re * other.re - im * other.im;
    im = re * other.im + im * other.re;
    re = newRe;
    return *this;
}

Complex& Complex::operator/=(const double coef) {
    re /= coef;
    im /= coef;
    return *this;
}

Complex& Complex::operator/=(const Complex& other) {
    double denominator = other.re * other.re + other.im * other.im;
    double newRe = (re * other.re + im * other.im) / denominator;
    im = (im * other.re - re * other.im) / denominator;
    re = newRe;
    return *this;
}

bool Complex::operator==(const Complex& other) const {
    return re == other.re && im == other.im;
}

bool Complex::operator!=(const Complex& other) const {
    return !(*this == other);
}

bool Complex::operator>(const Complex& other) const {
    return mod() > other.mod();
}

bool Complex::operator>=(const Complex& other) const {
    return mod() > other.mod() || (mod() == other.mod() && arg() >= other.arg());
}

bool Complex::operator<(const Complex& other) const {
    return mod() < other.mod();
}

bool Complex::operator<=(const Complex& other) const {
    return mod() < other.mod() || (mod() == other.mod() && arg() <= other.arg());
}
