#include <bits/stdc++.h>
using namespace std;

// Comparator structure for sorting in ascending order
struct CompareAscending {
    template <typename T>
    static bool compare(const T &x, const T &y) {
        return x < y;
    }
};

// Function to swap two elements in the array
template <typename T>
void swapElements(T arr[], int index1, int index2) {
    T temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
}

// Function to choose the pivot as the middle element
template <typename T>
inline int choosePivot(T arr[], int start, int end) {
    return (start + end) / 2;
}

// Function to partition the array around the pivot for QuickSort
template <typename T, typename Comparator>
int partitionArray(T arr[], int left, int right, T &pivot) {
    do {
        while (Comparator::compare(arr[++left], pivot)); // Move left until an element >= pivot
        while ((left < right) && Comparator::compare(pivot, arr[--right])); // Move right until an element <= pivot
        if (left < right) swapElements(arr, left, right); // Swap if out of place
    } while (left < right);
    return left; // Return partition index
}

// Basic QuickSort implementation using the middle element as pivot
template <typename T, typename Comparator>
void basicQuickSort(T arr[], int start, int end) {
    while (start < end) {
        int pivotIndex = choosePivot(arr, start, end); // Choose pivot
        swapElements(arr, pivotIndex, end); // Place pivot at the end
        int partitionIndex = partitionArray<T, Comparator>(arr, start - 1, end + 1, arr[end]); // Partition around pivot
        swapElements(arr, partitionIndex, end); // Move pivot to final position

        // Recursive QuickSort on smaller subarray to optimize stack depth
        if (partitionIndex - start < end - partitionIndex) {
            basicQuickSort<T, Comparator>(arr, start, partitionIndex - 1); // Sort left partition
            start = partitionIndex + 1; // Update start to focus on the right partition
        } else {
            basicQuickSort<T, Comparator>(arr, partitionIndex + 1, end); // Sort right partition
            end = partitionIndex - 1; // Update end to focus on the left partition
        }
    }
}

// Optimized pivot selection: median of three elements (first, middle, last)
template <typename T>
int findMedianPivot(T arr[], int start, int end) {
    int mid = (start + end) / 2;
    if (arr[start] > arr[mid]) swapElements(arr, start, mid);
    if (arr[start] > arr[end]) swapElements(arr, start, end);
    if (arr[mid] > arr[end]) swapElements(arr, mid, end);
    return mid; // Return the median element index
}

// QuickSort using median-of-three pivot selection for improved balance
template <typename T, typename Comparator>
void quickSortMedianPivot(T arr[], int start, int end) {
    while (start < end) {
        int pivotIndex = findMedianPivot(arr, start, end); // Choose median of three as pivot
        swapElements(arr, pivotIndex, end); // Place pivot at the end
        int partitionIndex = partitionArray<T, Comparator>(arr, start - 1, end + 1, arr[end]); // Partition around pivot
        swapElements(arr, partitionIndex, end); // Move pivot to final position

        // Recursive QuickSort on smaller subarray
        if (partitionIndex - start < end - partitionIndex) {
            quickSortMedianPivot<T, Comparator>(arr, start, partitionIndex - 1); // Sort left partition
            start = partitionIndex + 1; // Update start for right partition
        } else {
            quickSortMedianPivot<T, Comparator>(arr, partitionIndex + 1, end); // Sort right partition
            end = partitionIndex - 1; // Update end for left partition
        }
    }
}

