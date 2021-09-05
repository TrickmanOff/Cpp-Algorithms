#include <algorithm>

template <typename T>
class SharedPtr {
private:
    T* ptr = nullptr;
    int* counter = nullptr;

public:
    SharedPtr() noexcept = default;
    explicit SharedPtr(T*);
    SharedPtr(const SharedPtr&) noexcept;
    SharedPtr(SharedPtr&&) noexcept;

    ~SharedPtr();

    SharedPtr& operator=(T*);
    SharedPtr& operator=(const SharedPtr&) noexcept;
    SharedPtr& operator=(SharedPtr&&) noexcept;

    T& operator*();
    const T& operator*() const;

    T* operator->() const noexcept;

    void reset(T*);
    void decr_counter() noexcept;

    void swap(SharedPtr&) noexcept;

    T* get() const noexcept;

    explicit operator bool() const noexcept;
};

template<typename T>
SharedPtr<T>::SharedPtr(T* t_ptr): ptr(t_ptr) {
    if (ptr) {
        counter = new int(1);
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept : ptr(other.ptr), counter(other.counter) {
    if (ptr) {
        ++(*counter);
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept {
    swap(other);
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    decr_counter();
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(T* t_ptr) {
    reset(t_ptr);
    return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) noexcept {
    if (counter == other.counter) {
        return *this;
    }

    decr_counter();
    ptr = other.ptr;
    counter = other.counter;
    if (ptr) {
        ++(*counter);
    }
    return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept {
    if (counter == other.counter) {
        return *this;
    }

    SharedPtr(std::move(other)).swap(*this);
    return *this;
}

template<typename T>
T& SharedPtr<T>::operator*() {
    return *ptr;
}

template<typename T>
const T& SharedPtr<T>::operator*() const {
    return *ptr;
}

template<typename T>
T* SharedPtr<T>::operator->() const noexcept {
    return ptr;
}

template<typename T>
void SharedPtr<T>::reset(T* t_ptr) {
    SharedPtr(t_ptr).swap(*this);
}

template<typename T>
void SharedPtr<T>::decr_counter() noexcept {
    if (!ptr) {
        return;
    }

    if (--(*counter) == 0) {
        delete ptr;
        delete counter;
    }
}

template<typename T>
void SharedPtr<T>::swap(SharedPtr& other) noexcept {
    std::swap(ptr, other.ptr);
    std::swap(counter, other.counter);
}

template<typename T>
T* SharedPtr<T>::get() const noexcept {
    return ptr;
}

template<typename T>
SharedPtr<T>::operator bool() const noexcept {
    return ptr;
}

