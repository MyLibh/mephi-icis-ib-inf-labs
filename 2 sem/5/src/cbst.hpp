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
        return (!v->is_leaf() && !v->left->weight && !v->right->weight && (v == m_root || v->weight));
    }

    // The left (resp. right) child edge (v, u) of v is red, its other child edge is not red, and the left(right) child edge of u is red
    __forceinline bool check_cond2(ptr_node_t v, ptr_node_t node_t::* dir) const noexcept
    {
        return (!v->is_leaf() && !(*v.*dir)->is_leaf() && !(*v.*dir)->weight && node_t::another(v, dir)->weight && !(*(*v.*dir).*dir)->weight);
    }

    // The left (resp. right) child edge (v, u) of v is red, its other child edge is not red, and the right(left) child edge of u is red
    __forceinline bool check_cond3(ptr_node_t v, ptr_node_t node_t::* dir) const noexcept
    {
        return (!v->is_leaf() && !(*v.*dir)->is_leaf() && !(*v.*dir)->weight && node_t::another(v, dir)->weight && !node_t::another(*v.*dir, dir)->weight);
    }

    // One of the child edges of v is overweighted and the other is not red
    __forceinline bool check_cond4(ptr_node_t v) const noexcept
    {
        return (!v->is_leaf() && (v->left->has_overweight() && !v->right->weight) || (v->right->has_overweight() && !v->left->weight));
    }

    // The left (resp. right) child edge of v is overweighted, its other child edge leading to a node u is red, and the left(right) child edge of u is not red
    __forceinline bool check_cond5(ptr_node_t v, ptr_node_t node_t::* dir) const noexcept
    {
        return (!v->is_leaf() && !node_t::another(v, dir)->is_leaf() && (*v.*dir)->has_overweight() && !node_t::another(v, dir)->weight && (*node_t::another(v, dir).*dir)->weight);
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
        auto u = node_t::another(v, dir);

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

        node_t::another(v, dir) = *u.*dir;
        *u.*dir = v;

        fix_cond4(v);
    }

    bool try_check_and_fix_cond1(w_lock<state_mutex>& wlock, ptr_node_t v) noexcept
    {
        if (v->is_leaf())
            return false;
        
        w_lock l_wlock(v->left->mutex);
        w_lock r_wlock(v->right->mutex);
        if (check_cond1(v))
        {
            x_lock xlock(std::move(wlock));
            x_lock l_xlock(std::move(l_wlock));
            x_lock r_xlock(std::move(r_wlock));

            fix_cond1(v);
            
            wlock = std::move(xlock);

            return true;
        }

        return false;
    }

    bool try_check_and_fix_cond2(w_lock<state_mutex>& p_wlock, ptr_node_t parent, ptr_node_t v) noexcept
    {
        if (parent->is_leaf())
            return false;

        { // LL
            w_lock wlock;
            if (v != parent)
            {
                wlock = std::move(w_lock(v->mutex));
                if (v->is_leaf())
                    return false;
            }
            w_lock l_wlock(v->left->mutex);
            if (check_cond2(v, &node_t::left))
            {
                x_lock p_xlock(std::move(p_wlock));
                x_lock xlock(std::move(wlock));
                x_lock l_xlock(std::move(l_wlock));

                fix_cond2(parent, v, &node_t::left);

                p_wlock = std::move(p_xlock);

                return true;
            }
        }

        { // RR
            w_lock wlock;
            if (v != parent)
            {
                wlock = std::move(w_lock(v->mutex));
                if (v->is_leaf())
                    return false;
            }
            w_lock r_wlock(v->right->mutex);
            if (check_cond2(v, &node_t::right))
            {
                x_lock p_xlock(std::move(p_wlock));
                x_lock xlock(std::move(wlock));
                x_lock r_xlock(std::move(r_wlock));

                fix_cond2(parent, v, &node_t::right);

                p_wlock = std::move(p_xlock);

                return true;
            }
        }

        return false;
    }

    bool try_check_and_fix_cond3(w_lock<state_mutex>& p_wlock, ptr_node_t parent, ptr_node_t v) noexcept
    {
        if (parent->is_leaf())
            return false;

        { // LR
            w_lock wlock;
            if (v != parent)
            {
                wlock = std::move(w_lock(v->mutex));
                if (v->is_leaf())
                    return false;
            }
            w_lock l_wlock(v->left->mutex);
            if (v->left->is_leaf())
                return false;
            w_lock lr_wlock(v->left->right->mutex);
            if (check_cond3(v, &node_t::left))
            {
                x_lock p_xlock(std::move(p_wlock));
                x_lock xlock(std::move(wlock));
                x_lock l_xlock(std::move(l_wlock));
                x_lock lr_xlock(std::move(lr_wlock));

                //fix_cond3(parent, v, &node_t::left);

                p_wlock = std::move(p_xlock);

                return true;
            }
        }

        /*{ // RL
            w_lock wlock;
            if (v != parent)
            {
                wlock = std::move(w_lock(v->mutex));
                if (v->is_leaf())
                    return false;
            }
            w_lock r_wlock(v->right->mutex);
            if (v->right->is_leaf())
                return false;
            w_lock rl_wlock(v->right->left->mutex);
            if (check_cond3(v, &node_t::right))
            {
                x_lock p_xlock(std::move(p_wlock));
                x_lock xlock(std::move(wlock));
                x_lock r_xlock(std::move(r_wlock));
                x_lock rl_xlock(std::move(rl_wlock));

                fix_cond3(parent, v, &node_t::right);

                p_wlock = std::move(p_xlock);

                return true;
            }
        }*/

        return false;
    }

    bool try_check_and_fix_cond4(w_lock<state_mutex>& wlock, ptr_node_t v) noexcept
    {
        if (v->is_leaf())
            return false;
     
        w_lock l_wlock(v->left->mutex);
        w_lock r_wlock(v->right->mutex);
        if (check_cond4(v))
        {
            x_lock xlock(std::move(wlock));
            x_lock l_xlock(std::move(l_wlock));
            x_lock r_xlock(std::move(r_wlock));

            fix_cond4(v);

            wlock = std::move(xlock);

            return true;
        }

        return false;
    }

    bool try_check_and_fix_cond5(w_lock<state_mutex>& p_wlock, ptr_node_t parent, ptr_node_t v) noexcept
    {
        if (parent->is_leaf())
            return false;

        {
            w_lock wlock;
            if (v != parent)
            {
                wlock = std::move(w_lock(v->mutex));
                if (v->is_leaf())
                    return false;
            }
            w_lock l_wlock(v->left->mutex);
            w_lock r_wlock(v->right->mutex);
            if (v->right->is_leaf())
                return false;
            w_lock rl_wlock(v->right->left->mutex);
            if (check_cond5(v, &node_t::left))
            {
                x_lock p_xlock(std::move(p_wlock));
                x_lock xlock(std::move(wlock));
                x_lock l_xlock(std::move(l_wlock));
                x_lock r_xlock(std::move(r_wlock));
                x_lock rl_xlock(std::move(rl_wlock));

                fix_cond5(parent, v, &node_t::left);

                p_wlock = std::move(p_xlock);

                return true;
            }
        }

        {
            w_lock wlock;
            if (v != parent)
            {
                wlock = std::move(w_lock(v->mutex));
                if (v->is_leaf())
                    return false;
            }
            w_lock l_wlock(v->left->mutex);
            w_lock r_wlock(v->right->mutex);
            if (v->left->is_leaf())
                return false;
            w_lock lr_wlock(v->left->right->mutex);
            if (check_cond5(v, &node_t::right))
            {
                x_lock p_xlock(std::move(p_wlock));
                x_lock xlock(std::move(wlock));
                x_lock l_xlock(std::move(l_wlock));
                x_lock r_xlock(std::move(r_wlock));
                x_lock lr_xlock(std::move(lr_wlock));

                fix_cond5(parent, v, &node_t::right);

                p_wlock = std::move(p_xlock);

                return true;
            }
        }

        return false;
    }

    std::size_t try_rebalance_det(key_t&& key) noexcept
    {
        w_lock parent_wlock(m_root->mutex);
        if (m_root->is_leaf())
            return { };

        std::size_t viols_num{};

        auto parent = m_root;
        auto ptr = key <= m_root->key ? m_root->left : m_root->right;
        while (!ptr->is_leaf())
        {
            while (true)
            {
                //if (try_check_and_fix_cond1(parent_wlock, parent)) { ++viols_num; continue; } // Do not wanna think about changing ptr/tmp_ptr
                if (try_check_and_fix_cond2(parent_wlock, parent, ptr)) { ++viols_num; continue; }
                //if (try_check_and_fix_cond3(parent_wlock, parent, ptr)) { ++viols_num; continue; }
                //if (try_check_and_fix_cond4(parent_wlock, parent)) { ++viols_num; continue; }
                //if (try_check_and_fix_cond5(parent_wlock, parent, ptr)) { ++viols_num; continue; }

                break;
            }
            
            parent_wlock = std::move(w_lock(ptr->mutex));

            parent = ptr;
            if (!ptr->is_leaf())
                ptr = key <= ptr->key ? ptr->left : ptr->right;
        }

        return viols_num;
    }

    std::size_t try_rebalance_nondet(key_t&& key) noexcept
    {
       /* if (w_lock wlock(m_root->mutex); m_root->is_leaf())
            return { };

        std::size_t viols_num{};
        switch (auto cond = generator::get_int(1, 5); cond)
        {
        case 1: if (try_check_and_fix_cond1(m_root))         ++viols_num; break;
        case 2: if (try_check_and_fix_cond2(m_root, m_root)) ++viols_num; break;
        case 3: if (try_check_and_fix_cond3(m_root, m_root)) ++viols_num; break;
        case 4: if (try_check_and_fix_cond4(m_root))         ++viols_num; break;
        case 5: if (try_check_and_fix_cond5(m_root, m_root)) ++viols_num; break;
        }

        w_lock parent_wlock(m_root->mutex);
        auto parent = m_root;
        auto ptr = key <= m_root->key ? m_root->left : m_root->right;
        while (!ptr->is_leaf())
        {
            auto tmp_ptr = key <= ptr->key ? ptr->left : ptr->right;

            switch (auto cond = generator::get_int(1, 5); cond)
            {
            case 1: if (try_check_and_fix_cond1(ptr))          ++viols_num; break;
            case 2: if (try_check_and_fix_cond2(ptr, tmp_ptr)) ++viols_num; break;
            case 3: if (try_check_and_fix_cond3(ptr, tmp_ptr)) ++viols_num; break;
            case 4: if (try_check_and_fix_cond4(ptr))          ++viols_num; break;
            case 5: if (try_check_and_fix_cond5(ptr, tmp_ptr)) ++viols_num; break;
            }

            parent_wlock = std::move(w_lock(ptr->mutex));

            parent = ptr;
            if(!ptr->is_leaf())
                ptr = key <= ptr->key ? ptr->left : ptr->right;
        }*/

        return 0;//viols_num;
    }

public:
    cbst();

    __forceinline auto fix_to_key(key_t&& key, const bool det = false) noexcept
    {
        return (det ? try_rebalance_det(std::move(key)) : try_rebalance_nondet(std::move(key)));
    }

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
        w_lock tmp_wlock(ptr->mutex);
        if (!ptr->is_leaf())
        {
            parent_wlock = std::move(tmp_wlock);

            parent = ptr;
            ptr = key <= ptr->key ? ptr->left : ptr->right;
        }
    }

    if (key != ptr->key)
        return false;

    ptr = nullptr; // Remove reference
    
    // Do not acquire w_lock for parent childs due to implementation of xlock

    x_lock parent_xlock(std::move(parent_wlock));

    x_lock l_xlock(parent->left->mutex);
    x_lock r_xlock(parent->right->mutex);

    auto dir = key > parent->key ? &node_t::left : &node_t::right; // Sibling
    auto sibling = *parent.*dir;

    parent->key = std::move(sibling->key);
    parent->data = std::move(sibling->data);
    parent->weight += sibling->weight;
    
    l_xlock.unlock(); // To make sure xlocks free mutexes before deleting dat nodes
    r_xlock.unlock();
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