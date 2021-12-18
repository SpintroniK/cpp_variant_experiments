# C++ Variant Experiments

Attempts to make a variant visitor for trees.

## Rose Tree - Composable C++

The first experiment is based on Ben Deane's talk at CppCon 2021: 
[Composable C++: Principles and Patterns - Ben Deane - CppCon 2021](https://www.youtube.com/watch?v=1-IuTLrwpuU)

A little had to be used to make it compile with GCC 10.

The C++ file for that experiment is `simplified_rose_tree.cpp`.
Here's the compiler explorer link: [https://godbolt.org/z/e77PK5f6f](https://godbolt.org/z/e77PK5f6f).

## Variant Tree

Based on the Rose Tree code above, the code allows to have many types per node.
It also compiles with GCC 10.

The C++ file for that experiment is `simplified_variant_tree.cpp`.
Compiler explorer link: [https://godbolt.org/z/bv51GvccM](https://godbolt.org/z/bv51GvccM).

## Shared Pointer Rose Tree

This one uses `shared_ptr` to overcome the need for pair to be declared with complete types. It allows only one type of node.
Works with GCC >= 10.
The C++ for that experiment is `shared_ptr_rose_tree.cpp`.
Compiler explorer link: 
[https://godbolt.org/z/hssKGf5Pq](https://godbolt.org/z/hssKGf5Pq).