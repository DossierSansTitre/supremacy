#ifndef ALGORITHM_H
#define ALGORITHM_H

template <typename T>
void swap(T& a, T& b)
{
    T tmp(a);
    a = b;
    b = tmp;
}

template <typename Iter>
void iter_swap(Iter a, Iter b)
{
    swap(*a, *b);
}

template <typename Iter, typename Predicate>
Iter find_if(Iter first, Iter last, Predicate pred)
{
    for (Iter i = first; i != last; ++i)
    {
        if (pred(*i))
            return i;
    }
    return last;
}

template <typename Iter, typename Predicate>
Iter find_if_not(Iter first, Iter last, Predicate pred)
{
    for (Iter i = first; i != last; ++i)
    {
        if (!pred(*i))
            return i;
    }
    return last;
}

template <typename Iter, typename Predicate>
Iter partition(Iter first, Iter last, Predicate pred)
{
    first = find_if_not(first, last, pred);

    if (first == last)
        return first;

    for (Iter i = first + 1; i != last; ++i)
    {
        if (pred(*i))
        {
            iter_swap(i, first);
            ++first;
        }
    }
    return first;
}

template <typename Container>
void sort(Container& container)
{
    sort(container.begin(), container.end());
}

template <typename Iter>
void sort(Iter first, Iter last)
{
    if (first == last || first + 1 == last)
        return;

    auto pivot = *first;
    Iter middle = partition(first, last, [pivot](const auto& value) { return value < pivot; });

    sort(first, middle);
    sort(middle, last);
}

#endif
