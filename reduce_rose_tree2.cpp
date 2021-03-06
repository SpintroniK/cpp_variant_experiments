#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <variant>
#include <vector>


template <typename ... F>
class overloaded : private F...
{
public:
    overloaded(F... f) : F(f)... {}
    using F::operator()...;
};

template <typename... T>
struct Tree : std::pair<std::variant<T...>, std::vector<Tree<T...>>>
{
};

template <typename... T, typename TInit, typename FReduce, typename FTransform>
TInit reduce_tree(const Tree<T...>& t, TInit init, FReduce r_op, FTransform t_op)
{
    return r_op(std::visit(t_op , t.first), 
                std::transform_reduce(std::begin(t.second), std::end(t.second), init, r_op, [&] (const auto& b) 
                { 
                    return reduce_tree(b, init, r_op, t_op); 
                }));
}

using Node = Tree<std::string, int>;

template <typename T>
Node MakeNode(const T& n, const std::vector<Node>& c = {})
{
    return Node{ std::make_pair(n, c) };
}

 
int main(int, char**) 
{

    using namespace std::string_literals;

    // This is where I found that tree: https://blog.ploeh.dk/2019/08/05/rose-tree-catamorphism/
    const auto tree = MakeNode(42, 
    { 
        MakeNode(1337, { MakeNode("foo"s), MakeNode("bar"s) }),
        MakeNode(2112, 
        { 
            MakeNode(90125, { MakeNode("baz"s), MakeNode("qux"s), MakeNode("quux"s) }),
            MakeNode("quuz"s)
        }),
        MakeNode("corge"s)
    });


    const auto r = reduce_tree(tree, 0, 
                               [](auto i, auto j) { return i + j; },    // Reduce function
                               overloaded                               // Transform "overloaded" function
                               {
                                    [] (const std::string& v) -> int { return std::size(v); },  // If the node is the string => return its length
                                    [] (auto i) { return i; }                                   // If the node is an int => return its value
                               });

    std::cout << "Total = " << r << std::endl;

    return 0;
}