// Insertion Sort for small partitions in hybrid QuickSort
template <typename T>
void insertionSortArray(T arr[], int start, int end) {
    for (int i = start + 1; i <= end; ++i) {
        T key = arr[i];
        int j = i - 1;
        while (j >= start && arr[j] > key) { // Shift elements to the right to insert the key
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key; // Insert key at the correct position
    }
}

// Threshold for switching to Insertion Sort in hybrid QuickSort
const int SORT_THRESHOLD = 30;

// Hybrid QuickSort that switches to Insertion Sort for small partitions
template <typename T, typename Comparator>
void hybridQuickSort(T arr[], int start, int end) {
    while (start < end) {
        // Use Insertion Sort for small subarrays
        if (end - start + 1 <= SORT_THRESHOLD) {
            insertionSortArray(arr, start, end);
            return;
        }
        int pivotIndex = findMedianPivot(arr, start, end); // Choose median of three as pivot
        swapElements(arr, pivotIndex, end); // Place pivot at the end
        int partitionIndex = partitionArray<T, Comparator>(arr, start - 1, end + 1, arr[end]); // Partition around pivot
        swapElements(arr, partitionIndex, end); // Move pivot to final position

        // Recursive QuickSort on smaller subarray
        if (partitionIndex - start < end - partitionIndex) {
            hybridQuickSort<T, Comparator>(arr, start, partitionIndex - 1); // Sort left partition
            start = partitionIndex + 1; // Update start for right partition
        } else {
            hybridQuickSort<T, Comparator>(arr, partitionIndex + 1, end); // Sort right partition
            end = partitionIndex - 1; // Update end for left partition
        }
    }
}

// Non-recursive QuickSort using stack to manage subarrays
template <typename T, typename Comparator>
void nonRecursiveQuickSort(T arr[], int size) {
    stack<pair<int, int>> stackRange;
    stackRange.push(make_pair(0, size - 1)); // Initialize stack with the entire array range

    while (!stackRange.empty()) {
        int start = stackRange.top().first, end = stackRange.top().second;
        stackRange.pop();

        // Use Insertion Sort for small subarrays
        if (end - start + 1 <= SORT_THRESHOLD) {
            insertionSortArray(arr, start, end);
            continue;
        }

        int pivotIndex = findMedianPivot(arr, start, end); // Choose median of three as pivot
        swapElements(arr, pivotIndex, end); // Place pivot at the end
        int partitionIndex = partitionArray<T, Comparator>(arr, start - 1, end + 1, arr[end]); // Partition around pivot
        swapElements(arr, partitionIndex, end); // Move pivot to final position

        // Push larger partition to stack to limit stack depth
        if (partitionIndex - start < end - partitionIndex) {
            if (partitionIndex + 1 < end) stackRange.push(make_pair(partitionIndex + 1, end)); // Push right partition
            end = partitionIndex - 1; // Focus on left partition
        } else {
            if (partitionIndex - 1 > start) stackRange.push(make_pair(start, partitionIndex - 1)); // Push left partition
            start = partitionIndex + 1; // Focus on right partition
        }
    }
}

// Function to measure execution time of recursive QuickSort variations
void measureExecutionTime(void (*sortFunc)(int*, int, int), int arr[], int size, const string &desc) {
    int* tempArr = new int[size];
    copy(arr, arr + size, tempArr); // Copy array to avoid sorting original

    auto start = chrono::high_resolution_clock::now();
    sortFunc(tempArr, 0, size - 1); // Call sorting function
    auto end = chrono::high_resolution_clock::now();
    cout << desc << ": " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    delete[] tempArr; // Clean up temporary array
}

// Function to measure execution time of non-recursive QuickSort variation
void measureExecutionTimeNonRecursive(void (*sortFunc)(int*, int), int arr[], int size, const string &desc) {
    int* tempArr = new int[size];
    copy(arr, arr + size, tempArr); // Copy array to avoid sorting original

    auto start = chrono::high_resolution_clock::now();
    sortFunc(tempArr, size); // Call sorting function
    auto end = chrono::high_resolution_clock::now();
    cout << desc << ": " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    delete[] tempArr; // Clean up temporary array
}

// Main function to generate data and measure performance of sorting algorithms
int main() {
    int size;
    cout << "Enter the number of elements (size >= 1000): ";
    cin >> size;

    if (size < 1000) {
        cout << "Size must be >= 1000.\n";
        return 1;
    }

    int* data = new int[size];
    srand(time(0));
    for (int i = 0; i < size; i++) {
        data[i] = rand() % size; // Generate random data
    }

    cout << "Sorting results:\n";
    measureExecutionTime(basicQuickSort<int, CompareAscending>, data, size, "Original ");
    measureExecutionTime(quickSortMedianPivot<int, CompareAscending>, data, size, "(a)");
    measureExecutionTime(hybridQuickSort<int, CompareAscending>, data, size, "(a) + (b)");
    measureExecutionTimeNonRecursive(nonRecursiveQuickSort<int, CompareAscending>, data, size, "(a) + (b) + (c)");

    delete[] data; // Clean up original data array
    return 0;
}
