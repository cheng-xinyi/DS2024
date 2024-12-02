#include <cstdlib>  // for rand(), swap
#include <algorithm> // for swap
#include <iostream>

#ifndef VECTOR_H
#define VECTOR_H

typedef int Rank;
#define DEFAULT_CAPACITY 3

template <typename T>
class Vector {
protected:
    Rank _size;
    int _capacity;
    T* _elem;

    void copyFrom(T const* A, Rank lo, Rank hi);
    void expand();
    void shrink();
    
    
    Rank max(Rank lo, Rank hi);
    void selectionSort(Rank lo, Rank hi);
    
    Rank partition(Rank lo, Rank hi);
    void quickSort(Rank lo, Rank hi);
    void heapSort(Rank lo, Rank hi);
    static Rank binSearch(T* A, T const& e, Rank lo, Rank hi);  // 二分查找
    static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi);  // 斐波那契查找
public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) // 容量为c，规模为s，所有元素初始为v
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }

    Vector(T const* A, Rank n) { copyFrom(A, 0, n); }
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

    // 析构函数
    ~Vector() { delete[] _elem; }


    // 添加元素到末尾（push_back 方法）
    void push_back(const T& element) {
        insert(_size, element);
    }
    // 只读访问接口
    Rank size() const { return _size; }
    bool empty() const { return !_size; }
    int disordered() const;
    Rank find(T const& e) const { return find(e, 0, _size); }
    Rank find(T const& e, Rank lo, Rank hi) const;
    Rank search(T const& e) const { return (0 >= _size) ? -1 : search(e, 0, _size); }
    Rank search(T const& e, Rank lo, Rank hi) const;

    // 可写访问结构
    T& operator[] (Rank r) const;
    Vector<T>& operator = (Vector<T> const&);
    T remove(Rank r);
    int remove(Rank lo, Rank hi);
    Rank insert(Rank lo, T const& e);
    Rank insert(T const& e) { return insert(_size, e); }
    void sort(Rank lo, Rank hi);
    void sort() { sort(0, _size); }
    bool bubble(Rank lo, Rank hi);
    void bubbleSort(Rank lo, Rank hi);
    void merge(Rank lo, Rank mi, Rank hi);
    void mergeSort(Rank lo, Rank hi);
    void unsort(Rank lo, Rank hi);
    void unsort() { unsort(0, _size); }
    int deduplicate();
    int uniquify();
    void traverse(void(*)(T&));

    template <typename VST>
    void traverse(VST&);
};

template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
        _elem[_size++] = A[lo++];
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
    if (_elem) delete[] _elem;
    copyFrom(V._elem, 0, V.size());
    return *this;
}

template <typename T>
void Vector<T>::expand() {
    if (_size < _capacity) return;
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
    T* oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}

template <typename T>
void Vector<T>::shrink() {
    if (_capacity < DEFAULT_CAPACITY << 1) return;
    if (_size << 2 > _capacity) return;
    T* oldElem = _elem;
    _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}

template <typename T>
T& Vector<T>::operator[](Rank r) const {
    return _elem[r];
}

template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {
    T* V = _elem + lo;
    for (Rank i = hi - lo; i > 0; i--)
        std::swap(V[i - 1], V[rand() % i]);
}

template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    while (lo < hi) {
        --hi;  // 先递减 hi，因为索引是从 [lo, hi) 左闭右开区间
        if (_elem[hi] == e) return hi;  // 找到元素，返回索引
    }
    return -1;  // 未找到元素，返回 -1
}


template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
    expand();
    for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
    _elem[r] = e;
    _size++;
    return r;
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
    if (lo == hi) return 0;
    while (hi < _size) _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}

template <typename T>
T Vector<T>::remove(Rank r) {
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}

template <typename T>
int Vector<T>::deduplicate() {
    int oldSize = _size;
    Rank i = 1;
    while (i < _size)
        (find(_elem[i], 0, i) < 0) ? i++ : remove(i);
    return oldSize - _size;
}

