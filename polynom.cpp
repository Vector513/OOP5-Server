#include "polynom.h"
#include <sstream>
#include <iomanip>

Polynom::Polynom() : degree(0), An(), roots(), coefs() {}

Polynom::~Polynom() {}

void Polynom::clear() {}

void Polynom::fillRoots(const std::string& input) {
    roots.fill(input);
    degree = roots.getSize();
    calculateCoefs();
}

Array Polynom::multiplyPolynomials(const Array& roots1, const Array& roots2) {
    Array result;
    size_t newSize = roots1.getSize() + roots2.getSize() - 1;
    result.resize(newSize);

    for (size_t i = 0; i < roots1.getSize(); ++i) {
        for (size_t j = 0; j < roots2.getSize(); ++j) {
            result[i + j] += roots1[i] * roots2[j];
        }
    }

    return result;
}

void Polynom::calculateCoefs() {
    Array currentPoly;
    currentPoly.add(An);

    for (size_t i = 0; i < roots.getSize(); ++i) {
        Array factor;
        factor.add(-roots[i]);
        factor.add(1.0);
        currentPoly = multiplyPolynomials(currentPoly, factor);
    }// result[0] = (3-4i)*(-2-2i) = -6 - 6i + 8i -8 = -14 + 2i
     // result[1] = (3-4i)*1 = 3-4i

    coefs = currentPoly;
}

void Polynom::setRoot(const int index, const number& newRoot) {
    if (index == roots.getSize()) {
        roots.add(newRoot);
        degree++;
    }
    else if (index >= 0 && index < roots.getSize()) {
        roots[index] = newRoot;
    }
    calculateCoefs();
}

void Polynom::addRoot(const number& newRoot) {
    roots.add(newRoot);
    degree++;
    calculateCoefs();
}

void Polynom::setAn(const number& newAn) {
    An = newAn;
    calculateCoefs();
}

size_t Polynom::getDegree() const {
    return degree;
}

const number& Polynom::getAn() const {
    return An;
}

const Array& Polynom::getRoots() const {
    return roots;
}

const Array& Polynom::getCoefs() const {
    return coefs;
}

number Polynom::evaluate(const number& x) const {
    number result = An;

    for (size_t i = 0; i < roots.getSize(); ++i) {
        result *= (x - roots[i]);
    }

    return result;
}

void Polynom::resize(const size_t newSize) {
    roots.resize(newSize);
    coefs.resize(newSize);
    degree = roots.getSize();
    calculateCoefs();
}

void Polynom::show(std::ostream& output, bool isFirstForm) const {
    if (isFirstForm) {
        output << "p(x) = ";
        bool firstTerm = true;
        bool needMinus = false;
        for (size_t i = coefs.getSize(); i-- > 0;) {
            if (coefs[i].getRe() != 0 || coefs[i].getIm() != 0) {
                double re = coefs[i].getRe();
                double im = coefs[i].getIm();

                if (!firstTerm) {
                    needMinus = (re < 0 || (re == 0 && im < 0));
                    output << (needMinus ? " - " : " + ");
                }
                else {
                    firstTerm = false;
                }

                if (im == 0) {
                    output << std::abs(re);
                }
                else {
                    output << '(';
                    if (re != 0) {
                        output << std::abs(re);
                    }
                    output << ((im > 0) ^ (needMinus) ? " + " : " - ")
                           << std::abs(im) << "i)";
                }

                if (i > 0) {
                    output << "x";
                    if (i > 1) {
                        output << "^" << i;
                    }
                }
            }
        }

        if (firstTerm) {
            output << "0";
        }
        output << '\n';
    }
    else {
        output << "p(x) = ";

        if (An.getRe() != 0 || An.getIm() != 0) {
            if (An.getIm() == 0) {
                output << std::abs(An.getRe());
            }
            else {
                output << '(';
                if (An.getRe() != 0) {
                    output << std::abs(An.getRe());
                }
                output << (An.getIm() > 0 ? " + " : " - ")
                       << std::abs(An.getIm()) << "i)";
            }

            if (roots.getSize() != 0) {

                for (size_t i = 0; i < roots.getSize(); ++i) {
                    output << "(x";
                    double re = roots[i].getRe();
                    double im = roots[i].getIm();

                    if (re != 0) {
                        output << (re > 0 ? " - " : " + ") << std::abs(re);
                    }

                    if (im != 0) {
                        output << (im > 0 ? " - " : " + ") << std::abs(im) << "i";
                    }

                    output << ")";
                }
            }
        }
        else {
            output << "0";
        }
        output << '\n';
    }
}
