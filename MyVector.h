#pragma once
#include <cstddef>
#include<iterator>

// стратегия изменения capacity
enum class ResizeStrategy {
    Additive,
    Multiplicative
};

// тип значений в векторе
// потом будет заменен на шаблон
using ValueType = double;

class MyVector
{
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
        VectorIterator(pointer pData);

        //VectorIterator(pointer pData, size_t idx);

        reference operator*();
        pointer operator->();

        bool operator==(const VectorIterator& other) const;
        bool operator!=(const VectorIterator& other) const;
        VectorIterator& operator++();
        VectorIterator operator++(int);
        //VectorIterator& operator--();
        //VectorIterator operator--(int);
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
        ConstVectorIterator(pointer pData);
        //ConstVectorIterator(pointer pData, size_t idx);

        reference operator*();
        const pointer operator->();

        bool operator==(const ConstVectorIterator& other) const;
        bool operator!=(const ConstVectorIterator& other) const;
        ConstVectorIterator& operator++();
        ConstVectorIterator operator++(int);
        //ConstVectorIterator& operator--();
        //ConstVectorIterator operator--(int);
    private:
        pointer _ptr;
    };

    // заполнить вектор значениями ValueType()
    MyVector(size_t size = 0,
        ResizeStrategy strategy = ResizeStrategy::Multiplicative,
        float coef = 1.5f);
    // заполнить вектор значениями value
    MyVector(size_t size,
        ValueType value,
        ResizeStrategy = ResizeStrategy::Multiplicative,
        float coef = 1.5f);

    MyVector(const MyVector& copy);
    MyVector& operator=(const MyVector& copy);

    MyVector(MyVector&& other) noexcept;
    MyVector& operator=(MyVector&& other) noexcept;
    ~MyVector();

    size_t capacity() const;
    size_t size() const;
    float loadFactor() const;

    VectorIterator begin();
    ConstVectorIterator begin() const;
    VectorIterator end();
    ConstVectorIterator end() const;

    // доступ к элементу, 
    // должен работать за O(1)
    ValueType& operator[](const size_t i);
    const ValueType& operator[](const size_t i) const;

    // добавить в конец,
    // должен работать за amort(O(1))
    void pushBack(const ValueType& value);
    // вставить,
    // должен работать за O(n)
    void insert(const size_t i, const ValueType& value);     // версия для одного значения
    void insert(const size_t i, const MyVector& value);      // версия для вектора
    void insert(ConstVectorIterator it, const ValueType& value);  // версия для одного значения
    void insert(ConstVectorIterator it, const MyVector& value);   // версия для вектора

    // удалить с конца,
    // должен работать за amort(O(1))
    void popBack();
    // удалить
    // должен работать за O(n)
    void erase(const size_t i);
    void erase(const size_t i, const size_t len);            // удалить len элементов начиная с i

    // найти элемент,
    // должен работать за O(n)
    // если isBegin == true, найти индекс первого элемента, равного value, иначе последнего
    // если искомого элемента нет, вернуть end
    VectorIterator find(const ValueType& value, bool isBegin = true);

    // зарезервировать память (принудительно задать capacity)
    void reserve(const size_t capacity);

    // изменить размер
    // если новый размер больше текущего, то новые элементы забиваются value
    // если меньше - обрезаем вектор
    void resize(const size_t size, const ValueType& value = ValueType());

    // очистка вектора, без изменения capacity
    void clear();

    void resize(int must);
private:
    ValueType* _data;
    size_t _size;
    size_t _capacity;
    ResizeStrategy _strategy;
    float _coef;
};