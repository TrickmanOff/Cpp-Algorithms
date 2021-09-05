#include <algorithm>
#include <cstddef>
#include <tuple>

template<typename T>
class DefaultDeleter {
public:
    void operator()(T* ptr) {
        delete ptr;
    }
};

template <typename T, typename Deleter = DefaultDeleter<T>>
class UniquePtr {
private:
    // to use Base Class Optimization
    std::tuple<T*, Deleter> data;

    T*& ptr() {
        return std::get<0>(data);
    }

    T* ptr() const {
        return std::get<0>(data);
    }

public:
    Deleter& get_deleter() noexcept {
        return std::get<1>(data);
    }

    const Deleter& get_deleter() const noexcept {
        return std::get<1>(data);
    };

    UniquePtr() noexcept: data(nullptr, Deleter()) {}

    explicit UniquePtr(T* _ptr) noexcept: data(_ptr, Deleter()) {}

    UniquePtr(const UniquePtr&) = delete;

    explicit UniquePtr(UniquePtr&& u_ptr) noexcept {
        ptr() = nullptr;
        std::swap(ptr(), u_ptr.ptr());
        get_deleter() = std::move(u_ptr.get_deleter());
    }

    UniquePtr(T* _ptr, const Deleter& _deleter): data(_ptr, _deleter) {}

    UniquePtr& operator=(std::nullptr_t) noexcept {
        reset(nullptr);

        return *this;
    }

    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        reset(nullptr);

        std::swap(ptr(), other.ptr());
        get_deleter() = std::move(other.get_deleter());
        return *this;
    }

    ~UniquePtr() {
        if (ptr()) {
            get_deleter()(ptr());
        }
    }

    T& operator*() const {
        return *ptr();
    }

    T* operator->() const noexcept {
        return ptr();
    }

    T* release() noexcept {
        T* prev_ptr = ptr();
        ptr() = nullptr;
        return prev_ptr;
    }

    void reset(T* new_ptr) noexcept {
        if (ptr()) {
            get_deleter()(ptr());
        }
        ptr() = new_ptr;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(data, other.data);
    }

    T* get() const noexcept {
        return ptr();
    }

    explicit operator bool() const noexcept {
        return ptr();
    }
};

