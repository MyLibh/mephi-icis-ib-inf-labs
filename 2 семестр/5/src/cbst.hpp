#pragma once

#include "node.hpp"
#include "generator.hpp"

#include <queue>

template<typename _Kty, typename _Ty>
class cbst
{
public:
    using node_t = node<_Kty, _Ty>;
    using key_t = typename node_t::key_t;
    using data_t = typename node_t::data_t;
    using ptr_node_t = typename node_t::ptr_node_t;

private:
    // Both of the child edges of v are red, and either v is the root or the parent edge of v has a non - zero weight
    __forceinline bool check_cond1(ptr_node_t v) const noexcept
    {
        return (!v->left->weight && !v->right->weight && (v == m_root || v->weight));
    }

    // The left (resp. right) child edge (v, u) of v is red, its other child edge is not red, and the left(right) child edge of u is red
    __forceinline bool check_cond2(ptr_node_t v, ptr_node_t node_t::* dir) const noexcept
    {
        return (!(*v.*dir)->weight && node_t::another(v, dir)->weight && !(*(*v.*dir).*dir)->weight);
    }

    // The left (resp. right) child edge (v, u) of v is red, its other child edge is not red, and the right(left) child edge of u is red
    __forceinline bool check_cond3(ptr_node_t v, ptr_node_t node_t::* dir) const noexcept
    {
        return (!(*v.*dir)->weight && node_t::another(v, dir)->weight && !node_t::another(*v.*dir, dir)->weight);
    }

    // One of the child edges of v is overweighted and the other is not red
    __forceinline bool check_cond4(ptr_node_t v) const noexcept
    {
        return ((v->left->has_overweight() && !v->right->weight) || (v->right->has_overweight() && !v->left->weight));
    }

    // The left (resp. right) child edge of v is overweighted, its other child edge leading to a node u is red, and the left(right) child edge of u is not red
    __forceinline bool check_cond5(ptr_node_t v, ptr_node_t node_t::* dir) const noexcept
    {
        return ((*v.*dir)->has_overweight() && !node_t::another(v, dir)->weight && (*node_t::another(v, dir).*dir)->weight);
    }

    void fix_cond1(ptr_node_t v) noexcept
    {
        v->left->weight = v->right->weight = 1;
     
        if (v != m_root)
            v->weight--;
    }

    void fix_cond2(ptr_node_t parent, ptr_node_t v, ptr_node_t node_t::* dir) noexcept
    {
        auto u = *v.*dir;

        if (v == m_root)
            m_root = u;
        else
        {
            if (parent->left == v)
                parent->left = u;
            else
                parent->right = u;
        }

        std::swap(u->weight, v->weight);

        *v.*dir = node_t::another(u, dir);
        node_t::another(u, dir) = v;
    }

    void fix_cond3(ptr_node_t parent, ptr_node_t v, ptr_node_t node_t::* dir) noexcept
    {
        auto u = *v.*dir;
        auto t = node_t::another(u, dir);

        if (v == m_root)
            m_root = t;
        else
        {
            if (parent->left == v)
                parent->left = t;
            else
                parent->right = t;
        }

        std::swap(t->weight, v->weight);

        *v.*dir = node_t::another(t, dir);
        node_t::another(u, dir) = *t.*dir;

        node_t::another(t, dir) = v;
        *t.*dir = u;
    }

    void fix_cond4(ptr_node_t v) noexcept
    {
        if (v != m_root)
            v->weight--;

        v->left->weight--;
        v->right->weight--;
    }

    void fix_cond5(ptr_node_t parent, ptr_node_t v, ptr_node_t node_t::* dir) noexcept
    {
        fix_cond2(parent, v, dir);

        fix_cond4(v);
    }

    void try_rebalance(ptr_node_t parent, ptr_node_t v) noexcept
    {
        static const auto&& left = &node_t::left;
        static const auto&& right = &node_t::right;

        switch (auto cond = generator::get_int(1, 5); cond)
        {
        case 1: if (check_cond1(v))       fix_cond1(v);                                                                            break;
        case 2: if (check_cond2(v, left)) fix_cond2(parent, v, left); else if (check_cond2(v, right)) fix_cond2(parent, v, right); break;
        case 3: if (check_cond3(v, left)) fix_cond3(parent, v, left); else if (check_cond3(v, right)) fix_cond3(parent, v, right); break;
        case 4: if (check_cond4(v))       fix_cond4(v);                                                                            break;
        case 5: if (check_cond5(v, left)) fix_cond5(parent, v, left); else if (check_cond5(v, right)) fix_cond5(parent, v, right); break;
        }
    }

public:
    cbst();

    void fix_to_key(key_t&& key) noexcept;

    bool insert(key_t&& key, data_t&& data);

    bool remove(key_t&& key) noexcept;

    [[nodiscard]]
    std::pair<data_t&, bool> get(key_t&& key) const noexcept;

    void print(std::ostream& ostr) const;

