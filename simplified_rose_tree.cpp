#include <iostream>
#include <numeric>
#include <string_view>
#include <variant>
#include <vector>

template <typename ... F>
class overloaded : private F...
{
public:
  overloaded(F... f) : F(f)... {}
  using F::operator()...;
};


template<typename T>
struct rose_tree: std::variant<T, std::vector<rose_tree<T>>> 
{
    using base = typename rose_tree::variant; // See https://stackoverflow.com/a/63617380/3427069

    template<typename U>
    rose_tree(U&& v): std::variant<T, std::vector<rose_tree<T>>>{std::forward<U>(v)} 
    {
    }
};

template <typename T, typename TInit, typename FBranch, typename FLeaf>
auto reduce_tree(const rose_tree<T>& t, TInit i, FBranch r_op, FLeaf t_op)
{
    return std::visit(overloaded
    {
        [&] (const T& leaf) { return t_op(leaf); },
        [&] (const auto& branch) 
        {
            return std::transform_reduce(std::begin(branch), std::end(branch), i, r_op, [&] (const auto& t) { return reduce_tree(t, i, r_op, t_op); });
        }
    }, 
    static_cast<rose_tree<T>::base>(t)); // See SO link above
}

auto total_length(const rose_tree<std::string_view>& t) -> int
{
    return reduce_tree(t, 0, std::plus{}, [](const auto& leaf) -> int { return std::size(leaf); });
}

int main(int, char**) 
{

    rose_tree<std::string_view> node0{"3"};
    std::vector<rose_tree<std::string_view>> node1{node0, "2"};
    std::vector<rose_tree<std::string_view>> node2{"0", "1"};
    std::vector<rose_tree<std::string_view>> node3{node2, node1};
    
    std::cout << "Total length = " << total_length(node3) << std::endl;

    return 0;
}
