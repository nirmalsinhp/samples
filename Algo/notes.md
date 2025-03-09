# Algo Notes

## Notes

- Index array is a common and important technique. It allows you to iterate the number array in the sorted order, without changing the position of elements in the number array. Note that the sort compare function needs to compare indexes if values are the same. In C++, we can use stable_sort to do just that internally.

```cpp
    vector<int> id(sz);
    iota(begin(id), end(id), 0);
    stable_sort(begin(id), end(id), [&](int i, int j){ return n[i] < n[j]; });

- convex hull : polygon covering all points,
    - make only counter clockwise tunrs.

- heap using array
    - for index k -> children 2k, 2k + 1 -> parent k / 2. array start at index 1, put dummy in 0.
    - sink & swim

- for largest elements use min-heap, for smallest use max-heap, because top will give you max element for max heap and if you keep popping the elements it gives you mean left, and vice versa.

- prefix sums :
    - prefix sums with other ops
    - prefix sums one way
    - prefix sums left & right
- remember you can prepend or append data when creating results
- line sweep is technique useful for intervals
- line sweep + prefix sum is useful when applying multiple intervals shifts.

- Sorting intervals by their start times makes it easy to merge two intervals that are overlapping.
- finding non overlapping intervals need sorted using end time, like activity selection prob.
- Stacks are effective for managing the ordering of nested sequences, as the order in which we must process the sequences matches the order in which they are popped from the stack.
- We can visualize the binary search algorithm stopping when the left pointer "passes" the right pointer.
- What information do I need from my left and right subtrees to solve the problem for my subtree?
- in DFS, If we need more parameters than the original function signature allows, then we need to introduce a helper function to help us recurse.
### Graph DFS
    - DFS on adjacency list
    - DFS on matrix
        - flood fill
        - connected components
        - boundry DFS