
#ifndef STOMPCLIENT_ARRAY_H
#define STOMPCLIENT_ARRAY_H


#include <cstddef>

template <typename T>
struct Array {
public:
    const T* array;
    size_t  length;

    Array();
    Array(const T array[], size_t length);

    Array(const Array<T> &other);
    Array(Array<T> &&other) noexcept;

    Array<T>& operator=(const Array<T> &other);
    Array<T>& operator=(Array<T> &&other) noexcept;

    ~Array();

private:
    void clean();
    void copy(const Array<T> &other);
    void steal(Array<T> &other);
};


#endif //STOMPCLIENT_ARRAY_H
