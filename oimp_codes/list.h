#include <cstddef>
#include <cstdint>

template <typename T>
class List {
private:
    class Node {
    public:
        T val;

        Node* l_ptr;
        Node* r_ptr;

        explicit Node(const T& _val) : val(_val), l_ptr(nullptr), r_ptr(nullptr) {}
    };

    class Iterator {
    private:
        Node* cur_node;
        bool is_end;  // needed for end() without keeping another Node*

    public:
        explicit Iterator(Node* node, bool _is_end) : cur_node(node), is_end(_is_end) {}

        Iterator& operator++();
        Iterator& operator--();

        Iterator operator++(int);
        Iterator operator--(int);

        bool operator==(const Iterator&) const noexcept;
        bool operator!=(const Iterator&) const noexcept;

        T& operator*();
        const T& operator*() const;
    };

    Node* first_elem;
    Node* last_elem;

    size_t sz;

public:
    List(): first_elem(nullptr), last_elem(nullptr), sz(0) {}
    List(const List&);

    ~List();

    List& operator=(const List&);

    Iterator begin() const;
    Iterator end() const;

    void push_back(const T& elem);
    void push_front(const T& elem);

    void pop_back();
    void pop_front();

    size_t size() const noexcept;

    void erase();
};

template <typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    if (!this->cur_node->r_ptr) {
        is_end = true;
    } else {
        this->cur_node = this->cur_node->r_ptr;
    }
    return *this;
}

template<typename T>
typename List<T>::Iterator &List<T>::Iterator::operator--() {
    if (is_end) {
        is_end = false;
    } else {
        this->cur_node = this->cur_node->l_ptr;
    }
    return *this;
}

template<typename T>
typename List<T>::Iterator List<T>::Iterator::operator++(int) {
    Iterator prev = *this;
    ++(*this);
    return prev;
}

template<typename T>
typename List<T>::Iterator List<T>::Iterator::operator--(int) {
    Iterator prev = *this;
    --(*this);
    return prev;
}

template<typename T>
bool List<T>::Iterator::operator==(const List::Iterator& other) const noexcept {
    return (is_end == other.is_end) && (cur_node == other.cur_node);
}

template<typename T>
bool List<T>::Iterator::operator!=(const List::Iterator& other) const noexcept {
    return !(*this == other);
}

template<typename T>
T& List<T>::Iterator::operator*() {
    return cur_node->val;
}

template<typename T>
const T& List<T>::Iterator::operator*() const {
    return cur_node->val;
}

template<typename T>
List<T>::List(const List& other): List() {
    for (const T& elem : other) {
        push_back(elem);
    }
}

template<typename T>
List<T>::~List() {
    erase();
}

template<typename T>
List<T>& List<T>::operator=(const List& other) {
    erase();
    for (const T& elem : other) {
        push_back(elem);
    }
    return *this;
}

template<typename T>
typename List<T>::Iterator List<T>::begin() const {
    return Iterator(first_elem, !first_elem);
}

template<typename T>
typename List<T>::Iterator List<T>::end() const {
    return Iterator(last_elem, true);
}

template<typename T>
void List<T>::push_back(const T& elem) {
    if (size() == 0) {
        first_elem = last_elem = new Node(elem);
    } else {
        Node* cur_node = new Node(elem);
        last_elem->r_ptr = cur_node;
        cur_node->l_ptr = last_elem;
        last_elem = cur_node;
    }
    ++sz;
}

template<typename T>
void List<T>::push_front(const T& elem) {
    if (size() == 0) {
        first_elem = last_elem = new Node(elem);
    } else {
        Node* cur_node = new Node(elem);
        first_elem->l_ptr = cur_node;
        cur_node->r_ptr = first_elem;
        first_elem = cur_node;
    }
    ++sz;
}

template<typename T>
void List<T>::pop_back() {
    if (size() == 1) {
        delete last_elem;
        first_elem = last_elem = nullptr;
    } else {
        Node* prev = last_elem->l_ptr;
        delete last_elem;
        prev->r_ptr = nullptr;
        last_elem = prev;
    }
    --sz;
}

template<typename T>
void List<T>::pop_front() {
    if (size() == 1) {
        delete first_elem;
        first_elem = last_elem = nullptr;
    } else {
        Node* nxt = first_elem->r_ptr;
        delete first_elem;
        nxt->l_ptr = nullptr;
        first_elem = nxt;
    }
    --sz;
}

template<typename T>
size_t List<T>::size() const noexcept {
    return sz;
}

template<typename T>
void List<T>::erase() {
    while (size() > 0) {
        pop_back();
    }
}

