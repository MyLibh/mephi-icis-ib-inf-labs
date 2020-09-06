#pragma once

#include <iostream>
#include <set>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <queue>
#include <map>

template<typename _T>
struct vertex
{
	_T	x,
		y;
};

template<typename _T>
struct node
{
	using vertex_t = vertex<_T>;

public:
	enum class type : std::size_t
	{
		intersection,
		exit,
		deadlock
	};

public:
	void update_type() noexcept;

public:
	type type;
	vertex_t vertex;
	std::set<std::size_t> adjacent;
};

template<typename _T>
inline void node<_T>::update_type() noexcept
{
	if (type != type::exit)
		type = adjacent.size() == 1 ? type::deadlock : type::intersection;
}

template<typename _T>
class graph
{
	using node_t = node<_T>;
	using vertex_t = typename node_t::vertex_t;
	using matrix_t = std::vector<node_t>;

private:

public:
	void add_vertex(const std::size_t i, const node_t& node);
	void add_edge(const std::size_t i, const std::size_t j);
	void remove_vertex(const std::size_t i);
	void find_way(const std::size_t index);
	void print(std::ostream& ostr = std::cout) const;

	__forceinline void clear() noexcept
	{
		m_graph.clear();
	}

	template<typename _T>
	friend std::ostream& operator<<(std::ostream& ostr, const graph<_T>& graph);

	template<typename _T>
	friend std::istream& operator>>(std::istream& istr, graph<_T>& graph);

private:
	std::unordered_map<std::size_t, node_t> m_graph;
};

template<typename _T>
inline void graph<_T>::add_vertex(const std::size_t i, const node_t& node)
{
	auto&& [it, suc] = m_graph.insert({ i, node });
	if (!suc)
	{
		std::cerr << "[INFO] Cannot insert node, updating it..." << std::endl;

		it->second.vertex = node.vertex;
		it->second.adjacent.insert(std::begin(node.adjacent), std::end(node.adjacent));
	}
	
	it->second.update_type();
	it->second.adjacent.erase(i);

	for (auto&& v : it->second.adjacent)
		if (auto v_it = m_graph.find(v); v_it != end(m_graph))
		{
			v_it->second.adjacent.insert(i);

			v_it->second.update_type();
		}
		else
			m_graph.insert({ v, { node_t::type::deadlock, { 0, 0 }, { i } } });
}

template<typename _T>
inline void graph<_T>::add_edge(const std::size_t i, const std::size_t j)
{
	auto&& i_node = m_graph.at(i);
	i_node.adjacent.insert(j);
	i_node.update_type();

	auto&& j_node = m_graph.at(j);
	j_node.adjacent.insert(i);
	j_node.update_type();
}

template<typename _T>
inline void graph<_T>::remove_vertex(const std::size_t i)
{
	auto&& adj = m_graph.at(i).adjacent;
	for (auto&& index : adj)
	{
		auto&& node = m_graph.at(index);
		node.adjacent.erase(i);

		node.update_type();
	}

	m_graph.erase(i);
}

template<typename _T>
inline void graph<_T>::find_way(const std::size_t index)
{
	std::queue<std::size_t> q;
	q.push(index);

	std::map<std::size_t, bool> used;
	used[index] = true;

	std::map<std::size_t, std::size_t> dist, dir;
	dir[index] = -1;
	while (!q.empty())
	{
		auto cur{ q.front() };
		q.pop();

		for(auto&& adj_v : m_graph.at(cur).adjacent)
			if (!used[adj_v])
			{
				used[adj_v] = true;

				q.push(adj_v);

				auto&& [x1, y1] = m_graph.at(cur).vertex;
				auto&& [x2, y2] = m_graph.at(adj_v).vertex;

				dist[adj_v] = dist[cur] + static_cast<size_t>(std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2)));
				dir[adj_v] = cur;
			}
	}

	if (auto it = std::find_if(std::begin(m_graph), std::end(m_graph), [](auto p) { return p.second.type == node_t::type::exit; }); it == std::end(m_graph))
	{
		std::cerr << "[ERROR] Exit does not exist or inaccessable" << std::endl;
	}
	else
	{
		std::vector<std::size_t> path{ it->first };
		for (std::size_t i = dir[it->first]; i != -1; i = dir[i])
			path.push_back(dir[i]);

		path.back() = index;
		std::copy(std::rbegin(path), std::rend(path), std::ostream_iterator<std::size_t>(std::cout, "->"));
		std::cout << "free" << std::endl;
	}
}

template<typename _T>
inline void graph<_T>::print(std::ostream& ostr /* = std::cout */) const
{
	std::clog << "0 - Intersection\n1 - Exit\n2 - Deadlock\n" << std::endl;

	ostr << m_graph.size() << std::endl;

	for (auto&& [idx, node] : m_graph)
	{
		auto&& [type, vertex, adj] = node;
		ostr << idx << " (" << vertex.x << " " << vertex.y << ") {" << static_cast<std::size_t>(type) << "} [" << adj.size() << "] ";

		std::copy(std::begin(adj), std::end(adj), std::ostream_iterator<std::size_t>(ostr, " "));

		ostr << std::endl;
	}
}

template<typename _T>
inline std::ostream& operator<<(std::ostream& ostr, const graph<_T>& graph)
{
	graph.print(ostr);

	return ostr;
}

template<typename _T>
inline std::istream& operator>>(std::istream& istr, graph<_T>& graph)
{
	std::size_t size{};
	istr >> size;

	using node_t = node<_T>;
	for (size_t i{}; i < size; ++i)
	{
		node_t node;
		char dummy;
		std::size_t idx{},
					adj_num{},
					type{};
		istr >> idx >> dummy >> node.vertex.x >> node.vertex.y >> dummy >> dummy >> type >> dummy >> dummy >> adj_num >> dummy;

		node.type = static_cast<enum node_t::type>(type);

		using istr_it = std::istream_iterator<std::size_t>;
		std::copy_if(istr_it(istr), istr_it(), std::inserter(node.adjacent, std::end(node.adjacent)),
			[count = adj_num](size_t)  mutable { return count--; });

		graph.add_vertex(idx, node);
	}

	return istr;
}