#ifndef __MEMORY_POOL_HPP__
#define __MEMORY_POOL_HPP__

#include <climits>
#include <cstddef>
#include <memory>
#include <iostream>

template <typename T, size_t Blocksize = 4096>
class MemoryPool: public std::allocator<T> {
public:
    using ptr = T*;

    template<typename U> // an inner class/struct
    struct rebind {
        using other = MemoryPool<U>;
    };

    //default constructor
    MemoryPool() noexcept {
        current_block_ = nullptr;
        current_slot_ = nullptr;
        last_slot_ = nullptr;
        free_slots_ = nullptr;
    }

    //destructor
    ~MemoryPool() noexcept {
        slot_ptr_ cur = current_block_;
        while (cur != nullptr) {
            slot_ptr_ next = cur->next;
            operator delete(reinterpret_cast<void *>(cur));
            cur = next;
        }
    }

    ptr allocate(size_t n = 1, const T* hint = 0) {
        if (free_slots_ != nullptr) {
            ptr result = reinterpret_cast<ptr>(free_slots_);
            free_slots_ = free_slots_->next;
            return result;
        } else {
            if (current_slot_ >= last_slot_) {
                data_ptr_ new_block = reinterpret_cast<data_ptr_>(operator new(Blocksize));
                reinterpret_cast<slot_ptr_>(new_block)->next = current_block_;
                current_block_ = reinterpret_cast<slot_ptr_>(new_block);

                data_ptr_ body = new_block + sizeof(slot_ptr_);
                uintptr_t result = reinterpret_cast<uintptr_t>(body);

                size_t body_padding = (alignof(slot_type_) - result) % alignof(slot_type_);

                current_slot_ = reinterpret_cast<slot_ptr_>(body + body_padding);
                last_slot_ = reinterpret_cast<slot_ptr_>(new_block + Blocksize - sizeof(slot_type_));
            }
            return reinterpret_cast<ptr>(current_slot_++);
        }
    }

    void deallocate(ptr p, size_t n = 1) {
        if (p != nullptr) {
            reinterpret_cast<slot_ptr_> (p)->next = free_slots_;
            free_slots_ = reinterpret_cast<slot_ptr_>(p);
        }
    }

    template <typename U, typename... Args>
    void construct(U * p, Args&&... args) {
        new (p) U (std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U * p) {
        p->~U();
    }

private:
    union Slot_ {
        T element;
        Slot_* next;
    };

    using data_ptr_ = char *;
    using slot_type_ = Slot_;
    using slot_ptr_ = Slot_ *;

    slot_ptr_ current_block_;
    slot_ptr_ current_slot_;
    slot_ptr_ last_slot_;
    slot_ptr_ free_slots_;

    static_assert(Blocksize >= 2 * sizeof(slot_type_), "Block size too small");

};

#endif
