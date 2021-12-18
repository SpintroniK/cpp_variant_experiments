# C++ Variant Experiments

Attempts to make a variant visitor for trees.

## Rose Tree - Composable C++

The first experiment is based on Ben Deane's talk at CppCon 2021: 
[Composable C++: Principles and Patterns - Ben Deane - CppCon 2021](https://www.youtube.com/watch?v=1-IuTLrwpuU)

A little trick had to be used to make it compile with GCC 10.

The C++ file for that experiment is `simplified_rose_tree.cpp`.
Here's the compiler explorer link: [https://godbolt.org/z/e77PK5f6f](https://godbolt.org/z/e77PK5f6f).

## Variant Tree

Based on the Rose Tree code above, the code allows to have many types per node.
It also compiles with GCC 10.

The C++ file for that experiment is `simplified_variant_tree.cpp`.
Compiler explorer link: [https://godbolt.org/z/bv51GvccM](https://godbolt.org/z/bv51GvccM).

## Shared Pointer Rose Tree

This one uses `shared_ptr` to overcome the need for pair to be declared with complete types. It allows only one type of node and works with GCC >= 10.

This is clearly another attempt to make the example provided by [Ben Deane in his Composable C++ talk](https://www.youtube.com/watch?v=1-IuTLrwpuU) work. Although I have to admit the use of `shared_ptr` is probably not the best trick ever...

The C++ code for that experiment is `shared_ptr_rose_tree.cpp`.
Compiler explorer link: [https://godbolt.org/z/hssKGf5Pq](https://godbolt.org/z/hssKGf5Pq).

## Shared Pointer Variant Rose tree

This one is a generalization of the previous one (Shared Pointer Rose Tree). It allows multiple types for its nodes.

The code gets more complicated, but it also gets very generic! It would be great to get rid of those `shared_ptr`, though.

As always, tested with GCC >= 10, and here's the compiler explorer link: [https://godbolt.org/z/a3x5Ph77P](https://godbolt.org/z/a3x5Ph77P).
