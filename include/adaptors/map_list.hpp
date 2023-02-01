
#include <unordered_map>
#include <iterator> // std::bidirectional_iterator_tag
#include <cstddef> // std::ptrdiff_t

namespace graphxx {

template <typename Key, typename Value> class MapList : std::unordered_map<Key, Value> {

    public:
        using Base = std::unordered_map<Key, Value>;

    struct Iterator {

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Value;
        using pointer = Value*;
        using reference = Value&;

        Iterator(Base::iterator iterator) : _iterator(iterator) {}

        reference operator*() const { return _iterator->second ; }
        pointer operator->() { return &(_iterator->second); }

        Iterator& operator++() { ++_iterator; return *this; }
        Iterator operator++(int) { auto temp = *this; ++_iterator; return temp; }

        Iterator& operator--() { --_iterator; return *this; }
        Iterator operator--(int) { auto temp = *this; --_iterator; return temp; }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs)
        { return lhs._iterator == rhs._iterator; }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
        { return !(lhs == rhs); }

        private:
            Base::iterator _iterator;
    };

    Iterator begin() {
        return Iterator(Base::begin());
    }

    Iterator end() {
        return Iterator(Base::end());
    }

};
} // namespace graphxx