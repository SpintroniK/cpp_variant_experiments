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


template <typename... T>
struct Tree : std::variant<nil, std::pair<std::shared_ptr<std::variant<T...>>, std::vector<Tree<T...>>>>
{
    using base = typename Tree::variant;

    template<typename U>
    Tree(U&& v) : std::variant<nil, std::pair<std::shared_ptr<std::variant<T...>>, std::vector<Tree<T...>>>>{std::forward<U>(v)} 
    {
    }
};

template <typename... T, typename TInit, typename FReduce, typename FTransform>
auto transform_reduce(const Tree<T...>& t, TInit init, FReduce r_op, FTransform t_op)
{
    return std::visit(overloaded
    {
        [&] (nil) { return init; },
        [&] (const auto& p)
        {
            return r_op( 
                            std::visit(t_op , *(p.first)), 
                            std::accumulate(p.second.begin(), p.second.end(), init, [&] (const auto& a, const auto& b)
                            {
                                return r_op(a, transform_reduce(b, init, r_op, t_op));
                            }) // Still using good old accumulate, but perhaps should use transform_reduce now...
                        );
        }
    }, 
    static_cast<Tree<T...>::base>(t));
}

using Node = Tree<std::string, int>;
using VariantType = std::variant<std::string, int>;

template <typename T>
Node MakeNode(const T& n, const std::vector<Node>& c = { nil{} })
{
    return Node{ std::make_pair(std::make_shared<VariantType>(n), c) };
}

 
int main(int, char**) 
{

    using namespace std::string_literals;

    auto a = MakeNode("[Node a]"s);
    auto z = MakeNode("[Node z]"s);
    auto b = MakeNode(6, { z });
    auto c = MakeNode("[Node c]"s, { b, a });


    const auto r = transform_reduce(c, 0, 

                                    [](auto i, auto j) { return i + j; },       // Reduce function
                                    [] <typename T> (const T& v) -> int         // Transform function
                                    {
                                        if constexpr(std::is_same_v<T, std::string>)
                                        {
                                            return std::size(v); 
                                        }
                                        else
                                        {
                                            return v;
                                        }
                                    });

    std::cout << "Total = " << r << std::endl;

    return 0;
}