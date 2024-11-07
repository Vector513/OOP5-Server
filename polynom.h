#ifndef POLYNOM_H
#define POLYNOM_H

#include "array.h"

class Polynom
{
private:
    size_t degree;
    number An;
    Array roots;
    Array coefs;

public:
    Polynom();
    ~Polynom();

    void clear();

    void fillRoots(const std::string& input);
    Array multiplyPolynomials(const Array& poly1, const Array& poly2);
    void calculateCoefs();
    void setRoot(const int index, const number& newRoot);
    void addRoot(const number& newRoot);
    void setAn(const number& newAn);
    size_t getDegree() const;
    const number& getAn() const;
    const Array& getRoots() const;
    const Array& getCoefs() const;
    number evaluate(const number& x) const;

    void resize(const size_t newSize);

    void show(std::ostream& output, bool isFirstForm = true) const;
};

#endif // POLYNOM_H
