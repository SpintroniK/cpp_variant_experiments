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

struct nil{};


template <typename T>
struct Tree : std::variant<nil, std::pair<std::shared_ptr<T>, std::vector<Tree<T>>>>
{
    using base = typename Tree::variant;
    using value_type = T;

    template<typename U>
    Tree(U&& v) : std::variant<nil, std::pair<std::shared_ptr<T>, std::vector<Tree<T>>>>{std::forward<U>(v)} 
    {
    }
};

template <typename T, typename TInit, typename FReduce, typename FTransform>
auto transform_reduce(const Tree<T>& t, TInit init, FReduce r_op, FTransform t_op)
{
    return std::visit(overloaded
    {
        [&] (nil)
        {
            return init; 
        },
        [&] (const auto& p)
        {
            return r_op(t_op(*(p.first)), std::accumulate(p.second.begin(), p.second.end(), init, [&] (const auto& a, const auto& b)
            {
                return r_op(a, transform_reduce(b, init, r_op, t_op));
            }));
        }
    }, 
    static_cast<Tree<T>::base>(t));
}

template <typename T>
Tree<T> MakeNode(const T& val, const std::vector<Tree<T>>& childs = { nil{} })
{
    return Tree<T>{std::make_pair(std::make_shared<T>(val), childs)};
}
 
int main(int, char**) 
{

    using namespace std::string_literals;

    auto a = MakeNode("[Node a]"s);
    auto z = MakeNode("[Node z]"s);
    auto b = MakeNode("[Node b]"s, { z });
    auto c = MakeNode("[Node c]"s, { b, a });

    using node_type = decltype(c)::value_type;
    const auto r = transform_reduce(c, std::size(node_type{}),
                                    [](auto i, auto j) { return i + j; },       // Reduce function
                                    [](const auto& v) { return std::size(v); } // Transform function
                                    );    

    std::cout << "Total = " << r << std::endl;

    return 0;
}