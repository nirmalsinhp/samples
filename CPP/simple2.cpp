#include <bits/stdc++.h>

using namespace std;

template <typename T>
class Vec
{
    using value_type = T;
    using size_type = size_t;

    Vec() : _data(nullptr), size(0), capacity(0)
    {
    }

    Vec(int size)
    {
        _data = make_unique<T[]>(size);
        size = size;
        capacity = capacity;
    }

    Vec(int size, T val)
    {
        _data = make_unique<T[]>(size);
        size = size;
        capacity = capacity;
        fill(begin(_data.get()), end(_data.get()), val);
    }

    Vec(const Vec &rhs)
    {
        _data = make_unique<T[]>([rhs.size]);
        size = rhs.size;
        capacity = rhs.capacity;
        copy(begin(rhs._data.get()), end(rhs._data.get()), _data.begin());
    }

    Vec &operator=(Vec rhs)
    {
        //auto tmp = rhs;
        swap(*this, rhs);
        return *this;
    }

    Vec(Vec &&rhs) = default;

    //Vec &operator=(vec &&rhs) = default;

    size_type size() const { return sz;}
    T& operator[] (size_type i) { return _data[i];}
    const T& operator[] (size_type i) const { return _data[i];}

    void push_back(T val)
    {
        if (isFull)
        {
            capacity = 2 * size;
            auto tmp_data = make_unique<T[]>([capacity]);
            copy(begin(_data.get()), end(_data.get()), begin(tmp_data.get()));
            _data.reset(tmp_data);
        }
        (*_data)[size++] = val;
    }

    bool isFull()
    {
        return (size == capacity);
    }

    friend void swap(Veec &lhs, Vec &rhs);

    private : unique_ptr<T[]> _data;
    int size;
    int capacity;
};

template <typename T>
void swap(Vec<T> &lhs, Vec<T> &rhs)
{
    using namespace std;
    swap(lhs._data, rhs._data);
    swap(lhs.size, rhs.size);
    swap(lhs.capacity, rhs.capacity);
}

