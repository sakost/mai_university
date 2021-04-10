//
// Created by sakost on 22.11.2020.
//

#ifndef OOP_EXCERCISE_06_CUSTOM_VECTOR_H
#define OOP_EXCERCISE_06_CUSTOM_VECTOR_H

#include <cstring>
#include <memory>
#include "forward_vector_iterator.h"


template<typename T, typename Allocator=std::allocator<T>>
struct CustomVector {
    static_assert(std::is_same<typename std::remove_cv<T>::type, T>::value,
                  "vector must have a non-const, non-volatile value_type");

    using self_type = CustomVector<T, Allocator>;
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using iterator = ForwardVectorIterator<self_type, T>;
    using const_iterator = ForwardVectorIterator<const self_type, const T>;
    using size_type = std::size_t;
    using pointer = T *;
    using alloc_type = Allocator;

    CustomVector() : CustomVector(0) {}

    explicit CustomVector(alloc_type &default_alloc) : CustomVector(0, value_type(), default_alloc){}

    explicit CustomVector(size_type n, value_type const &default_value = value_type(), alloc_type default_alloc = alloc_type())
    {
        allocator = default_alloc;
        resize(n);
        fill(default_value);
    }

    void fill(value_type const &value) {
        for (auto &item : *this) {
            item = value;
        }
    }


    CustomVector(const_iterator &&begin_, const_iterator &&end_, alloc_type &default_alloc = alloc_type()): CustomVector(default_alloc) {
        allocator = default_alloc;
        _copy(begin_, end_);
    }

    CustomVector(const_iterator &begin_, const_iterator &end_, alloc_type &default_alloc = alloc_type()): CustomVector(default_alloc) {
        allocator = default_alloc;
        _copy(begin_, end_);
    }

    void _copy(const_iterator &begin_, const_iterator &end_) {
        auto begin_copy = begin_;
        auto end_copy = end_;

        for (; begin_copy != end_copy; ++begin_copy) {
            push_back(*begin_copy);
        }
    }

    void clear() {
        _start = nullptr;
        _finish = _end_of_storage = nullptr;
    }

    [[nodiscard]] size_type size() const {
        if (_finish == nullptr || _end_of_storage == nullptr || !_start) {
            return 0;
        }
        return size_type(_finish - _start.get());
    }

    [[nodiscard]] size_type capacity() {
        if (_end_of_storage == nullptr || !_start) {
            return 0;
        }
        return size_type(_end_of_storage - _start.get());
    }

    [[nodiscard]] bool empty() {
        return _start.get() == nullptr || _start.get() == _finish;
    }

    void resize(size_type n, value_type const &default_value = value_type()) {
        if (n == size()) return;
        if (n < size()) {
            _finish = _start.get() + n;
            return;
        }
        size_type prev_size = size();
        if (capacity() <= n) {
            _realloc(_calculate_new_capacity(n));
        }
        for (size_type i = prev_size; i < n; ++i) {
            *(_start.get() + i) = default_value;
        }
        _finish = _start.get() + n;
    }

    void _realloc(size_type n) {
        if (_start == nullptr) {
            _start = _node_construct(_allocate(n));
            _finish = _start.get();
            _end_of_storage = _start.get() + n;
            return;
        }
        auto new_ptr = _node_construct(_allocate(n));
        if (size() > 0)
            std::memcpy((void *) new_ptr.get(), (void *) _start.get(), std::min(n, size()) * sizeof(value_type));
        size_type prev_size = size();
        _start = std::move(new_ptr);
        _finish = _start.get() + prev_size;
        _end_of_storage = _start.get() + n;
    }

    pointer _allocate(size_type n){
        pointer ptr = allocator.allocate(n);
        for (size_type i = 0; i < n; ++i) {
            allocator.construct(ptr + i);
        }
        return ptr;
    }

    std::shared_ptr<value_type> _node_construct(pointer p){
        auto deleter = [this](pointer p){
            auto size = this->capacity();
            this->allocator.destroy(p);
            this->allocator.deallocate(p, size);
        };
        return std::shared_ptr<value_type> (p, deleter);
    }

    static size_type _calculate_new_capacity(size_type n) {
        if (n == 1) return 2;
        return n * 3 / 2;
    }

    iterator begin() {
        return iterator(_start.get(), this, 0);
    }

    iterator end() {
        return iterator(_start.get(), this, size());
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }

    const_iterator cbegin() const {
        return const_iterator(_start.get(), this, 0);
    }

    const_iterator cend() const {
        return const_iterator(_start.get(), this, size());
    }

    reference back() {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return *(_finish - 1);
    }

    reference front() {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return *_start;
    }

    void push_back(value_type const element) {
        if (size() == capacity()) {
            resize(size() + 1);
            _finish--;
        }
        *_finish = element;
        _finish++;
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Index cannot be equal or below zero");
        }
        allocator.destroy(--_finish);
    }

    reference operator[](size_type index) {
        return *(_start.get() + index);
    };

    const_reference operator[](size_type index) const {
        return *(_start.get() + index);
    };

    reference at(size_type index) {
        if (index >= size()) {
            throw std::out_of_range("Index cannot be greater or equal of size of vector");
        }
        return (*this)[index];
    }

    const_reference at(size_type index) const {
        if (index >= size()) {
            throw std::out_of_range("Index cannot be greater or equal of size of vector");
        }
        return (*this)[index];
    }

    bool compatible_iterator(const iterator &iter) {
        return this == iter.container;
    }

    bool compatible_iterator(const const_iterator &iter) {
        return this == iter.container;
    }

    void insert(iterator &&iter, value_type value) {
        assert(compatible_iterator(iter));
        size_type index = iter.index;
        value_type last = _move(iter, end(), 1);
        resize(size() + 1);
        (*this)[size() - 1] = last;
        (*this)[index] = value;
    }

    void insert(iterator &iter, value_type value) {
        assert(compatible_iterator(iter));
        size_type index = iter.index;
        value_type last = _move(iter, end(), 1);
        resize(size() + 1);
        (*this)[size() - 1] = last;
        (*this)[index] = value;
    }

    value_type _move(const iterator &_begin, const iterator &_end, typename iterator::difference_type offset) {
        value_type tmp;
        iterator iter;
        for (iter = _begin; iter != _end; iter = std::next(iter, offset)) {
            value_type tmp2 = *iter;
            *iter = tmp;
            tmp = tmp2;
        }
        return tmp;
    }

    void erase(iterator &iter) {
        assert(compatible_iterator(iter));
        if (size() == 0) {
            throw std::out_of_range("Index cannot be equal or below zero");
        }
        if (iter.index >= size()) {
            throw std::out_of_range("Index cannot be greater or equal of size of vector");
        }
        (*this)[iter.index] = _move(end(), iter, -1);
        resize(size() - 1);
        allocator.destroy(_finish);
    }

    void erase(iterator &&iter) {
        assert(compatible_iterator(iter));
        if (size() == 0) {
            throw std::out_of_range("Index cannot be equal or below zero");
        }
        if (iter.index >= size()) {
            throw std::out_of_range("Index cannot be greater or equal of size of vector");
        }
        (*this)[iter.index] = _move(end(), iter, -1);
        resize(size() - 1);
        allocator.destroy(_finish);
    }

    std::shared_ptr<value_type> _start;
    pointer _finish, _end_of_storage;

    alloc_type allocator;
};


#endif //OOP_EXCERCISE_06_CUSTOM_VECTOR_H