template <typename T>
int Vector<T>::uniquify() {
    Rank i = 0, j = 0;
    while (++j < _size)
        if (_elem[i] != _elem[j]) _elem[++i] = _elem[j];
    _size = ++i;
    shrink();
    return j - i;
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    switch (rand() % 5) {
    case 1: bubbleSort(lo, hi); break;
    case 2: selectionSort(lo, hi); break;
    case 3: mergeSort(lo, hi); break;
    case 4: heapSort(lo, hi); break;
    default: quickSort(lo, hi); break;
    }
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
    while (!bubble(lo, hi--));
}

template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {
    bool sorted = true;
    while (++lo < hi)
        if (_elem[lo - 1] > _elem[lo]) {
            sorted = false;
            std::swap(_elem[lo - 1], _elem[lo]);
        }
    return sorted;
}


template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) {
    T pivot = _elem[lo];  // 选定最左侧为pivot
    while (lo < hi) {
        while (lo < hi && pivot <= _elem[hi]) hi--;
        _elem[lo] = _elem[hi];  // 将比pivot小的移到左侧
        while (lo < hi && _elem[lo] <= pivot) lo++;
        _elem[hi] = _elem[lo];  // 将比pivot大的移到右侧
    }
    _elem[lo] = pivot;  // 将pivot归位
    return lo;  // 返回pivot的位置
}

template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;  // 递归基
    Rank mi = partition(lo, hi - 1);
    quickSort(lo, mi);
    quickSort(mi + 1, hi);
}

template <typename T>
void Vector<T>::heapSort(Rank lo, Rank hi) {
    // 将区间元素转化为一个最大堆
    std::make_heap(_elem + lo, _elem + hi);
    // 逐个取出堆顶元素
    for (Rank i = hi - 1; i > lo; i--) {
        std::swap(_elem[lo], _elem[i]);  // 将最大元素放到末尾
        std::make_heap(_elem + lo, _elem + i);  // 重新调整堆
    }
}

template <typename T>
Rank Vector<T>::max(Rank lo, Rank hi) {
    Rank maxIdx = lo;
    for (Rank i = lo + 1; i < hi; i++) {
        if (_elem[maxIdx] < _elem[i]) {
            maxIdx = i;
        }
    }
    return maxIdx;
}

template <typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi) {
    while (lo < --hi) {
        Rank maxIdx = max(lo, hi + 1);
        std::swap(_elem[hi], _elem[maxIdx]);
    }
}


template <typename T>
Rank Vector<T>::binSearch(T* A, T const& e, Rank lo, Rank hi) {
    while (lo < hi) {
        Rank mi = (lo + hi) >> 1;
        if (e < A[mi]) hi = mi;
        else if (A[mi] < e) lo = mi + 1;
        else return mi;
    }
    return -1;  // 查找失败
}
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    T* A = _elem + lo;  // A[0, hi-lo) 为当前向量的子区间
    int lb = mi - lo;   // 前半段长度
    T* B = new T[lb];   // 临时空间，用来存储前半段
    for (Rank i = 0; i < lb; ++i) B[i] = A[i];  // 复制前半段到临时空间 B

    int lc = hi - mi;   // 后半段长度
    T* C = _elem + mi;  // C[0, hi-mi) 为当前向量的后半段

    // 合并两个有序的子区间
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k] <= B[j]))) A[i++] = C[k++];
    }
    delete[] B;  // 释放临时空间
}
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;  // 如果区间元素少于 2，则直接返回
    Rank mi = (lo + hi) / 2;  // 计算中间位置
    mergeSort(lo, mi);        // 递归排序前半部分
    mergeSort(mi, hi);        // 递归排序后半部分
    merge(lo, mi, hi);        // 归并两部分
}
template <typename T>
Rank Vector<T>::fibSearch(T* A, T const& e, Rank lo, Rank hi) {
    // 这里可以实现斐波那契查找算法，暂时省略
    return binSearch(A, e, lo, hi);  // 作为占位，先使用二分查找
}
#endif


