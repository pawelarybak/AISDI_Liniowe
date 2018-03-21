#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
//#include <bits/valarray_before.h>

namespace aisdi
{

    template <typename Type>
    class LinkedList
    {
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

        using element = struct Element {
            pointer value;
            struct Element* next;
            struct Element* prev;

            Element(pointer val) : value(val) {}
            ~Element() {
                delete value;
            }
        };
        using element_pointer = element*;

        LinkedList() : size(0) {
            element_pointer guardian = new Element(nullptr);
            root = tail = guardian;
            guardian->next = nullptr;
            guardian->prev = nullptr;
        }

        LinkedList(std::initializer_list<Type> l) : LinkedList() {
            for (const auto& val : l) {
                insert(end(), val);
            }
        }

        LinkedList(const LinkedList& other) : LinkedList() {
            for (auto it = other.begin(); it != other.end(); ++it) {
                insert(end(), *it);
            }
        }

        LinkedList(LinkedList&& other) {
            root = other.root;
            tail = other.tail;
            size = other.size;
            other.root = nullptr;
            other.tail = nullptr;
        }

        ~LinkedList() {
            element_pointer next = root;
            while (next != nullptr) {
                element_pointer to_delete = next;
                next = to_delete->next;
                delete to_delete;
            }
        }

        LinkedList& operator=(const LinkedList& other) {
            if (this == &other) {
                return *this;
            }
            erase(cbegin(), cend());
            for (auto it = other.begin(); it != other.end(); ++it) {
                insert(end(), *it);
            }
            return *this;
        }

        LinkedList& operator=(LinkedList&& other) {
            if (this == &other) {
                return *this;
            }
            element_pointer next = root;
            while (next != nullptr) {
                element_pointer to_delete = next;
                next = to_delete->next;
                delete to_delete;
            }
            root = other.root;
            tail = other.tail;
            size = other.size;
            other.root = nullptr;
            other.tail = nullptr;
            return *this;
        }

        bool isEmpty() const {
            return getSize() == 0;
        }

        size_type getSize() const {
            return size;
        }

        void append(const Type& item) {
            insert(end(), item);
        }

        void prepend(const Type& item) {
            insert(begin(), item);
        }

        void insert(const const_iterator& insertPosition, const Type& item) {
            element_pointer new_element = new Element(new value_type(item));
            new_element->next = insertPosition.element();
            new_element->prev = insertPosition.element()->prev;
            new_element->next->prev = new_element;

            if (new_element->prev != nullptr) {
                new_element->prev->next = new_element;
            } else {
                root = new_element;
            }
            ++size;
        }

        Type popFirst() {
            if (isEmpty()) {
                throw std::logic_error("You cannot pop from empty collection");
            }
            Type ret_val = *begin();
            erase(begin());
            return ret_val;
        }

        Type popLast() {
            if (isEmpty()) {
                throw std::logic_error("You cannot pop from empty collection");
            }
            Type ret_val = *(--end());
            erase(--end());
            return ret_val;
        }

        void erase(const const_iterator& position) {
            if (position == end()) {
                throw std::out_of_range("Iterator out of range");
            }
            element_pointer del_el = position.element();
            del_el->next->prev = del_el->prev;
            if (del_el->prev != nullptr) {
                del_el->prev->next = del_el->next;
            } else {
                root = del_el->next;
            }
            delete del_el;
            --size;
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) {
            element_pointer frst_el = firstIncluded.element();
            element_pointer lst_el = lastExcluded.element();
            lst_el->prev = frst_el->prev;
            if (frst_el->prev != nullptr) {
                frst_el->prev->next = lst_el;
            } else {
                root = lst_el;
            }
            element_pointer next = frst_el;
            while (next != lst_el) {
                element_pointer to_delete = next;
                next = to_delete->next;
                delete to_delete;
                --size;
            }
        }

        iterator begin()
        {
            return iterator(root, *this);
        }

        iterator end()
        {
            return iterator(tail, *this);
        }

        const_iterator cbegin() const
        {
            return const_iterator(root, *this);
        }

        const_iterator cend() const
        {
            return const_iterator(tail, *this);
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        const_iterator end() const
        {
            return cend();
        }

    private:
        element_pointer root;
        element_pointer tail;
        size_type size;
    };

    template <typename Type>
    class LinkedList<Type>::ConstIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        using element_pointer = typename LinkedList::element_pointer;

        explicit ConstIterator(element_pointer el, const LinkedList& parent) : current_element(el), parent(parent) {}

        reference operator*() const {
            if (*this == parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            return *(current_element->value);
        }

        element_pointer element() const {
            return current_element;
        }

        ConstIterator& operator++() {
            if (*this == parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            current_element = current_element->next;
            return *this;
        }

        ConstIterator operator++(int) {
            if (*this == parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            ConstIterator result = *this;
            current_element = current_element->next;
            return result;
        }

        ConstIterator& operator--() {
            if (*this == parent.begin()) {
                throw std::out_of_range("Iterator out of range");
            }
            current_element = current_element->prev;
            return *this;
        }

        ConstIterator operator--(int) {
            if (*this == parent.begin()) {
                throw std::out_of_range("Iterator out of range");
            }
            ConstIterator result = *this;
            current_element = current_element->prev;
            return result;
        }

        ConstIterator& operator+=(difference_type d) {
            for (int i = 0; i < d; ++i, ++(*this));
            return *this;
        }

        ConstIterator& operator-=(difference_type d) {
            for (int i = 0; i < d; ++i, --(*this));
            return *this;
        }

        ConstIterator operator+(difference_type d) const {
            ConstIterator new_iter = *this;
            new_iter += d;
            return new_iter;
        }

        ConstIterator operator-(difference_type d) const {
            ConstIterator new_iter = *this;
            new_iter -= d;
            return new_iter;
        }

        bool operator==(const ConstIterator& other) const {
            return current_element == other.current_element;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }

    private:
        element_pointer current_element;
        const LinkedList& parent;
    };

    template <typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
    {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator(element_pointer el, const LinkedList& parent) : ConstIterator(el, parent) {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator& operator++()
        {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int)
        {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--()
        {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int)
        {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const
        {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const
        {
            return ConstIterator::operator-(d);
        }

        reference operator*() const
        {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H
