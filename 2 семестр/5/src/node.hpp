#pragma once

#include <memory>
#include <iomanip>
#include <shared_mutex>

// TODO: This class should be moved into cbst

template<typename _Kty, typename _Ty>
class node
{
public:
    using key_t = _Kty;
    using data_t = _Ty;
    using ptr_node_t = std::shared_ptr<node<key_t, data_t>>;

public:
    [[nodiscard]]
    __forceinline static ptr_node_t& another(ptr_node_t v, ptr_node_t node<key_t, data_t>::* dir) noexcept
    {
        return (dir == &node<key_t, data_t>::left ? v->right : v->left);
    }

public:
    node(key_t key = std::numeric_limits<key_t>::max(), data_t data = data_t{}, const int weight = 1, ptr_node_t left = nullptr, ptr_node_t right = nullptr);

    __forceinline bool is_leaf() const noexcept
    {
        return (left == nullptr && right == nullptr);
    }

    __forceinline bool has_overweight() const noexcept
    {
        return (weight > 1);
    }

    void print(std::ostream& ostr = std::cout) const;

    template<typename _Key, typename _T>
    friend std::ostream& operator<<(std::ostream& ostr, const node<_Key, _T>& node);

public:
    key_t key;
    data_t data;
    int weight;
    ptr_node_t left, right;
    std::shared_mutex mutex;
};

template<typename _Kty, typename _Ty>
inline node<_Kty, _Ty>::node(key_t key /* = std::numeric_limits<key_t>::max() */,
                    data_t data /* = data_t{} */,
                    const int weight /* = 1 */,
                    ptr_node_t left /* = nullptr */,
                    ptr_node_t right /* = nullptr */) :
    key(key),
    data(data),
    weight(weight),
    left(left),
    right(right),
    mutex()
{ }

template<typename _Kty, typename _Ty>
inline void node<_Kty, _Ty>::print(std::ostream& ostr) const
{
    if (this->is_leaf())
        ostr << "[" << key << "]{" << data << "}";
    else
        ostr << "[" << key << "] " << (!weight ? "R" : (weight == 1 ? "B" : "O"));
}

template<typename _Key, typename _T>
inline std::ostream& operator<<(std::ostream& ostr, const node<_Key, _T>& node)
{
    node.print(ostr);

    return ostr;
}