#ifndef ALGORITHM_H
#define ALGORITHM_H

template <typename T>
void swap(T& a, T& b)
{
    T tmp(a);
    a = b;
    b = tmp;
}

#endif
