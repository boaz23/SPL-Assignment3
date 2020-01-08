
#ifndef STOMPCLIENT_ARRAY_H
#define STOMPCLIENT_ARRAY_H


#include <cstddef>

template <typename T> struct Array {
    const T* array;
    size_t  length;

    // TODO: maybe implement rule of 5
    // TODO: keep in mind the ownership of the array pointer
};


#endif //STOMPCLIENT_ARRAY_H
