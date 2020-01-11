#include "../include/Array.h"

template <typename T>
Array<T>::Array() : array(nullptr), length(0) { }

template <typename T>
Array<T>::Array(const T array[], size_t length) : array(array), length(length) { }

template <typename T>
Array<T>::Array(const Array<T> &other) : array(nullptr), length(0) {
    copy(other);
}

template <typename T>
Array<T>::Array(Array<T> &&other) noexcept : array(nullptr), length(0) {
    steal(other);
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T> &other) {
    if (&other != this) {
        clean();
        copy(other);
    }
    return *this;
}

template <typename T>
Array<T> & Array<T>::operator=(Array<T> &&other) noexcept {
    if (&other != this) {
        clean();
        steal(other);
    }
    return *this;
}

template <typename T>
Array<T>::~Array() {
    clean();
}

template <typename T>
void Array<T>::clean() {
    delete[] array;
}

template <typename T>
void Array<T>::copy(const Array<T> &other) {
    T copy[] = new T[other.length];
    for (int i = 0; i < other.length; ++i) {
        copy[i] = other.array[i];
    }
    length = other.length;
}

template <typename T>
void Array<T>::steal(Array<T> &other) {
    array = other.array;
    length = other.length;
    other.array = nullptr;
    other.length = 0;
}
