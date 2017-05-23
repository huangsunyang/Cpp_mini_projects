#include <memory>

template <typename T>
class StackNode_ {
public:
    T data;
    StackNode_ * prev;
};

template <typename T, typename Alloc = std::allocator<T>>
class StackAlloc {
public:
    typedef StackNode_<T> Node;
    typedef typename Alloc::template rebind<Node>::other allocator;

    StackAlloc(): head_(0) {}
    ~StackAlloc() {
        clear();
    }

    bool empty() {
        return head_ == nullptr;
    }

    void clear();

    void push(T element);

    T pop();

    T top() {
        return head_->data;
    }

private:
    allocator allocator_;
    Node * head_;
};

template <typename T, typename Alloc>
void StackAlloc<T, Alloc>::clear() {
    Node * cur = head_;
    
    while (cur != nullptr) {
        Node * temp = cur;
        cur = cur->prev;

        allocator_.destroy(temp);
        allocator_.deallocate(temp, 1);
    }

    head_ = nullptr;
}

template <typename T, typename Alloc>
void StackAlloc<T, Alloc>::push(T element) {
    Node * new_node = allocator_.allocate(1);

    allocator_.construct(new_node, Node());

    new_node->data = element;
    new_node->prev = head_;
    head_ = new_node;
}

template <typename T, typename Alloc>
T StackAlloc<T, Alloc>::pop() {
    T result = head_->data;
    Node * temp = head_;
    head_ = head_->prev;

    allocator_.destroy(temp);
    allocator_.deallocate(temp, 1);

    return result;
}
