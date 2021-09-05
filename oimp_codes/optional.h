#include <utility>

// Это исключение нужно генерировать в функции value
// при обращении к пустому объекту Optional
struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;
    Optional(const T& elem);
    Optional(T && elem);  // linter interprets "T&& elem" as an error
    Optional(const Optional& other);

    Optional& operator=(const Optional& other);
    Optional& operator=(const T& elem);
    Optional& operator=(T&& elem);

    bool has_value() const;

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;

    T& value();
    const T& value() const;

    void reset();

    ~Optional();
};

template<typename T>
Optional<T>::Optional(const T& elem) {
    new (data) T(elem);
    defined = true;
}

template<typename T>
Optional<T>::Optional(T&& elem) {
    new (data) T(std::move(elem));
    defined = true;
}

template<typename T>
Optional<T>::Optional(const Optional& other): defined(other.defined) {
    if (defined) {
        new (data) T(other.value());
    }
}

template<typename T>
Optional<T>& Optional<T>::operator=(const Optional& other) {
    if (!other.defined) {
        reset();
    } else {
        if (defined) {
            this->value() = other.value();
        } else {
            new (data) T(other.value());
        }
    }
    defined = other.defined;
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(const T& elem) {
    if (defined) {
        this->value() = elem;
    } else {
        new (data) T(elem);
        defined = true;
    }
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(T&& elem) {
    // linter interprets "T&& elem" as an error
    if (defined) {
        this->value() = std::move(elem);
    } else {
        new (data) T(std::move(elem));
        defined = true;
    }
    return *this;
}

template<typename T>
bool Optional<T>::has_value() const {
    return defined;
}

template<typename T>
T& Optional<T>::operator*() {
    return this->value();
}

template<typename T>
const T& Optional<T>::operator*() const {
    return this->value();
}

template<typename T>
T* Optional<T>::operator->() {
    if (!defined) {
        throw BadOptionalAccess();
    } else {
        return reinterpret_cast<T*>(data);
    }
}

template<typename T>
const T* Optional<T>::operator->() const {
    if (!defined) {
        throw BadOptionalAccess();
    } else {
        return reinterpret_cast<const T*>(data);
    }
}

template<typename T>
T& Optional<T>::value() {
    if (!defined) {
        throw BadOptionalAccess();
    } else {
        return *reinterpret_cast<T*>(data);
    }
}

template<typename T>
const T& Optional<T>::value() const {
    if (!defined) {
        throw BadOptionalAccess();
    } else {
        return *reinterpret_cast<const T*>(data);
    }
}

template<typename T>
void Optional<T>::reset() {
    if (defined) {
        this->value().~T();
    }
    defined = false;
}

template<typename T>
Optional<T>::~Optional() {
    if (defined) {
        this->value().~T();
    }
}

