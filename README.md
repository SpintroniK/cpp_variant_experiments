# C++ Variant Experiments

Attempts to make a variant visitor for trees.
Below are the different steps that led me to a solution that, I believe, is acceptable.

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

Source code: `shared_ptr_variant_rose_tree.cpp`.

As always, tested with GCC >= 10, and here's the compiler explorer link: [https://godbolt.org/z/a3x5Ph77P](https://godbolt.org/z/a3x5Ph77P).

## Typed Tree

A simple tree with a single type for its nodes.
It turns out, it's not necessary to use `shared_ptr` after all...

Source code: `reduce_typed_tree.cpp`.

As always, tested with GCC >= 10, and here's the compiler explorer link: [https://godbolt.org/z/j1nxxzr7d](https://godbolt.org/z/j1nxxzr7d).

## Transform & Reduce a Rose Tree

This one gets rid of the `shared_ptr` trick, and pretty much achieves what Ben Deane describes at [the end of his CppCon talk](https://www.youtube.com/watch?v=1-IuTLrwpuU). It allows an arbitrary number of types for the nodes.  
The `reduce_tree` function is used to apply a transform-reduce algorithm to the rose tree.

The tree I used in the code is taken from [this article](https://blog.ploeh.dk/2019/08/05/rose-tree-catamorphism/). It shows that the tree can be reduced to a single number, even though its nodes and leaves have two different types: integers and strings, respectively.

Everything is generic until the tree is actually defined as a tree of strings and integers. When the `reduce_tree` function is applied to the tree, an initial value of `0` is passed as the second argument, then, the "reduce" function is a lambda that returns the sum of two elements.
The transform function, on the other hand, uses the `overloaded` idiom to handle the two nodes types separately: if the node is a string, its length is returned, if it's an integer, that same integer is returned.

Source code: `reduce_rose_tree.cpp`.

As always, the code has been tested with GCC >= 10.
And here's the compiler explorer link: [https://godbolt.org/z/1qhjWx33P](https://godbolt.org/z/1qhjWx33P).
