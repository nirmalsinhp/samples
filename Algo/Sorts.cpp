#include <bits/stdc++.h>

using namespace std;

#define RDEBUG 0
template <typename T>
void bubble_sort(vector<T>& vec)
{
    unsigned long long SW = 0;
    auto n = vec.size();
    bool swaped = true;
    while (swaped)
    {
        swaped = false;
        for (size_t i = 1; i < n; ++i)
        {
            if (vec[i] < vec[i - 1])
            {
                swap(vec[i], vec[i - 1]);
                swaped = true;
                SW++;
            }
        }
    }
    cout << "Bubble Swaps : " << SW << endl;
}

// O(n2), n2/2, comparison, n exchanges.
//  in  every iteration i, find min in remaining entry and exchange with a[i].
//  everything left of i is in correct position.
template <typename T>
void selection_sort(vector<T>& vec)
{
    auto size = vec.size();
    for (auto i = 0; i < size; ++i)
    { // L --> R
        auto min = i;
        for (int j = i + 1; j < size; ++j)
        { // L --> R
            if (vec[j] < vec[min])
                min = j;
        }
        swap(vec[min], vec[i]);
    }
}
// shell sort is a generalization of insertion sort.
// insertion sort is shell sort with gap = 1.
template <typename T>
void mselection_sort(vector<T>& vec)
{
    for (auto itr = vec.begin(); itr != vec.end(); ++itr)
    {
        auto mitr = min_element(itr, vec.end());
        swap(*itr, *mitr);
    }
}

// best case n(already sorted), worst case  O(n2) (reverse sorted)
// in interation i, swap a[i] with each largest entry to it's left.
// entries on left are sorted, right is unseen.
template <typename T>
void insertion_sort(vector<T>& vec)
{
    long long S = 0;
    auto size = vec.size();
    for (size_t i = 0; i < size; ++i)
    { // L --> R
        for (size_t j = i; j > 0; --j)
        { // R --> L || L <-- R
            if (vec[j] < vec[j - 1])
            {
                S++;
                swap(vec[j], vec[j - 1]);
            }
            else
                break; // everything in left is sorted,
        }
    }
    cout << S << " Swaps" << endl;
}

template <typename T>
void shell_sort(vector<T>& vec) {
    auto n = vec.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            T temp = vec[i];
            int j;
            for (j = i; j >= gap && vec[j - gap] > temp; j -= gap) {
                vec[j] = temp;
            }
            vec[j] = temp;
        }
    }
}

void merge(vector<int>& arr, vector<int>& aux, int lo, int mid, int hi)
{
    // assert(is_sorted(begin(arr) + lo, begin(arr) + mid ));
    // assert(is_sorted(begin(arr) + mid + 1, begin(arr) + hi ));

    //    cout << " lo : " << lo << " mid :" << mid <<" hi : " << hi << endl;
    for (int k = lo; k <= hi; ++k)
        aux[k] = arr[k];

    int i = lo;
    int j = mid + 1;
    for (int k = lo; k <= hi; ++k)
    {
        // k <= h, because lo-hi, is closed range.

        if (i > mid)
            arr[k] = aux[j++];
        else if (j > hi)
            arr[k] = aux[i++];
        else if (aux[i] < aux[j])
            arr[k] = aux[i++];
        else
            arr[k] = aux[j++];
    }
}

void merge_sort(vector<int>& arr, vector<int>& aux, int lo, int hi)
{
    if (hi <= lo)
        return;
    auto mid = lo + (hi - lo) / 2;
    merge_sort(arr, aux, lo, mid);
    merge_sort(arr, aux, mid + 1, hi);
    // if(arr[mid] < arr[mid + 1])
    //     return;
    merge(arr, aux, lo, mid, hi);
}

void merge_sort(vector<int>& arr)
{
    vector<int> aux(arr.size());
    merge_sort(arr, aux, 0, arr.size() - 1);
    return;
}

vector<int> merge_sort_c(vector<int> arr)
{
    if (arr.size() < 2)
        return arr;
    auto hi = arr.size();
    auto lo = 0;
    auto mid = lo + (hi - lo) / 2;
    vector<int> left(arr.begin() + lo, arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.begin() + hi);
    vector<int> res;
    auto lres = merge_sort_c(left);
    auto rres = merge_sort_c(right);
    merge(lres.begin(), lres.end(), rres.begin(), rres.end(), back_inserter(res));
    return res;
}

// also check 3 way partitioning, where if val == pivot, just increment, no swaps.
// quick - select -- run partition multiple times, till you get the element k, nth element.
// dutch flag problem
int partition(vector<int>& arr, int lo, int hi)
{
    int pi = arr[hi];
    int first = lo;

    for (int i = lo; i < hi; ++i)
    {
        if (arr[i] < pi)
        {
            swap(arr[i], arr[first]);
            first++;
        }
    }
    swap(arr[hi], arr[first]);
    return first;
}

void qsort(vector<int>& arr, int lo, int hi)
{
    if (lo < hi)
    {
        auto pivot = partition(arr, lo, hi);
        qsort(arr, lo, pivot - 1);
        qsort(arr, pivot + 1, hi);
    }
}

void quick_sort(vector<int>& arr)
{
    auto lo = 0;
    auto hi = arr.size() - 1;
    qsort(arr, lo, hi);
}

