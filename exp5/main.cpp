#include <iostream>
#include "Vector.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// 辅助函数：生成随机向量
vector<int> generateRandomVector(int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, size * 10); // 生成1到size*10之间的随机数
    vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = distrib(gen);
    }
    return vec;
}

// 辅助函数：生成顺序向量
vector<int> generateSortedVector(int size) {
    vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = i + 1;
    }
    return vec;
}

// 辅助函数：生成逆序向量
vector<int> generateReversedVector(int size) {
    vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = size - i;
    }
    return vec;
}

// 辅助函数：检查向量是否已排序
bool isSorted(const vector<int>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1]) {
            return false;
        }
    }
    return true;
}

// (1) 起泡排序
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// (2) 插入排序
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// (3) 选择排序
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_index = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        swap(arr[i], arr[min_index]);
    }
}

// (4) 归并排序
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1);
    vector<int> R(n2);

    for (int i = 0; i < n1; ++i) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void mergeSortHelper(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void mergeSort(vector<int>& arr) {
    mergeSortHelper(arr, 0, arr.size() - 1);
}

// (5) 快速排序
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; ++j) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortHelper(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(vector<int>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}

// (6) 堆排序
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();

    // 构建最大堆
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }

    // 从堆中提取元素
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 测试函数
void testAlgorithm(const string& algorithmName, void (*sortFunc)(vector<int>&), const vector<int>& data) {
    vector<int> arr = data;
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    if (!isSorted(arr)) {
        cout << "Error: " << algorithmName << " did not sort the array correctly." << endl;
    }

    cout << setw(15) << left << algorithmName
         << setw(10) << left << duration.count() << " us" << endl;
}

int main() {
    int size = 10000; // 可以调整测试规模

    cout << "测试规模: " << size << endl << endl;

    // 顺序序列测试
    cout << "--- 顺序序列 ---" << endl;
    vector<int> sortedData = generateSortedVector(size);
    testAlgorithm("起泡排序", bubbleSort, sortedData);
    testAlgorithm("插入排序", insertionSort, sortedData);
    testAlgorithm("选择排序", selectionSort, sortedData);
    testAlgorithm("归并排序", mergeSort, sortedData);
    testAlgorithm("快速排序", quickSort, sortedData);
    testAlgorithm("堆排序", heapSort, sortedData);
    cout << endl;

    // 逆序序列测试
    cout << "--- 逆序序列 ---" << endl;
    vector<int> reversedData = generateReversedVector(size);
    testAlgorithm("起泡排序", bubbleSort, reversedData);
    testAlgorithm("插入排序", insertionSort, reversedData);
    testAlgorithm("选择排序", selectionSort, reversedData);
    testAlgorithm("归并排序", mergeSort, reversedData);
    testAlgorithm("快速排序", quickSort, reversedData);
    testAlgorithm("堆排序", heapSort, reversedData);
    cout << endl;

    // 随机序列测试
    cout << "--- 随机序列 ---" << endl;
    vector<int> randomData = generateRandomVector(size);
    testAlgorithm("起泡排序", bubbleSort, randomData);
    testAlgorithm("插入排序", insertionSort, randomData);
    testAlgorithm("选择排序", selectionSort, randomData);
    testAlgorithm("归并排序", mergeSort, randomData);
    testAlgorithm("快速排序", quickSort, randomData);
    testAlgorithm("堆排序", heapSort, randomData);
    cout << endl;

    return 0;
}
