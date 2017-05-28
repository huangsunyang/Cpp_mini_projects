#ifndef __MY_ENABLE_SHARED_FROM_THIS__
#define __MY_ENABLE_SHARED_FROM_THIS__

#include <memory>

template <typename T>
class my_enable_shared_from_this {
public:
    my_enable_shared_from_this(): weak_ptr_() {}
    ~my_enable_shared_from_this() {}
    
    std::shared_ptr<T> shared_from_this() {
        return weak_ptr_.lock();
    }
    std::shared_ptr<T const> shared_from_this() const {
        return weak_ptr_.lock();
    }
protected:
    my_enable_shared_from_this& operator= (my_enable_shared_from_this const) {
        return *this;
    }

private:
    std::weak_ptr<T> weak_ptr_;
};

#endif
