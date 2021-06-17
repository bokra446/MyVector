#pragma once
#include <cstddef>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include<iterator>

// стратегия изменения capacity
enum class ResizeStrategy {
    Additive,
    Multiplicative
};

// тип значений в векторе
// потом будет заменен на шаблон


template <typename T> class MyVector
{
    using ValueType = T;
public:
    // реализовать итераторы
    class VectorIterator {
    public:
        using value_type = ValueType;
        using pointer = ValueType*;
        using reference = ValueType&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        VectorIterator() = default;
        VectorIterator(pointer pData)
            : _ptr(pData)
        {}

        reference operator*() {
            return *_ptr;
        }
        pointer operator->() {
            return _ptr;
        }

        bool operator==(const VectorIterator& other) const {
            return _ptr == other._ptr;
        }
        bool operator!=(const VectorIterator& other) const {
            return !(_ptr == other._ptr);
        }

        VectorIterator& operator++() {
            _ptr++;
            return *this;
        }
        VectorIterator operator++(int) {
            VectorIterator temp(*this);
            ++* (this);
            return temp;
        }
 
    private:
        pointer _ptr;
    };
    class ConstVectorIterator {
    public:
        using value_type = ValueType;
        using pointer = const ValueType*;
        using reference = const ValueType&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        ConstVectorIterator() = default;
        ConstVectorIterator(pointer pData)
            : _ptr(pData)
        {}

        reference operator*() {
            return *_ptr;
        }
        const pointer operator->() {
            return _ptr;
        }

        bool operator==(const ConstVectorIterator& other) const {
            return _ptr == other._ptr;
        }
        bool operator!=(const ConstVectorIterator& other) const {
            return !(_ptr == other._ptr);
        }
        ConstVectorIterator& operator++() {
            ++_ptr;
            return *this;
        }
        ConstVectorIterator operator++(int) {
            VectorIterator temp(*this);
            ++* (this);
            return temp;
        }

    private:
        pointer _ptr;
    };

    // заполнить вектор значениями ValueType()
    MyVector(size_t size = 0,
        ResizeStrategy strategy = ResizeStrategy::Multiplicative,
        float coef = 1.5f) {
        _size = size;
        _capacity = static_cast<int>(_size * coef);
        _data = new ValueType[_capacity];
        _coef = coef;
        _strategy = strategy;
    }
    // заполнить вектор значениями value
    MyVector(size_t size,
        ValueType value,
        ResizeStrategy strategy = ResizeStrategy::Multiplicative,
        float coef = 1.5f) {
        _size = size;
        _capacity = static_cast<int>(_size * coef);
        _data = new ValueType[_capacity];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = value;
        }
        _strategy = strategy;
        _coef = coef;
    }

    MyVector(const MyVector& copy) {
        _size = copy._size;
        _capacity = copy._capacity;
        if (nullptr == copy._data) {
            _data = nullptr;
        }
        else {
            _data = new ValueType[_capacity];
            for (size_t i = 0; i < _size; ++i) {
                _data[i] = copy._data[i];
            }
        }
        _coef = copy._coef;
        _strategy = copy._strategy;
    }
    MyVector& operator=(const MyVector& copy) {
        if (&copy == this) {
            return *this;
        }
        if (_data != nullptr) {
            delete[] _data;
        }
        _size = copy._size;
        _capacity = copy._capacity;
        if (nullptr == copy._data) {
            _data = nullptr;
        }
        else {
            _data = new ValueType[_capacity];
            for (size_t i = 0; i < _size; ++i) {
                _data[i] = copy._data[i];
            }
        }
        _coef = copy._coef;
        _strategy = copy._strategy;
        return *this;
    }

    MyVector(MyVector&& other) noexcept {
        _size = 0;
        std::swap(_size, other._size);
        _capacity = 0;
        std::swap(_capacity, other._capacity);
        _data = other._data;
        if (other._data != nullptr) {
            delete[] other._data;
            other._data = nullptr;
        }
        std::swap(_coef, other._coef);
        std::swap(_strategy, other._strategy);
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (&other == this)
            return *this;
        if (_data != nullptr) {
            delete[] _data;
            _data = nullptr;
        }
        _size = 0;
        std::swap(_size, other._size);
        _capacity = 0;
        std::swap(_capacity, other._capacity);
        std::swap(_data, other._data);
        std::swap(_coef, other._coef);
        std::swap(_strategy, other._strategy);
        return *this;
    }

    ~MyVector() {
        if (_data != nullptr) {
            delete[] _data;
        }
        _size = 0;
        _capacity = 0;
    }

    size_t capacity() const {
        return _capacity;
    }
    size_t size() const {
        return _size;
    }

    float loadFactor() const {
        if (!_capacity) {
            return 1;
        }
        else {
            return static_cast<float>(_size) / _capacity;
        }
    }

    VectorIterator begin() {
        return MyVector::VectorIterator(&_data[0]);
    }
    ConstVectorIterator begin() const {
        return MyVector::ConstVectorIterator(&_data[0]);
    }
    VectorIterator end() {
        return MyVector::VectorIterator(&_data[size()]);
    }
    ConstVectorIterator end() const {
        return MyVector::ConstVectorIterator(&_data[size()]);
    }

    // доступ к элементу, 
    // должен работать за O(1)
    ValueType& operator[](const size_t i) {
        return _data[i];
    }
    const ValueType& operator[](const size_t i) const {
        return _data[i];
    }

    // добавить в конец,
    // должен работать за amort(O(1))
    void pushBack(const ValueType& value) {
        if (_capacity < _size + 1) {
            resizeCapacity(_capacity + 1);
        }
        _data[_size] = value;
        ++_size;
    }
    // вставить,
    // должен работать за O(n)
    // версия для одного значения
    void insert(const size_t i, const ValueType& value) { 
        if (_capacity < _size + 1) {
            resizeCapacity(_capacity + 1);
        }
        ++_size;
        if (_data == nullptr) {
            _data = new ValueType(1);
        }
        ValueType prevValue, curValue;
        prevValue = _data[i];
        _data[i] = value;
        for (size_t j = i + 1; j < _size; ++j) {
            curValue = _data[j];
            _data[j] = prevValue;
            prevValue = curValue;
        }
    } 
    // версия для вектора
    void insert(const size_t i, const MyVector& value) {
        if (_capacity < (_size + value._size)) {
            resizeCapacity(_size + value._size);
        }
        for (size_t j = size() - 1; j >= i; --j) {
            _data[j + value._size] = _data[j];
        }
        for (size_t j = i; j < value._size + i; ++j) {
            _data[j] = value._data[j - i];
        }
        _size = _size + value._size;
    } 
    // версия для одного значения
    void insert(ConstVectorIterator it, const ValueType& value) {
        if (_capacity < _size + 1) {
            resizeCapacity(_capacity + 1);
        }
        ++_size;
        if (_data == nullptr) {
            _data = new ValueType(1);
        }
        MyVector::VectorIterator i;
        for (i = begin(); i.operator->() != it.operator->(); ++i) {}
        ValueType prevValue, curValue;
        prevValue = *it;
        *(i) = value;
        ++i;
        for (; i != end(); ++i) {
            curValue = *(i);
            *(i) = prevValue;
            prevValue = curValue;
        }
    } 
    // версия для вектора
    void insert(ConstVectorIterator it, const MyVector& value) {
        if (_capacity < (_size + value._size)) {
            resizeCapacity(_size + value._size);
        }
        MyVector::VectorIterator i, j;
        for (i = begin(), j = begin(); i.operator->() != it.operator->(); ++i, ++j) {}
        MyVector::VectorIterator k = j;
        ValueType* buffer = new ValueType[value._size];
        size_t l = 0;
        for (; i != (_data + value._size); ++i, ++l) {
            buffer[l] = *i;
        }
        ValueType temp;
        for (; i != (_data + value._size + _size); ++i, ++j, ++l) {
            temp = *i;
            *i = buffer[l % value._size];
            buffer[l % value._size] = temp;
        }
        for (int l = 0; l < value._size; ++l, ++k) {
            *k = value._data[l];
        }
        _size = _size + value._size;
    } 

    // удалить с конца,
    // должен работать за amort(O(1))
    void popBack() {
        --_size;
    }
    // удалить
    // должен работать за O(n)
    void erase(const size_t i) {
        erase(i, 1);
    }
    // удалить len элементов начиная с i
    void erase(const size_t i, const size_t len) {
        _size = _size - len;
        for (size_t j = i; j < _size; ++j) {
            _data[j] = _data[j + len];
        }
    } 

    // найти элемент,
    // должен работать за O(n)
    // если isBegin == true, найти индекс первого элемента, равного value, иначе последнего
    // если искомого элемента нет, вернуть end
    VectorIterator find(const ValueType& value, bool isBegin = true) {
        if (isBegin) {
            MyVector::VectorIterator i = nullptr;
            for (i = begin(); i != end(); ++i) {
                if (*(i) == value) {
                    return i;
                }
            }
        }
        else {
            MyVector::VectorIterator vEnd = nullptr;
            for (auto i = begin(); i != end(); ++i) {
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

    // зарезервировать память (принудительно задать capacity)
    void reserve(const size_t capacity) {
        ValueType* data = new ValueType[capacity];
        for (size_t i = 0; i < _size; ++i) {
            data[i] = _data[i];
        }
        std::swap(_data, data);
        if (_data != nullptr) {
            delete[] _data;
            _data = nullptr;
        }
        _capacity = capacity;
    }

    // изменить размер
    // если новый размер больше текущего, то новые элементы забиваются value
    // если меньше - обрезаем вектор
    void resize(const size_t size, const ValueType& value = ValueType()) {
        if (size > _size) {
            if (size > _capacity) {
                resizeCapacity(size);
            }
            for (size_t i = _size; i < size; ++i) {
                _data[i] = value;
            }
        }
        _size = size;
    }

    // очистка вектора, без изменения capacity
    void clear() {
        _size = 0;
    }


    friend std::ostream& operator<<(std::ostream& out, const MyVector& o) {
        for (MyVector::ConstVectorIterator i = o.begin(); i != o.end(); ++i) {
            out << *i;
        }
        out << ": data, " << o._size << ": size" << std::endl;
        return out;
    }
private:
    ValueType* _data;
    size_t _size;
    size_t _capacity;
    ResizeStrategy _strategy;
    float _coef;

    void resizeCapacity(int must) {
        if (!_capacity) {
            ++_capacity;
        }
        while (_capacity <= must) {
            if (_strategy == ResizeStrategy::Multiplicative) {
                _capacity = static_cast<size_t>(_capacity * _coef + _capacity);
            }
            else {
                _capacity = _capacity + static_cast<int>(_coef);
            }
        }
        ValueType* data = new ValueType[_capacity];
        for (size_t i = 0; i < _size; ++i) {
            data[i] = _data[i];
        }
        if (_data != nullptr) {
            delete[] _data;
            _data = nullptr;
        }
        std::swap(_data, data);
    }
};