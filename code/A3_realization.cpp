#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <algorithm>

class ArrayGenerator {
public:
    ArrayGenerator() { srand(time(nullptr)); }

    std::vector<int> generateRandom(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % 6001;
        }
        return arr;
    }

    std::vector<int> generateReverseSorted(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = size - i;
        }
        return arr;
    }

    std::vector<int> generateAlmostSorted(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = i + 1;
        }
        for (int i = 0; i < 10; i++) {
            int idx1 = rand() % size;
            int idx2 = rand() % size;
            std::swap(arr[idx1], arr[idx2]);
        }
        return arr;
    }
};

class SortTester {
    void insertionSort(std::vector<int>& arr, int left, int right) {
        for (int i = left + 1; i <= right; i++) {
            int key = arr[i];
            int j = i - 1;
            while (j >= left && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

    void heapify(std::vector<int>& arr, int i, int first, int last) {
        int left = first + 2 * (i - first) + 1;
        int right = first + 2 * (i - first) + 2;
        int largest = i;
        if (left <= last && arr[left] > arr[largest]) {
          largest = left;
        }
        if (right <= last && arr[right] > arr[largest]) {
          largest = right;
        }
        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            heapify(arr, largest, first, last);
        }
    }

    void heapSort(std::vector<int>& arr, int left, int right) {
        int n = right - left + 1;
        for (int i = left + n / 2 - 1; i >= left; i--) {
            heapify(arr, i, left, right);
        }
        for (int i = right; i > left; i--) {
            std::swap(arr[left], arr[i]);
            heapify(arr, left, left, i - 1);
        }
    }

    void quickSort(std::vector<int>& arr, int left, int right, int depth, int max_depth) {
        if (left >= right) {
          return;
        }

        if (right - left + 1 < 16) {
            insertionSort(arr, left, right);
            return;
        }
        if (depth >= max_depth) {
            heapSort(arr, left, right);
            return;
        }

        int pivotInd = left + rand() % (right - left + 1);
        int pivot = arr[pivotInd];
        std::swap(arr[pivotInd], arr[right]);

        int i = left - 1;
        for (int j = left; j < right; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[i + 1], arr[right]);
        int partInd = i + 1;

        quickSort(arr, left, partInd - 1, depth + 1, max_depth);
        quickSort(arr, partInd + 1, right, depth + 1, max_depth);
    }

    void classicQuickSort(std::vector<int>& arr, int left, int right) {
        if (left >= right) {
          return;
        }

        int pivotInd = left + rand() % (right - left + 1);
        int pivot = arr[pivotInd];
        std::swap(arr[pivotInd], arr[right]);

        int i = left - 1;
        for (int j = left; j < right; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[i + 1], arr[right]);
        int partInd = i + 1;

        classicQuickSort(arr, left, partInd - 1);
        classicQuickSort(arr, partInd + 1, right);
    }

public:
    SortTester() { srand(time(nullptr)); }

    double testClassicQuickSort(std::vector<int> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        classicQuickSort(arr, 0, arr.size() - 1);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    double testIntroSort(std::vector<int> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        int n = arr.size();
        int max_depth = 2 * log2(n);
        quickSort(arr, 0, n - 1, 0, max_depth);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
};

int main() {
    ArrayGenerator generator;
    SortTester tester;

    std::cout << "Size,Random_Classic,Random_Intro,Reverse_Classic,Reverse_Intro,Almost_Classic,Almost_Intro\n";

    for (int size = 500; size <= 10000; size += 500) {
        std::vector<int> randomArr = generator.generateRandom(size);
        std::vector<int> reverseArr = generator.generateReverseSorted(size);
        std::vector<int> almostArr = generator.generateAlmostSorted(size);

        double randomClassic = tester.testClassicQuickSort(randomArr);
        double randomIntro = tester.testIntroSort(randomArr);
        double reverseClassic = tester.testClassicQuickSort(reverseArr);
        double reverseIntro = tester.testIntroSort(reverseArr);
        double almostClassic = tester.testClassicQuickSort(almostArr);
        double almostIntro = tester.testIntroSort(almostArr);

        std::cout << size << "," << randomClassic << "," << randomIntro << ","<< reverseClassic << "," << reverseIntro << ","<< almostClassic << "," << almostIntro << "\n";
    }
    
    return 0;
}