    template<typename _Key, typename _T>
    friend std::ostream& operator<<(std::ostream& ostr, const cbst<_Key, _T>& cbst);

private:
    ptr_node_t m_root;
};

template<typename _Kty, typename _Ty>
inline cbst<_Kty, _Ty>::cbst() :
    m_root(std::make_shared<node_t>())
{ }

template<typename _Kty, typename _Ty>
inline void cbst<_Kty, _Ty>::fix_to_key(key_t&& key) noexcept
{
    auto parent = m_root;
    auto ptr = m_root;
    while (!ptr->is_leaf())
    {
        try_rebalance(parent, ptr);

        parent = ptr;
        ptr = key <= ptr->key ? ptr->left : ptr->right;
    }
}

template<typename _Kty, typename _Ty>
inline bool cbst<_Kty, _Ty>::insert(key_t&& key, data_t&& data)
{
    std::shared_lock wlock(m_root->mutex);
    if (m_root->is_leaf())
    {
        wlock.unlock();
        std::lock_guard<std::shared_mutex> xlock(m_root->mutex);

        auto ptr = m_root;
        m_root = std::make_shared<node_t>(key);

        m_root->left = std::make_shared<node_t>(key, data);
        m_root->right = ptr;

        return true;
    }

    auto ptr = m_root;
    while (!ptr->is_leaf())
    {
        auto& tmp_ptr = key <= ptr->key ? ptr->left : ptr->right;

        std::shared_lock tmp_wlock(tmp_ptr->mutex);

        wlock.unlock();
        wlock = std::move(tmp_wlock);

        ptr = tmp_ptr;
    }

    if (key == ptr->key)
        return false;

    wlock.unlock(); // CHECK: rb-tree conditions can break
    std::lock_guard<std::shared_mutex> xlock(ptr->mutex);
    if (!ptr->is_leaf())
        throw std::runtime_error("wrong pointerization occured");

    auto dir = key <= ptr->key ? &node_t::left : &node_t::right;

    *ptr.*dir = std::make_shared<node_t>(key, data);
    node_t::another(ptr, dir) = std::make_shared<node_t>(ptr->key, std::move(ptr->data));

    ptr->key = ptr->left->key;
    ptr->weight--;

    return true;
}

template<typename _Kty, typename _Ty>
inline bool cbst<_Kty, _Ty>::remove(key_t&& key) noexcept
{
    std::shared_lock parent_wlock(m_root->mutex);
    if (m_root->is_leaf())
        return false;

    auto parent = m_root;
    auto ptr = m_root;
    while (!ptr->is_leaf())
    {
        auto& tmp_ptr = key <= ptr->key ? ptr->left : ptr->right;

        std::shared_lock tmp_wlock(tmp_ptr->mutex);

        parent_wlock.unlock();
        parent_wlock = std::move(tmp_wlock);

        parent = ptr;
        ptr = tmp_ptr;
    }

    if (key != ptr->key)
        return false;

    auto dir = key <= parent->key ? &node_t::left : &node_t::right;

    parent_wlock.unlock();

    using lg = std::lock_guard<std::shared_mutex>;
    lg parent_xlock(parent->mutex); // CHECK: rb-tree conditions can break
    lg xlock((*parent.*dir)->mutex);

    auto sibling = node_t::another(parent, dir);
    lg sibling_xlock(sibling->mutex);

    parent->key = std::move(sibling->key);
    parent->data = std::move(sibling->data);
    parent->weight += sibling->weight;
    parent->left = nullptr;
    parent->right = nullptr;

    return true;
}

template<typename _Kty, typename _Ty>
[[nodiscard]]
inline std::pair<typename cbst<_Kty, _Ty>::data_t&, bool> cbst<_Kty, _Ty>::get(key_t&& key) const noexcept
{
    std::shared_lock rlock(m_root->mutex);
    auto ptr = m_root;
    while (!ptr->is_leaf())
    {
        auto& tmp_ptr = key <= ptr->key ? ptr->left : ptr->right;

        std::shared_lock tmp_rlock(tmp_ptr->mutex);

        rlock.unlock();
        rlock = std::move(tmp_rlock);

        ptr = tmp_ptr;
    }

    return { ptr->data, key == ptr->key };
}

template<typename _Kty, typename _Ty>
inline void cbst<_Kty, _Ty>::print(std::ostream& ostr) const
{
    if (!m_root)
    {
        ostr << "empty";

        return;
    }

    std::size_t level{};
    std::queue<ptr_node_t> queue;
    queue.push(m_root);
    while (!queue.empty())
    {
        ostr << level << " ";
        auto num_nodes = queue.size();
        while (num_nodes--)
        {
            auto cur = queue.front();

            ostr << *cur << "    ";

            if (cur->left)
                queue.push(cur->left);

            if (cur->right)
                queue.push(cur->right);

            queue.pop();
        }

        ostr << std::endl;

        ++level;
    }
}

template<typename _Key, typename _T>
inline std::ostream& operator<<(std::ostream& ostr, const cbst<_Key, _T>& cbst)
{
    cbst.print(ostr);

    return ostr;
}