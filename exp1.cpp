#include "Vector.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

using namespace std;

class ComplexNumber {
private:
    double real;
    double imag;
public:
    ComplexNumber(double r = 0, double i = 0) : real(r), imag(i) {}

    
    double getReal() const { return real; }

    
    double getImag() const { return imag; }

    
    double modulus() const { return sqrt(real * real + imag * imag); }

    // 运算符重载，用于比较复数
    bool operator>(const ComplexNumber& other) const {
    if (this->modulus() != other.modulus()) {
        return this->modulus() > other.modulus();  
    }
    return this->real > other.real;  
}
    bool operator<(const ComplexNumber& other) const {
        if (this->modulus() != other.modulus())
            return this->modulus() < other.modulus();
        return this->real < other.real;
    }

	bool operator<=(const ComplexNumber& other) const {
    	return (*this < other) || (*this == other);
	}


    bool operator==(const ComplexNumber& other) const {
        return (real == other.real) && (imag == other.imag);
    }

 
    friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& cn) {
        os << cn.real;
        if (cn.imag >= 0)
            os << " + " << cn.imag << "i";
        else
            os << " - " << -cn.imag << "i";
        return os;
    }
};

// 生成随机复数
ComplexNumber generateRandomComplex(double range = 100.0) {
    double real = ((double)rand() / RAND_MAX) * range;
    double imag = ((double)rand() / RAND_MAX) * range;
    return ComplexNumber(real, imag);
}

// 打印 Vector 中的复数
void printVector(const Vector<ComplexNumber>& V) {
    for (Rank i = 0; i < V.size(); ++i) {
        std::cout << V[i] << " ";
    }
    std::cout << std::endl;
}

Vector<ComplexNumber> findInRange(const Vector<ComplexNumber>& sortedVec, double m1, double m2) {
    Vector<ComplexNumber> result;
    
    for (Rank i = 0; i < sortedVec.size(); ++i) {
        double mod = sortedVec[i].modulus();
        if (mod >= m1 && mod < m2) {
            result.insert(result.size(), sortedVec[i]);
        }
    }
    
    return result;
}
int main() {
    srand(time(0));  // 初始化随机数生成器

    Vector<ComplexNumber> V, W, X, Y;
    for (int i = 0; i < 2000; ++i) {
        V.insert(V.size(), generateRandomComplex(50.0));
    }
    W = V;  
    X = V;  
    // 第一个问题：置乱和查找
    V.unsort();  // 置乱
    // 查找实部和虚部均为 20 的复数
    ComplexNumber target(20.0, 20.0);
    Rank found = V.find(target, 0, V.size()); 
    if (found != -1) {
        std::cout << "Found " << target << " at position: " << found << std::endl;
    } else {
        std::cout << "Complex number " << target << " not found." << std::endl;
    }
    // 插入一个新的复数 (25.0 + 25.0i) 到向量中位置 100
    V.insert(100, ComplexNumber(25.0, 25.0));
    // 删除第 125 个元素
    V.remove(125);
    // 去重操作
    int removedCount = V.deduplicate();
    // 第二个问题：排序算法的效率比较
    // 1. 无序向量的冒泡排序和归并排序时间
    cout<<"无序向量的冒泡排序和归并排序时间"<<endl;
    clock_t start = clock();
    W.bubbleSort(0, W.size());
    clock_t end = clock();
    double bubbleUnorderedTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "Bubble Sort (Unordered Vector) Time: " << bubbleUnorderedTime << " seconds" << std::endl;

    start = clock();
    W.mergeSort(0, W.size());
    end = clock();
    double mergeUnorderedTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "Merge Sort (Unordered Vector) Time: " << mergeUnorderedTime << " seconds" << std::endl;
	//2.有序向量的冒泡排序和归并排序时间
	cout<<"有序向量的冒泡排序和归并排序时间"<<endl;
	X.sort();
	Y=X;
	start = clock();
    X.bubbleSort(0, X.size());
    end = clock();
    double bubbleOrderedTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "Bubble Sort (Ordered Vector) Time: " << bubbleOrderedTime << " seconds" << std::endl;

    start = clock();
    X.mergeSort(0, X.size());
    end = clock();
    double mergeOrderedTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "Merge Sort (Ordered Vector) Time: " << mergeOrderedTime << " seconds" << std::endl;
	//3.逆序向量的冒泡排序和归并排序时间
	cout<<"逆序向量的冒泡排序和归并排序时间"<<endl;
	for (Rank i = 0; i < Y.size() / 2; ++i) {
        std::swap(Y[i], Y[Y.size() - 1 - i]);
    }
    start = clock();
    Y.bubbleSort(0, Y.size());
    end = clock();
    double bubbleReverseTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "Bubble Sort (Reverse Ordered Vector) Time: " << bubbleReverseTime << " seconds" << std::endl;

    start = clock();
    Y.mergeSort(0, Y.size());
    end = clock();
    double mergeReverseTime = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "Merge Sort (Reverse Ordered Vector) Time: " << mergeReverseTime << " seconds" << std::endl;
    //第三个问题
    double m1=20,m2=25;
    Vector<ComplexNumber> result = findInRange(X, m1, m2);
    std::cout << "\nComplex numbers with modulus in range [" << m1 << ", " << m2 << "):" << std::endl;
    printVector(result);
    return 0;
}
