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
        return (!(*v.*dir)->is_leaf() && !(*v.*dir)->weight && node_t::another(v, dir)->weight && !(*(*v.*dir).*dir)->weight);
    }

    // The left (resp. right) child edge (v, u) of v is red, its other child edge is not red, and the right(left) child edge of u is red
    __forceinline bool check_cond3(ptr_node_t v, ptr_node_t node_t::* dir) const noexcept
    {
        return (!(*v.*dir)->is_leaf() && !(*v.*dir)->weight && node_t::another(v, dir)->weight && !node_t::another(*v.*dir, dir)->weight);
    }

    // One of the child edges of v is overweighted and the other is not red
    __forceinline bool check_cond4(ptr_node_t v) const noexcept
    {
        return ((v->left->has_overweight() && !v->right->weight) || (v->right->has_overweight() && !v->left->weight));
    }

    // The left (resp. right) child edge of v is overweighted, its other child edge leading to a node u is red, and the left(right) child edge of u is not red
    __forceinline bool check_cond5(ptr_node_t v, ptr_node_t node_t::* dir) const noexcept
    {
        return (!node_t::another(v, dir)->is_leaf() && (*v.*dir)->has_overweight() && !node_t::another(v, dir)->weight && (*node_t::another(v, dir).*dir)->weight);
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

    bool try_check_and_fix_cond1(ptr_node_t v) noexcept
    {
        w_lock wlock(v->mutex);
        w_lock left_wlock(v->left->mutex);
        w_lock right_wlock(v->right->mutex);
        if (check_cond1(v))
        {
            x_lock xlock(std::move(wlock));
            x_lock left_xlock(std::move(left_wlock));
            x_lock right_xlock(std::move(right_wlock));

            fix_cond1(v);
            
            return true;
        }

        return false;
    }

    bool try_check_and_fix_cond2(ptr_node_t parent, ptr_node_t v) noexcept
    {
        if (v->is_leaf())
            return false;

        w_lock parent_wlock(parent->mutex);
        w_lock wlock(v->mutex);
        w_lock left_wlock(v->left->mutex);
        w_lock right_wlock(v->right->mutex);
        if (check_cond2(v, &node_t::left))
        {
            x_lock p_xlock(std::move(parent_wlock));
            x_lock xlock(std::move(wlock));
            x_lock left_xlock(std::move(left_wlock));
            x_lock right_xlock(std::move(right_wlock));

            fix_cond2(parent, v, &node_t::left);

            return true;
        }
        else if (check_cond2(v, &node_t::right))
        {
            x_lock p_xlock(std::move(parent_wlock));
            x_lock xlock(std::move(wlock));
            x_lock left_xlock(std::move(left_wlock));
            x_lock right_xlock(std::move(right_wlock));

            fix_cond2(parent, v, &node_t::right);

            return true;
        }

        return false;
    }

    bool try_check_and_fix_cond3(ptr_node_t parent, ptr_node_t v) noexcept
    {
        if (v->is_leaf())
            return false;

        w_lock parent_wlock(parent->mutex);
        w_lock wlock(v->mutex);
        w_lock left_wlock(v->left->mutex);
        w_lock right_wlock(v->right->mutex);
        if (check_cond3(v, &node_t::left))
        {
            x_lock p_xlock(std::move(parent_wlock));
            x_lock xlock(std::move(wlock));
            x_lock left_xlock(std::move(left_wlock));
            x_lock right_xlock(std::move(right_wlock));

            fix_cond3(parent, v, &node_t::left);

            return true;
        }
        else if (check_cond3(v, &node_t::right))
        {
            x_lock p_xlock(std::move(parent_wlock));
            x_lock xlock(std::move(wlock));
            x_lock left_xlock(std::move(left_wlock));
            x_lock right_xlock(std::move(right_wlock));

            fix_cond3(parent, v, &node_t::right);

            return true;
        }

        return false;
    }

    bool try_check_and_fix_cond4(ptr_node_t v) noexcept
    {
        w_lock wlock(v->mutex);
        w_lock left_wlock(v->left->mutex);
        w_lock right_wlock(v->right->mutex);
        if (check_cond4(v))
        {
            x_lock xlock(std::move(wlock));
            x_lock left_xlock(std::move(left_wlock));
            x_lock right_xlock(std::move(right_wlock));

            fix_cond4(v);

            return true;
        }

        return false;
    }

    bool try_check_and_fix_cond5(ptr_node_t parent, ptr_node_t v) noexcept
    {
        if (v->is_leaf())
            return false;

        w_lock parent_wlock(parent->mutex);
        w_lock wlock(v->mutex);
        w_lock left_wlock(v->left->mutex);
        w_lock right_wlock(v->right->mutex);
        if (check_cond5(v, &node_t::left))
        {
            x_lock p_xlock(std::move(parent_wlock));
            x_lock xlock(std::move(wlock));
            x_lock left_xlock(std::move(left_wlock));
            x_lock right_xlock(std::move(right_wlock));

            fix_cond5(parent, v, &node_t::left);

            return true;
        }
        else if (check_cond5(v, &node_t::right))
        {
            x_lock p_xlock(std::move(parent_wlock));
            x_lock xlock(std::move(wlock));
            x_lock left_xlock(std::move(left_wlock));
            x_lock right_xlock(std::move(right_wlock));

            fix_cond5(parent, v, &node_t::right);

            return true;
        }

        return false;
    }

    bool try_rebalance_det(key_t&& key) noexcept
    {
        w_lock parent_wlock(m_root->mutex);
        if (m_root->is_leaf())
            return false;

        auto parent = m_root;
        auto ptr = key <= m_root->key ? m_root->left : m_root->right;
        while (!ptr->is_leaf())
        {
            auto& tmp_ptr = key <= ptr->key ? ptr->left : ptr->right;

            while (true)
            {
                if (try_check_and_fix_cond1(ptr))          continue;
                if (try_check_and_fix_cond1(ptr))          continue;
                if (try_check_and_fix_cond2(ptr, tmp_ptr)) continue;
                if (try_check_and_fix_cond3(ptr, tmp_ptr)) continue;
                if (try_check_and_fix_cond4(ptr))          continue;
                if (try_check_and_fix_cond5(ptr, tmp_ptr)) continue;
                
                break;
            }
            
            parent_wlock = std::move(w_lock(ptr->mutex));

            parent = ptr;
            ptr = key <= ptr->key ? ptr->left : ptr->right;
        }

        return true; // TODO: Not exactly. Count violations fixed
    }

    bool try_rebalance_nondet(key_t&& key) noexcept
    {
        w_lock parent_wlock(m_root->mutex);
        if (m_root->is_leaf())
            return false;

        auto parent = m_root;
        auto ptr = key <= m_root->key ? m_root->left : m_root->right;
        while (!ptr->is_leaf())
        {
            auto& tmp_ptr = key <= ptr->key ? ptr->left : ptr->right;

            switch (auto cond = generator::get_int(1, 5); cond)
            {
            case 1: try_check_and_fix_cond1(ptr);          break;
            case 2: try_check_and_fix_cond2(ptr, tmp_ptr); break;
            case 3: try_check_and_fix_cond3(ptr, tmp_ptr); break;
            case 4: try_check_and_fix_cond4(ptr);          break;
            case 5: try_check_and_fix_cond5(ptr, tmp_ptr); break;
            }

            parent_wlock = std::move(w_lock(ptr->mutex));

            parent = ptr;
            ptr = key <= ptr->key ? ptr->left : ptr->right;
        }

        return true; // TODO: Not exactly. Count violations fixed
    }

public:
    cbst();

    void fix_to_key(key_t&& key, const bool det = false) noexcept;

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
inline void cbst<_Kty, _Ty>::fix_to_key(key_t&& key, const bool det /* = false */) noexcept
{
    if (det)
        try_rebalance_det(std::move(key));
    else
        try_rebalance_nondet(std::move(key));
}

template<typename _Kty, typename _Ty>
inline bool cbst<_Kty, _Ty>::insert(key_t&& key, data_t&& data)
{
    w_lock wlock(m_root->mutex);
    if (m_root->is_leaf())
    {
        x_lock xlock(std::move(wlock));

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

        wlock = std::move(w_lock(tmp_ptr->mutex));

        ptr = tmp_ptr;
    }

    if (key == ptr->key)
        return false;

    x_lock xlock(std::move(wlock));

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
    w_lock parent_wlock(m_root->mutex);
    if (m_root->is_leaf())
        return false;

    auto parent = m_root;
    auto ptr = key <= m_root->key ? m_root->left : m_root->right;
    while (!ptr->is_leaf())
    {
        auto& tmp_ptr = key <= ptr->key ? ptr->left : ptr->right;

        parent_wlock = std::move(w_lock(ptr->mutex));

        parent = ptr;
        ptr = tmp_ptr;
    }

    if (key != ptr->key)
        return false;

    auto dir = key <= parent->key ? &node_t::left : &node_t::right;

    x_lock parent_xlock(std::move(parent_wlock));
    x_lock xlock((*parent.*dir)->mutex);

    auto sibling = node_t::another(parent, dir);
    x_lock sibling_xlock(sibling->mutex);

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
    r_lock rlock(m_root->mutex);
    auto ptr = m_root;
    while (!ptr->is_leaf())
    {
        auto& tmp_ptr = key <= ptr->key ? ptr->left : ptr->right;

        rlock = std::move(r_lock(tmp_ptr->mutex));

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