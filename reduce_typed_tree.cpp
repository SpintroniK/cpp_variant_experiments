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
struct Tree : std::variant<nil, std::pair<T, std::vector<Tree<T>>>>
{
    using base = typename Tree::variant;

    template<typename U>
    Tree(U&& v) : std::variant<nil, std::pair<T, std::vector<Tree<T>>>>{std::forward<U>(v)} 
    {
    }
};

template <typename T, typename TInit, typename BinaryReductionOp, typename UnaryTransformOp>
auto reduce_tree(const Tree<T>& t, TInit init, BinaryReductionOp reduce, UnaryTransformOp transform)
{
    return std::visit(overloaded
    {
        [&] (nil) { return init; },
        [&] (const auto& p)
        {
            return reduce(transform(p.first), 
                          std::transform_reduce(std::begin(p.second), std::end(p.second), init, reduce, [&] (const auto& branch) 
                          { 
                            return reduce_tree(branch, init, reduce, transform); 
                          }));
        }
    }, 
    static_cast<Tree<T>::base>(t));
}

// Generic stuff above

struct Data
{
    Data(double d) : value{d} {}
    double value{};
};

using Node = Tree<Data>;

template <typename T>
Node MakeNode(const T& n, const std::vector<Node>& c = { nil{} })
{
    return Node{ std::make_pair(T{n}, c) };
}

 
int main(int, char**) 
{

    using namespace std::string_literals;

    const auto tree = MakeNode(42., 
                                { 
                                    MakeNode(32.5, 
                                    {
                                        MakeNode(125.8)
                                    }), 
                                    MakeNode(12.16, 
                                    { 
                                        MakeNode(8.43) 
                                    }) 
                                });


    const auto r = reduce_tree(tree, double{0}, [](auto i, auto j) { return i + j; }, [] (auto i)  { return i.value; });

    std::cout << "Total = " << r << std::endl;

    return 0;
}