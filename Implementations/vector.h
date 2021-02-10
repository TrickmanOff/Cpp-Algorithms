//
// made on 17.10.2020
//
#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>

template <typename T>
class RawMemory;

template <typename T>
class Vector {
private:
    RawMemory<T> data;
    size_t sz = 0;

public:
    Vector() = default;
    explicit Vector(size_t);
    Vector(const Vector&);
    Vector(Vector&&) noexcept;

    ~Vector();

    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&) noexcept;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void Reserve(size_t);
    void Resize(size_t);

    void PushBack(const T&);
    void PushBack(T&&);

    void PopBack();

    void Clear();

    void Swap(Vector&) noexcept;

    size_t Size() const noexcept;
    size_t Capacity() const noexcept;

    template <typename ... Args>
    T& EmplaceBack(Args&& ...);

    T* begin() noexcept;
    const T* begin() const noexcept;

    T* end() noexcept;
    const T* end() const noexcept;
};

template <typename T>
class RawMemory {
public:
    T* buf = nullptr;
    size_t cp = 0;

    static T* Allocate(size_t n) {
        return static_cast<T*>((operator new(sizeof(T) * n)));
    }

    static void Deallocate(T* buf) {
        operator delete(buf);
    }

    RawMemory() = default;

    explicit RawMemory(size_t n) {
        buf = Allocate(n);
        cp = n;
    }

    RawMemory(const RawMemory&) = delete;

    RawMemory(RawMemory&& other) noexcept {
        Swap(other);
    }

    RawMemory& operator=(const RawMemory&) = delete;

    RawMemory& operator=(RawMemory&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~RawMemory() {
        Deallocate(buf);
    }

    void Swap(RawMemory<T>& other) noexcept {
        std::swap(buf, other.buf);
        std::swap(cp, other.cp);
    }

    T& operator[](const size_t index) {
        return buf[index];
    }

    const T& operator[](const size_t index) const {
        return buf[index];
    }

    T* operator+(size_t add) {
        return buf + add;
    }

    const T* operator+(size_t add) const {
        return buf + add;
    }
};

template<typename T>
Vector<T>::Vector(size_t n): data(n) {
    std::uninitialized_default_construct_n(data.buf, n);
    sz = n;
}

template<typename T>
Vector<T>::Vector(const Vector& other): data(other.sz) {
    std::uninitialized_copy_n(other.data.buf, other.sz, data.buf);
    sz = other.sz;
}

template<typename T>
Vector<T>::Vector(Vector&& other) noexcept {
    Swap(other);
}

template <typename T>
Vector<T>::~Vector<T>() {
    std::destroy_n(data.buf, sz);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (other.sz > data.cp) {
        Vector tmp(other);
        Swap(tmp);
    } else {
        for (size_t i = 0; i < std::min(sz, other.sz); ++i) {
            data[i] = other[i];
        }
        if (sz < other.sz) {
            std::uninitialized_copy_n(other.data + sz, other.sz - sz, data + sz);
        } else if (sz > other.sz) {
            std::destroy_n(data + other.sz, sz - other.sz);
        }
    }
    sz = other.sz;
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    Swap(other);
    return *this;
}

template<typename T>
T& Vector<T>::operator[](size_t index) {
    return data[index];
}

template<typename T>
const T& Vector<T>::operator[](size_t index) const {
    return data[index];
}

template <typename T>
void Vector<T>::Reserve(size_t n) {
    if (n > data.cp) {
        RawMemory<T> data2(n);
        std::uninitialized_move_n(data.buf, sz, data2.buf);
        std::destroy_n(data.buf, sz);
        data.Swap(data2);
    }
}

template<typename T>
void Vector<T>::Resize(size_t n) {
    Reserve(n);

    if (n < sz) {
        std::destroy_n(data + n, sz - n);
    } else if (n > sz) {
        std::uninitialized_default_construct_n(data + sz, n - sz);
    }
    sz = n;
}

template<typename T>
void Vector<T>::PushBack(const T& elem) {
    if (sz == data.cp) {
        Reserve(sz == 0 ? 1 : 2 * sz);
    }
    new (data + sz) T(elem);
    ++sz;
}

template<typename T>
void Vector<T>::PushBack(T&& elem) {
    if (sz == data.cp) {
        Reserve(sz == 0 ? 1 : 2 * sz);
    }
    new (data + sz) T(std::move(elem));
    ++sz;
}

template<typename T>
void Vector<T>::PopBack() {
    std::destroy_at(data + sz - 1);
    --sz;
}

template <typename T>
void Vector<T>::Clear() {
    std::destroy_n(data.buf, sz);
    sz = 0;
}

template<typename T>
void Vector<T>::Swap(Vector& other) noexcept {
    data.Swap(other.data);
    std::swap(sz, other.sz);
}

template<typename T>
size_t Vector<T>::Size() const noexcept {
    return sz;
}

template<typename T>
size_t Vector<T>::Capacity() const noexcept {
    return data.cp;
}

template<typename T>
template<typename... Args>
T& Vector<T>::EmplaceBack(Args&& ... args) {
    if (sz == data.cp) {
        Reserve(sz == 0 ? 1 : 2 * sz);
    }
    new (data + sz) T(std::forward<Args>(args)...);
    ++sz;
    return data[sz-1];
}

template<typename T>
T* Vector<T>::begin() noexcept {
    return data.buf;
}

template<typename T>
const T* Vector<T>::begin() const noexcept {
    return data.buf;
}

template<typename T>
T* Vector<T>::end() noexcept {
    return data + sz;
}

template<typename T>
const T* Vector<T>::end() const noexcept {
    return data + sz;
}
