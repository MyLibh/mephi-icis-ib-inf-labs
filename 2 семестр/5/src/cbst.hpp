#pragma once

#include <memory>
#include <iostream>
#include <queue>

template<typename _Kty, typename _Ty>
class node
{
public:
    using key_t = _Kty;
    using data_t = _Ty;
    using ptr_node_t = std::shared_ptr<node<key_t, data_t>>;

public:
    [[nodiscard]]
    static ptr_node_t& another(ptr_node_t v, ptr_node_t node<key_t, data_t>::* dir) noexcept
    {
        return (dir == &node<key_t, data_t>::left ? v->right : v->left);
    }

public:
    node(key_t key = std::numeric_limits<key_t>::max(), data_t data = data_t{}, const int weight = 1, ptr_node_t left = nullptr, ptr_node_t right = nullptr) :
        key(key),
        data(data),
        weight(weight),
        left(left),
        right(right)
    { }
    
    __forceinline bool is_leaf() const noexcept
    {
        return (left == nullptr && right == nullptr);
    }

    __forceinline bool has_overweight() const noexcept
    {
        return (weight > 1);
    }

    void print(std::ostream& ostr = std::cout) const
    {
        if (this->is_leaf())
            ostr << "[" << key << "]{" << data << "}";
        else
            ostr << "[" << key << "] " << (!weight ? "R" : (weight == 1 ? "B" : "O"));
    }

    template<typename _Key, typename _T>
    friend std::ostream& operator<<(std::ostream& ostr, const node<_Key, _T>& node)
    {
        node.print(ostr);

        return ostr;
    }

public:
    key_t key;
    data_t data;
    int weight;
    ptr_node_t left, right;
};

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
        auto left = &node_t::left;
        auto right = &node_t::right;

        if (check_cond1(v))         fix_cond1(v);
        if (check_cond2(v, left))   fix_cond2(parent, v, left);
        if (check_cond2(v, right))  fix_cond2(parent, v, right);
        if (check_cond3(v, left))   fix_cond3(parent, v, left);
        if (check_cond3(v, right))  fix_cond3(parent, v, right);
        if (check_cond4(v))         fix_cond4(v);
        if (check_cond5(v, left))   fix_cond5(parent, v, left);
        if (check_cond5(v, right))  fix_cond5(parent, v, right);
    }

public:
    cbst() :
        m_root(std::make_shared<node_t>())
    { }

    void fix_to_key(key_t&& key) noexcept
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

    bool insert(key_t&& key, data_t&& data)
    {
        if (m_root->is_leaf())
        {
            auto ptr = m_root;
            m_root = std::make_shared<node_t>(key);

            m_root->left = std::make_shared<node_t>(key, data);
            m_root->right = ptr;

            return true;
        }

        auto parent = m_root;
        auto cur = m_root;
        while (!cur->is_leaf())
        {
            // if (key == cur->key)
            //     return false;

            parent = cur;
            cur = key <= cur->key ? cur->left : cur->right;
        }

        auto leaf = cur;
        
        cur = std::make_shared<node_t>(std::min(key, leaf->key), data_t{}, leaf->weight - 1);
        if (key <= parent->key)
            parent->left = cur;
        else
            parent->right = cur;

        leaf->weight = 1;
        if (key <= leaf->key)
        {
            cur->left = std::make_shared<node_t>(key, data);
            cur->right = leaf;
        }
        else
        {
            cur->left = leaf;
            cur->right = std::make_shared<node_t>(key, data);
        }

        return true;
    }

    bool remove(key_t&& key) noexcept
    {
        if (m_root->is_leaf())
            return false;

        auto grandparent = m_root;
        auto parent = m_root;
        auto cur = m_root;
        while (!cur->is_leaf())
        {
            grandparent = parent;
            parent = cur;
            cur = key <= cur->key ? cur->left : cur->right;
        }

        if (key != cur->key)
            return false;
        
        auto&& u = key > parent->key ? parent->left : parent->right;

        if (key <= grandparent->key)
        {
            grandparent->left = u;

            grandparent->left->weight = parent->weight + u->weight;
        }
        else
        {
            grandparent->right = u;

            grandparent->right->weight = parent->weight + u->weight;
        }

        return true;
    }

    [[nodiscard]]
    std::pair<data_t&, bool> get(key_t&& key) const noexcept
    {
        auto ptr = m_root;
        while (!ptr->is_leaf())
            ptr = key <= ptr->key ? ptr->left : ptr->right;

        return { ptr->data, key == ptr->key };
    }

    void print(std::ostream& ostr = std::cout) const
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
            while(num_nodes--)
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
    friend std::ostream& operator<<(std::ostream& ostr, const cbst<_Key, _T>& cbst)
    {
        cbst.print(ostr);

        return ostr;
    }

private:
    ptr_node_t m_root;
};