void radix_sort(vector<int>& arr)
{
    const int radix = 10;
    int me = *max_element(arr.begin(), arr.end());
    int mlen = to_string(me).size();

    int exp = 10;
    for (int i = 0; i < mlen; ++i)
    {
        int place = pow(10, i);
        vector<vector<int>> buckets(radix);
        for (auto a : arr)
        {
            int d = (a / place) % 10;
            buckets[d].push_back(a);
        }
        vector<int> aux;
        for (auto& b : buckets)
        {
            copy(b.begin(), b.end(), back_inserter(aux));
        }
        #ifdef RDEBUG
            for(auto a : aux)
                cout << a << " ";
            cout << endl;
        #endif
        arr = aux;
    }
}

template <typename T>
void heap_sort(vector<T>& vec)
{
    priority_queue<T> pq;
    for (const auto& v : vec)
        pq.push(v);
    for (auto& v : vec)
    {
        v = pq.top();
        pq.pop();
    }
}

int main()
{

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(1, 10000000);

    auto gen = [&dist, &rng]()
        {
            return dist(rng);
        };
    vector<int> v(100000);
    generate(v.begin(), v.end(), gen);
    /*
        auto tb1 = chrono::high_resolution_clock::now();
        bubble_sort(v);
        auto tb2 = chrono::high_resolution_clock::now();
        auto db1 = chrono::duration_cast<chrono::microseconds>(tb2 - tb1);
        cout << "Bubble " << db1.count() << " MicroSecond(s)" << endl;
        cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;;
    */

    // shuffle(v.begin(), v.end(), default_random_engine());
    /*    auto t1 = chrono::high_resolution_clock::now();
        insertion_sort(v);
        auto t2 = chrono::high_resolution_clock::now();
        auto d1 = chrono::duration_cast<chrono::microseconds>(t2 - t1);
        cout << "Insertion " << d1.count() << " MicroSecond(s)" << endl;
        cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;;

        auto t11 = chrono::high_resolution_clock::now();
        insertion_sort(v);
        auto t22 = chrono::high_resolution_clock::now();
        auto d11 = chrono::duration_cast<chrono::microseconds>(t22 - t11);
        cout << "Already Sorted Insertion " << d11.count() << " MicroSeconds(s)" << endl;
        cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;;

        reverse(v.begin(), v.end());
        auto tir1 = chrono::high_resolution_clock::now();
        insertion_sort(v);
        auto tir2 = chrono::high_resolution_clock::now();
        auto dir1 = chrono::duration_cast<chrono::microseconds>(tir2 - tir1);
        cout << "reverse Sorted Insertion " << dir1.count() << " MicroSeconds(s)" << endl;
        cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;;
    */
    /*
        shuffle(v.begin(), v.end(), default_random_engine());
        auto t3 = chrono::high_resolution_clock::now();
        selection_sort(v);
        auto t4 = chrono::high_resolution_clock::now();
        auto d2 = chrono::duration_cast<chrono::microseconds> (t4 - t3);
        cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;;
        cout << "Selection " << d2.count() << " MicroSeconds(s)" << endl;

        shuffle(v.begin(), v.end(), default_random_engine());
        auto mt3 = chrono::high_resolution_clock::now();
        mselection_sort(v);
        auto mt4 = chrono::high_resolution_clock::now();
        auto md2 = chrono::duration_cast<chrono::microseconds> (mt4 - mt3);
        cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;;
        cout << "MSelection " << md2.count() << " MicroSeconds(s)" << endl;
    */
    /*
        shuffle(v.begin(), v.end(), default_random_engine());
        auto t5 = chrono::high_resolution_clock::now();
        merge_sort(v);
        auto t6 = chrono::high_resolution_clock::now();
        auto d3 = chrono::duration_cast<chrono::microseconds> (t6 - t5);
        cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;;
        cout << "merge " << d3.count() << " MicroSeconds(s)" << endl;

        shuffle(v.begin(), v.end(), default_random_engine());
        vector<int> arr = v;
        auto tm5 = chrono::high_resolution_clock::now();
        auto sarr = merge_sort_c(arr);
        auto tm6 = chrono::high_resolution_clock::now();
        auto dm3 = chrono::duration_cast<chrono::microseconds> (tm6 - tm5);
        cout << boolalpha << is_sorted(sarr.begin(), sarr.end()) << endl;;
        cout << "merge " << dm3.count() << " MicroSeconds(s)" << endl;
    */
/*
    shuffle(v.begin(), v.end(), default_random_engine());
    auto t7 = chrono::high_resolution_clock::now();
    quick_sort(v);
    auto t8 = chrono::high_resolution_clock::now();
    auto d4 = chrono::duration_cast<chrono::microseconds>(t8 - t7);
    cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;
    cout << "Quick " << d4.count() << " MicroSeconds(s)" << endl;
*/

    vector<int> arr1 {2,33,23,45,1234,121,2424,11,32,43343,3};
    shuffle(v.begin(), v.end(), default_random_engine());
    auto t11 = chrono::high_resolution_clock::now();
    radix_sort(v);
    auto t12 = chrono::high_resolution_clock::now();
    auto d6 = chrono::duration_cast<chrono::microseconds>(t12 - t11);
    cout << boolalpha << is_sorted(v.begin(), v.end()) << endl;
    cout << "Radix " << d6.count() << " MicroSeconds(s)" << endl;

    /*
        shuffle(v.begin(), v.end(), default_random_engine());
        auto t9 = chrono::high_resolution_clock::now();
        heap_sort(v);
        auto t10 = chrono::high_resolution_clock::now();
        auto d5 = chrono::duration_cast<chrono::microseconds> (t10 - t9);
        cout << boolalpha << is_sorted(v.rbegin(), v.rend()) << endl; // used reverse iterators as priority_queue is max_heap.
        cout << "Heap " << d5.count() << " MicroSeconds(s)" << endl;
    */
    return 0;
}