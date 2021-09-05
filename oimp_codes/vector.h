#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>

template <typename T>
class RawMemory {
public:
    T* buf = nullptr;
    size_t cp = 0;

    static T* allocate(size_t n) {
        return static_cast<T*>((operator new(sizeof(T) * n)));
    }

    static void deallocate(T* buf) {
        operator delete(buf);
    }

    RawMemory() = default;

    explicit RawMemory(size_t n) {
        buf = allocate(n);
        cp = n;
    }

    RawMemory(const RawMemory&) = delete;

    RawMemory(RawMemory&& other) noexcept {
        swap(other);
    }

    RawMemory& operator=(const RawMemory&) = delete;

    RawMemory& operator=(RawMemory&& other) noexcept {
        swap(other);
        return *this;
    }

    ~RawMemory() {
        deallocate(buf);
    }

    void swap(RawMemory<T>& other) noexcept {
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

template <typename T>
class Vector {
private:
    RawMemory<T> data;
    size_t sz = 0;

public:
    Vector() = default;
    explicit Vector(size_t);
    Vector(const Vector&);

    ~Vector();

    Vector& operator=(const Vector&);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void reserve(size_t);
    void resize(size_t);

    void push_back(const T&);
    void push_back(T&&);

    void pop_back();

    void clear();

    void swap(Vector&) noexcept;

    size_t size() const noexcept;
    size_t capacity() const noexcept;

    T* begin() noexcept;
    const T* begin() const noexcept;

    T* end() noexcept;
    const T* end() const noexcept;
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

template <typename T>
Vector<T>::~Vector<T>() {
    std::destroy_n(data.buf, sz);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    // строгая гарантия безопасности
    Vector tmp(other);
    swap(tmp);
    sz = other.sz;
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
void Vector<T>::reserve(size_t n) {
    // строгая гарантия безопасности
    if (n > data.cp) {
        RawMemory<T> data2(n);
        std::uninitialized_copy_n(data.buf, sz, data2.buf);
        std::destroy_n(data.buf, sz);
        data.swap(data2);
    }
}

template<typename T>
void Vector<T>::resize(size_t n) {
    // базовая гарантия безопасности
    reserve(n);

    if (n < sz) {
        std::destroy_n(data + n, sz - n);
    } else if (n > sz) {
        std::uninitialized_default_construct_n(data + sz, n - sz);
    }
    sz = n;
}

template<typename T>
void Vector<T>::push_back(const T& elem) {
    // базовая гарантия безопасности
    if (sz == data.cp) {
        reserve(sz == 0 ? 1 : 2 * sz);
    }
    new (data + sz) T(elem);
    ++sz;
}

template<typename T>
void Vector<T>::push_back(T&& elem) {
    // базовая гарантия безопасности
    if (sz == data.cp) {
        reserve(sz == 0 ? 1 : 2 * sz);
    }
    new (data + sz) T(std::move(elem));
    ++sz;
}

template<typename T>
void Vector<T>::pop_back() {
    std::destroy_at(data + sz - 1);
    --sz;
}

template <typename T>
void Vector<T>::clear() {
    std::destroy_n(data.buf, sz);
    sz = 0;
}

template<typename T>
void Vector<T>::swap(Vector& other) noexcept {
    data.swap(other.data);
    std::swap(sz, other.sz);
}

template<typename T>
size_t Vector<T>::size() const noexcept {
    return sz;
}

template<typename T>
size_t Vector<T>::capacity() const noexcept {
    return data.cp;
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

