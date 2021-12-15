#include <iostream>
#include <numeric>
#include <string>
#include <variant>
#include <vector>


template <typename ... F>
class overloaded : private F...
{
public:
    overloaded(F... f) : F(f)... {}
    using F::operator()...;
};


template<typename... T>
struct Tree: std::variant<T..., std::vector<Tree<T...>>>
{
    using base = typename Tree::variant;

    template<typename V>
    Tree(V&& v): std::variant<T..., std::vector<Tree<T...>>>{std::forward<V>(v)} 
    {
    }
};


template <typename... T, typename TInit, typename FBranch, typename FLeaf>
auto reduce_tree(const Tree<T...>& t, TInit i, FBranch r_op, FLeaf t_op)
{
    return std::visit(overloaded
    {
        [&] (const Tree<T...>& leaf) 
        {
            return std::visit(t_op, static_cast<Tree<T...>::base>(leaf));
        },
        [&] (const std::vector<Tree<T...>>& branch) 
        {
            return std::transform_reduce(std::begin(branch), std::end(branch), i, r_op, [&] (const auto& b) 
            { 
                return reduce_tree(b, i, r_op, t_op); 
            });
        }
    }, static_cast<Tree<T...>::base>(t));
}

using Node = Tree<std::vector<char>, std::string>; // Define a Node as either a vector of chars or a string

auto total_length(const Node& t) -> int
{
    return reduce_tree(t, 0, std::plus{}, [](const auto& leaf) -> int { return std::size(leaf); });
}

int main(int, char**) 
{
    using namespace std::string_literals;

    Node node0{"abc"s};
    std::vector<Node> node1{node0, "2"s};
    std::vector<Node> node2{node1, "123"s};
    std::vector<Node> node3{node2, node1};
    
    std::cout << "Total length = " << total_length(node3) << std::endl;

    return 0;
}