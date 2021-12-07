// Copyright 2021 byteihq <kotov038@gmail.com>

#ifndef SHAREDPTR_SHAREDPTR_H
#define SHAREDPTR_SHAREDPTR_H

#include <atomic>   //for std::atomic_uint
#include <cstddef>  //for size_t
#include <utility>  //for std::swap

template<typename T>
class SharedPtr {
private:
    T *ptr_;
    std::atomic_uint *counter_;
public:
    SharedPtr() : ptr_(nullptr), counter_(nullptr) {}

    explicit SharedPtr(T *ptr) : ptr_(ptr), counter_(new std::atomic_uint(1)) {}

    SharedPtr(const SharedPtr &r) : ptr_(r.ptr_), counter_(r.counter_) {
        ++*counter_;
    }

    SharedPtr(SharedPtr &&r) noexcept: ptr_(r.ptr_), counter_(r.counter_) {
        r.ptr_ = nullptr;
        r.counter_ = nullptr;
    }

    ~SharedPtr() { reset(); }

    auto operator=(const SharedPtr &r) -> SharedPtr & {
        if (this != &r) {
            reset();
            counter_ = r.counter_;
            ++*counter_;
            ptr_ = r.ptr_;
        }
        return *this;
    }

    auto operator=(SharedPtr &&r) noexcept -> SharedPtr & {
        if (this != &r) {
            reset();
            counter_ = r.counter_;
            ptr_ = r.ptr_;
            r.ptr_ = nullptr;
            r.counter_ = nullptr;
        }
        return *this;
    }

    explicit operator bool() const { return ptr_ != nullptr; }

    auto operator*() const -> T & { return *ptr_; }

    auto operator->() const -> T * { return ptr_; }

    auto get() -> T * { return ptr_; }

    void reset() {
        if (counter_ == nullptr) {
            return;
        }
        --*counter_;
        if (*counter_ == 0) {
            delete ptr_;
            delete counter_;
        }
        ptr_ = nullptr;
        counter_ = nullptr;
    }

    void reset(T *ptr) {
        reset();
        ptr_ = ptr;
        counter_ = new std::atomic_uint(1);
    }

    void swap(SharedPtr &r) {
        std::swap(ptr_, r.ptr_);
        std::swap(counter_, r.counter_);
    }

    auto use_count() const -> size_t {
        return counter_ == nullptr ? 0u : static_cast<size_t>(*counter_);
    }
};

#endif //SHAREDPTR_SHAREDPTR_H
