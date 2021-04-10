//
// Created by sakost on 23.11.2020.
//

#ifndef OOP_EXCERCISE_06_CUSTOMALLOCATOR_H
#define OOP_EXCERCISE_06_CUSTOMALLOCATOR_H

#include <list>
#include <cinttypes>
#include <cstdlib>

template <typename T, std::size_t BLOCK_SIZE>
struct CustomAllocator {
    static_assert(BLOCK_SIZE != 0, "the block must be above zero");

    using value_type = T;
    using pointer = T *;
    using size_type = std::size_t;
    CustomAllocator(): _used_blocks(nullptr), _free_blocks(){}

    template<typename U>
    struct rebind{
        using other = CustomAllocator<U, BLOCK_SIZE>;
    };

    pointer allocate(size_type count = 1){
        if(_used_blocks == nullptr){
            _used_blocks = (pointer)malloc(BLOCK_SIZE * sizeof(value_type));
            for (size_type i = 0; i < BLOCK_SIZE; ++i)
            {
                _free_blocks.push_back(_used_blocks + i);
            }
        }
        size_type ind = free_space(count);
        _free_blocks.erase(std::next(_free_blocks.begin(), ind), std::next(_free_blocks.begin(), count + ind + 1));
        return _used_blocks + ind;
    }

    void deallocate(pointer ptr, size_type count = 1){
        for (int i = 0; i < count; ++i)
        {
            _free_blocks.push_front(ptr + i);
        }
    }

    bool empty(){
        return _free_blocks.empty();
    }

    int32_t size(){
        return _free_blocks.size();
    }

    ~CustomAllocator(){
        while(!_free_blocks.empty()){
            _free_blocks.pop_back();
        }

        free(_used_blocks);
    }

    size_type free_space(size_type n)
    {
        size_type total = 0;

        auto i = _free_blocks.rbegin();
        for (; i != _free_blocks.rend(); ++i)
        {
            total = 1;
            auto ptr_a = i;
            auto ptr_b = std::next(ptr_a, 1);
            for (; ptr_b != _free_blocks.rend() && total < n; ++ptr_b, ++ptr_a)
            {
                if (*ptr_a - *ptr_b == 1)
                    ++total;
            }
            if (total >= n)
                break;
        }

        if (total < n)
            throw std::bad_alloc();

        return std::distance(i, _free_blocks.rbegin());
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&... args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    void destroy(pointer p)
    {
        p->~T();
    }

    pointer _used_blocks = nullptr;
    std::list<pointer> _free_blocks;
};


#endif //OOP_EXCERCISE_06_CUSTOMALLOCATOR_H
