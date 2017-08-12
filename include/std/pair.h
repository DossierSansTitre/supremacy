#ifndef STD_PAIR_H
#define STD_PAIR_H

template <typename T, typename U>
class Pair
{
public:
    Pair()
    {

    }

    Pair(const T& first, const U& second)
    : first(first)
    , second(second)
    {

    }

    Pair(const Pair<T, U>& pair)
    : first(pair.first)
    , second(pair.second)
    {

    }

    Pair<T, U>& operator=(const Pair<T, U>& pair)
    {
        first = pair.first;
        second = pair.second;
        return *this;
    }

    ~Pair()
    {

    }

    bool operator==(const Pair<T, U>& other) const
    {
        return (first == other.first && second == other.second);
    }

    bool operator!=(const Pair<T, U>& other) const
    {
        return !(*this == other);
    }

    T   first;
    U   second;
};

template <typename T, typename U>
Pair<T, U> make_pair(const T& first, const U& second)
{
    return Pair<T, U>(first, second);
}

#endif
