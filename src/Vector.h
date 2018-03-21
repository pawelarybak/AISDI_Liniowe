#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>

namespace aisdi {

    template <typename Type>
    class Vector {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        Vector() {
            data_array = new Type[INIT_SIZE];
            allocated_size = INIT_SIZE;
            elements = 0;
        }

        Vector(std::initializer_list<Type> l) {
            elements = allocated_size = l.size();
            data_array = new Type[allocated_size];
            std::copy(l.begin(), l.end(), data_array);
        }

        Vector(const Vector& other) {
            elements = allocated_size = other.getSize();
            data_array = new Type[allocated_size];
            std::copy(other.begin(), other.end(), data_array);
        }

        Vector(Vector&& other) {
            data_array = other.data_array;
            other.data_array = nullptr;
            elements = other.elements;
            allocated_size = other.allocated_size;
        }

        ~Vector() {
            delete[] data_array;
        }

        Vector& operator=(const Vector& other) {
            if (this == &other) {
                return *this;
            }
            delete[] data_array;
            data_array = new Type[other.allocated_size];
            std::copy(other.cbegin(), other.cend(), begin());
            allocated_size = other.allocated_size;
            elements = other.elements;
            return *this;
        }

        Vector& operator=(Vector&& other) {
            if (this == &other) {
                return *this;
            }
            delete[] data_array;
            data_array = other.data_array;
            other.data_array = nullptr;
            allocated_size = other.allocated_size;
            elements = other.elements;
            return *this;
        }

        bool isEmpty() const {
            return getSize() == 0;
        }

        size_type getSize() const {
            return elements;
        }

        void append(const Type& item) {
            insert(end(), item);
        }

        void prepend(const Type& item) {
            insert(begin(), item);
        }

        void insert(const const_iterator& insertPosition, const Type& item) {
            difference_type dst =  insertPosition - cbegin();
            if (elements == allocated_size) {
                reallocate();
            }
            for (pointer it = data_array + elements - 1; it >= data_array + dst; --it) {
                *(it + 1) = *it;
            }
            ++elements;
            *(begin() + dst) = item;
        }

        Type popFirst() {
            if (isEmpty()) {
                throw std::logic_error("You cannot pop from empty collection");
            }
            Type val = data_array[0];
            std::copy(++begin(), end(), begin());
            --elements;
            return val;
        }

        Type popLast() {
            if (isEmpty()) {
                throw std::logic_error("You cannot pop from empty collection");
            }
            --elements;
            return data_array[elements];
        }

        void erase(const const_iterator& position) {
            std::copy(position + 1, cend(), iterator(position));
            --elements;
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) {
            std::copy(lastExcluded, cend(), iterator(firstIncluded));
            elements -= lastExcluded - firstIncluded;
        }

        iterator begin() {
            return iterator(data_array, *this);
        }

        iterator end() {
            return iterator(data_array + elements, *this);
        }

        const_iterator cbegin() const {
            return const_iterator(data_array, *this);
        }

        const_iterator cend() const {
            return const_iterator(data_array + elements, *this);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    private:
        void reallocate() {
            pointer new_arr = new Type[allocated_size * 2];
            std::copy(begin(), end(), new_arr);
            allocated_size *= 2;
            delete[] data_array;
            data_array = new_arr;
        }

        pointer data_array;
        size_type elements;
        size_type allocated_size;

        size_type INIT_SIZE = 4;
    };

    template <typename Type>
    class Vector<Type>::ConstIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;

        explicit ConstIterator(pointer ptr, const Vector<Type>& parent) : current_pointer(ptr), parent(parent) {}

        reference operator*() const {
            if (*this < parent.begin() || *this >= parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            return *(current_pointer);
        }

        ConstIterator& operator++() {
            if (*this >= parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            ++current_pointer;
            return *this;
        }

        ConstIterator operator++(int) {
            if (*this >= parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            ConstIterator result = *this;
            ++current_pointer;
            return result;
        }

        ConstIterator& operator--() {
            if (*this <= parent.begin()) {
                throw std::out_of_range("Iterator out of range");
            }
            --current_pointer;
            return *this;
        }

        ConstIterator operator--(int) {
            if (*this <= parent.begin()) {
                throw std::out_of_range("Iterator out of range");
            }
            ConstIterator result = *this;
            --current_pointer;
            return result;
        }

        ConstIterator& operator+=(difference_type d) {
            current_pointer += d;
            return *this;
        }

        ConstIterator& operator-=(difference_type d) {
            current_pointer -= d;
            return *this;
        }

        ConstIterator operator+(difference_type d) const {
            ConstIterator new_iter = *this;
            new_iter += d;
            return new_iter;
        }

        difference_type operator-(const ConstIterator &other) const {
            return current_pointer - other.current_pointer;
        }

        ConstIterator operator-(difference_type d) const {
            ConstIterator new_iter = *this;
            new_iter -= d;
            return new_iter;
        }

        bool operator==(const ConstIterator& other) const {
            return current_pointer == other.current_pointer;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }

        bool operator<=(const ConstIterator &other) const {
            return current_pointer <= other.current_pointer;
        }

        bool operator>=(const ConstIterator &other) const {
            return current_pointer >= other.current_pointer;
        }

        bool operator<(const ConstIterator &other) const {
            return !(*this >= other);
        }

        bool operator>(const ConstIterator &other) const {
            return !(*this <= other);
        }

    protected:
        pointer current_pointer;
        const Vector<Type>& parent;
    };

    template <typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;

        explicit Iterator(pointer ptr, Vector<Type>& parent) : ConstIterator(ptr, parent) {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other) {}

        Iterator& operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const {
            return ConstIterator::operator-(d);
        }

        reference operator*() const {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_VECTOR_H
