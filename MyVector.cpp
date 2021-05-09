#include "MyVector.h"
//#include <ctime>
#include <stdlib.h>
#include <algorithm>
#include <cstddef>


MyVector::MyVector(size_t size,
    ResizeStrategy strategy,
    float coef = 1.5f) {
    _size = size;
    _capacity = _size * coef;
    _data = new ValueType[_capacity];
    _coef = coef;
    _strategy = strategy;
}

MyVector::MyVector(size_t size,
    ValueType value,
    ResizeStrategy strategy,
    float coef = 1.5f) {
    _size = size;
    _capacity = _size * coef;
    _data = new ValueType[_capacity];
    for (int i = 0; i < _size; ++i) {
        _data[i] = value;
    }
    _strategy = strategy;
    _coef = coef;
}

MyVector::MyVector(const MyVector& copy) {
    _size = copy._size;
    _capacity = copy._capacity;
    if (nullptr == copy._data) {
        _data = nullptr;
    }
    else {
        _data = new ValueType[_capacity];
        for (int i = 0; i < _size; ++i) {
            _data[i] = copy._data[i];
        }
    }
    _coef = copy._coef;
    _strategy = copy._strategy;
}

MyVector& MyVector::operator=(const MyVector& copy) {
    if (&copy == this) {
        return *this;
    }
    _size = copy._size;
    _capacity = copy._capacity;
    if (_data != nullptr) {
        delete[] _data;
    }
    if (nullptr == copy._data) {
        _data = nullptr;
    }
    else {
        _data = new ValueType[_capacity];
        for (int i = 0; i < _size; ++i) {
            _data[i] = copy._data[i];
        }
    }
    _coef = copy._coef;
    _strategy = copy._strategy;
}

MyVector::MyVector(MyVector&& other) noexcept {
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
    _data = other._data;
    other._data = nullptr;
    std::swap(_coef, other._coef);
    std::swap(_strategy, other._strategy);
}

MyVector& MyVector::operator=(MyVector&& other) noexcept {
    if (&other == this)
        return *this;
    if (_data != nullptr) {
        delete[] _data;
    }
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
    _data = other._data;
    other._data = nullptr;
    std::swap(_coef, other._coef);
    std::swap(_strategy, other._strategy);
}

MyVector::~MyVector() {
    if (_data != nullptr) {
        delete[] _data;
    }
}

size_t MyVector::capacity() const {
    return _capacity;
}

size_t MyVector::size() const {
    return _size;
}

float MyVector::loadFactor() const {
    if (!_capacity) {
        return 1;
    }
    else {
        return _size / _capacity;
    }
}

ValueType& MyVector::operator[](const size_t i) {
    return _data[i];
}

const ValueType& MyVector::operator[](const size_t i) const {
    return _data[i];
}

void MyVector::pushBack(const ValueType& value) {
    if (_capacity < _size + 1) {
        resize(_capacity + 1);
    }
    ++_size;
    _data[_size] = value;
}

void MyVector::insert(const size_t i, const ValueType& value) {
    if (_capacity < _size + 1) {
        resize(_capacity + 1);
    }
    ++_size;
    if (_data == nullptr) {
        _data = new ValueType(1);
    }
    ValueType prevValue, curValue;
    prevValue = _data[i];
    _data[i] = value;
    for (int j = i + 1; j < _size; ++j) {
        curValue = _data[j];
        _data[j] = prevValue;
        prevValue = curValue;
    }
}
void MyVector::insert(const size_t i, const MyVector& value) {
    if (_capacity < (_size + value._size)) {
        resize(_size + value._size);
    }
    for (int j = _size - 1; j >= i; --j) {
        _data[j + value._size] = _data[j];
    }
    for (int j = i; j < value._size; ++j) {
        _data[j] = value._data[j - i];
    }
    _size = _size + i;
}
void MyVector::insert(ConstVectorIterator it, const ValueType& value){}
void MyVector::insert(ConstVectorIterator it, const MyVector& value) {}

void MyVector::popBack() {
    --_size;
}

void MyVector::erase(const size_t i) {
    erase(i, 1);
}

void MyVector::erase(const size_t i, const size_t len) {
    _size = _size - len;
    for (int j = i; j < _size; ++j) {
        _data[j] = _data[j + len];
    }
}

