#include "array.h"
#include <sstream>
#include <algorithm>

Array::Array() : size(0), capacity(0), array(nullptr) {}

Array::Array(size_t initialSize) : size(0), capacity(0), array(nullptr) {
    resize(initialSize);
}

Array::Array(const std::initializer_list<number>& initList) : size(0), capacity(0), array(nullptr) {
    resize(initList.size());
    std::copy(initList.begin(), initList.end(), array);
    size = initList.size();
}

Array::Array(const Array& other) : size(other.size), capacity(other.capacity), array(nullptr) {
    if (capacity > 0) {
        array = new number[capacity];
        std::copy(other.array, other.array + size, array);
    }
}

Array& Array::operator=(const Array& other) {
    if (this == &other) return *this;

    delete[] array;

    size = other.size;
    capacity = other.capacity;

    if (capacity > 0) {
        array = new number[capacity];
        std::copy(other.array, other.array + size, array);
    }
    else {
        array = nullptr;
    }

    return *this;
}

Array::~Array() {
    delete[] array;
}

size_t Array::getSize() const {
    return size;
}

void Array::resize(size_t newSize) {
    if (newSize > capacity) {
        reallocate(newSize);
    }
    size = newSize;
}

void Array::add(number value) {
    if (size >= capacity) {
        reallocate(capacity == 0 ? 1 : capacity * 2);
    }
    array[size++] = value;
}

void Array::remove()
{
    if (size > 0) {
        size--;
        if (size <= capacity / 4 && capacity > 2) {
            resize(capacity / 2);
        }
    }
}

number Array::average() const
{
    number res = 0;
    for (size_t i = 0; i < size; i++) {
        res += array[i];
    }
    res /= size;

    return res;
}

number Array::MSD() const
{
    number avg = average();
    number res = 0;
    for (size_t i = 0; i < size; i++) {
        res += pow((array[i] - avg), 2);
    }
    res = sqrt((1.0 / (size - 1)) * res);

    return res;
}

void Array::fill(const std::string& input) {
    std::istringstream stream(input);
    number value;

    clear();

    while (stream >> value) {
        add(value);
        if (stream.peek() == ',' || stream.peek() == ' ' || stream.peek() == ';') {
            stream.ignore();
        }
    }

    if (stream.fail() && !stream.eof()) {
        throw std::invalid_argument("Invalid input string for filling Array");
    }
}

void Array::clear() {
    size = 0;
}

number& Array::operator[](size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return array[index];
}

const number& Array::operator[](size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return array[index];
}

void Array::reallocate(size_t newCapacity) {
    number* newArray = new number[newCapacity];
    std::copy(array, array + size, newArray);
    delete[] array;
    array = newArray;
    capacity = newCapacity;
}
