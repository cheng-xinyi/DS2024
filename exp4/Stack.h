#include "Vector.h"

#ifndef STACK_H
#define STACK_H

template <typename T>
class Stack : public Vector<T> {
public:
    // 继承 Vector 的 remove 函数
    using Vector<T>::remove;
    using Vector<T>::insert;
    using Vector<T>::size;
    using Vector<T>::operator[];

    // 添加元素到栈顶
    void push(const T& e) {
        this->insert(this->size(), e);
    }

    // 从栈顶移除并返回元素
    T pop() {
        if (this->size() == 0) {
            throw std::out_of_range("Stack is empty");
        }
        return this->remove(this->size() - 1);
    }

    // 查看栈顶元素
    T& top() {
        if (this->size() == 0) {
            throw std::out_of_range("Stack is empty");
        }
        return (*this)[this->size() - 1];
    }

    // 常量版本的 top
    const T& top() const {
        if (this->size() == 0) {
            throw std::out_of_range("Stack is empty");
        }
        return (*this)[this->size() - 1];
    }

    // 检查栈是否为空
    bool empty() const {
        return this->size() == 0;
    }
};

#endif