MyVector::VectorIterator MyVector::find(const ValueType& value, bool isBegin) const{
    if (isBegin) {
        for (VectorIterator i = begin(); i != end(); ++i) {
            if (*(i) == value) {
                return i;
            }
        }
    }
    else {
        VectorIterator vEnd = nullptr;
        for (VectorIterator i = begin(); i != end(); ++i) {
            if (*(i) == value) {
                vEnd = i;
            }
        }
        if (vEnd != nullptr) {
            return vEnd;
        }
    }
    return end();
}

void MyVector::reserve(const size_t capacity) {
    ValueType* data = new ValueType[capacity];
    for (int i = 0; i < _size; ++i) {
        data[i] = _data[i];
    }
    std::swap(_data, data);
    delete[] data;
    _capacity = capacity;
}

void MyVector::resize(const size_t size, const ValueType& value) {
    if (size > _size) {
        if (size > _capacity) {
            resize(size);
        }
        for (int i = _size; i < size; ++i) {
            _data[i] = value;
        }
    }
    else {
        _size = size;
    }
}

void MyVector::clear() {
    _size = 0;
    delete[] _data;
}

void MyVector::resize(int must) {
    while (_capacity < must) {
        if (_strategy == ResizeStrategy::Multiplicative) {
            _capacity = _capacity * _coef;
        }
        else {
            _capacity = _capacity + _coef;
        }
    }
    ValueType* data = new ValueType[_capacity];
    for (int i = 0; i < _size; ++i) {
        data[i] = _data[i];
    }
    std::swap(_data, data);
    delete[] data;
}

MyVector::VectorIterator MyVector::begin(){
    if (_size) {
        return VectorIterator(&_data[0]);
    }
    else {
        return VectorIterator(nullptr);
    }
}
MyVector::ConstVectorIterator MyVector::cbegin() const{
    if (_size) {
        return ConstVectorIterator(&_data[0]);
    }
    else {
        return ConstVectorIterator(nullptr);
    }
}
MyVector::VectorIterator MyVector::end(){
    if (_size) {
        return VectorIterator(&_data[_size]);
    }
    else {
        return VectorIterator(nullptr);
    }
}
MyVector::ConstVectorIterator MyVector::cend() const{
    if (_size) {
        return ConstVectorIterator(&_data[_size]);
    }
    else {
        return ConstVectorIterator(nullptr);
    }
}

MyVector::VectorIterator::VectorIterator(pointer pData) 
    : _ptr(pData)
    {}

MyVector::ConstVectorIterator::ConstVectorIterator(pointer pData) 
    : _ptr(pData)
    {}


MyVector::VectorIterator::reference MyVector::VectorIterator::operator* () {
    return *_ptr;
}
MyVector::VectorIterator::pointer MyVector::VectorIterator::operator->() {
    return _ptr;
}

const MyVector::ConstVectorIterator::reference MyVector::ConstVectorIterator::operator* () {
    return *_ptr;
}
const MyVector::ConstVectorIterator::pointer MyVector::ConstVectorIterator::operator->() {
    return _ptr;
}

bool MyVector::VectorIterator::operator==(const VectorIterator& other) const {
    return _ptr == other._ptr;
}
bool MyVector::VectorIterator::operator!=(const VectorIterator& other) const {
    return !(_ptr == other._ptr);
}
MyVector::VectorIterator& MyVector::VectorIterator::operator++() {
    _ptr++;
    return *this;
}
MyVector::VectorIterator MyVector::VectorIterator::operator++(int) {
    VectorIterator temp = *this;
    ++* (this);
    return temp;
}
/*MyVector::VectorIterator& MyVector::VectorIterator::operator--() {
    _ptr--;
    return *this;
}
MyVector::VectorIterator MyVector::VectorIterator::operator--(int) {
    VectorIterator temp = *this;
    --* (this);
    return temp;
}*/

bool MyVector::ConstVectorIterator::operator==(const ConstVectorIterator& other) const {
    return _ptr == other._ptr;
}
bool MyVector::ConstVectorIterator::operator!=(const ConstVectorIterator& other) const {
    return !(_ptr == other._ptr);
}
MyVector::ConstVectorIterator& MyVector::ConstVectorIterator::operator++() {
    _ptr++;
    return *this;
}
MyVector::ConstVectorIterator MyVector::ConstVectorIterator::operator++(int) {
    ConstVectorIterator temp = *this;
    ++*(this);
    return temp;
}
/*MyVector::ConstVectorIterator& MyVector::ConstVectorIterator::operator--() {
    _ptr--;
    return *this;
}
MyVector::ConstVectorIterator MyVector::ConstVectorIterator::operator--(int) {
    ConstVectorIterator temp = *this;
    --*(this);
    return temp;
}*/