#pragma once

#include "graph.hpp"
#include "profiler.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

template<typename _T>
class graph_cli
{
	using vertex_t = vertex<_T>;
	using graph_t = graph<_T>;
	using ptr_to_graph_t = std::shared_ptr<graph_t>;

private:
	[[nodiscard]]
	auto get_vertex() const;

	[[nodiscard]]
	auto get_index() const;

	[[nodiscard]]
	auto get_adj() const;

	[[nodiscard]]
	auto get_type() const;

public:
	graph_cli(ptr_to_graph_t graph, std::filesystem::path path);
	~graph_cli() noexcept;

	void add_vertex();
	void add_edge();
	void remove_vertex();
	void find_way();
	void print();
	void gen();
	void profile();

	void save();

private:
	ptr_to_graph_t m_graph;
	std::fstream m_fstream;
};

template<typename _T>
inline auto graph_cli<_T>::get_vertex() const
{
	vertex_t v;
	std::cout << "X\n> ";
	std::cin >> v.x;

	std::cout << "Y\n> ";
	std::cin >> v.y;

	return v;
}

template<typename _T>
inline auto graph_cli<_T>::get_index() const
{
	std::cout << "Index\n> ";
	std::size_t idx{};
	std::cin >> idx;

	return idx;
}

template<typename _T>
inline auto graph_cli<_T>::get_adj() const
{
	std::cout << "> ";

	std::string line;
	std::cin >> std::ws;
	std::getline(std::cin, line);

	std::stringstream sstr(line);
	std::set<std::size_t> adj(std::istream_iterator<std::size_t>(sstr), {});

	return adj;
}

template<typename _T>
inline auto graph_cli<_T>::get_type() const
{
	std::cout << "Type(1 - exit, 0 - autodetect)\n> ";
	std::size_t type{};
	std::cin >> type;

	return type;
}

template<typename _T>
inline graph_cli<_T>::graph_cli(ptr_to_graph_t graph, std::filesystem::path path) :
	m_graph(graph),
	m_fstream(path)
{
	if (!std::filesystem::exists(path))
	{
		std::ofstream tmp(path);
		tmp.close();

		m_fstream.open(path);
		if (!m_fstream.is_open())
		{
			std::cerr << "[ERROR] Cannot create/open file with graph" << std::endl;

			std::exit(1);
		}
	}
	else
		m_fstream >> *m_graph;
}

template<typename _T>
inline graph_cli<_T>::~graph_cli() noexcept
{
	save();

	m_fstream.close();
}

template<typename _T>
inline void graph_cli<_T>::add_vertex()
{
	auto index = get_index();
	auto vertex = get_vertex();
	auto adj = get_adj();
	auto type = get_type();

	m_graph->add_vertex(index, { type == 1 ? node<_T>::type::exit : node<_T>::type::deadlock, vertex, adj });

	save();
}

template<typename _T>
inline void graph_cli<_T>::add_edge()
{
	m_graph->add_edge(get_index(), get_index());

	save();
}

template<typename _T>
inline void graph_cli<_T>::remove_vertex()
{
	m_graph->remove_vertex(get_index());

	save();
}

template<typename _T>
inline void graph_cli<_T>::find_way()
{
	m_graph->find_way(get_index());
}

template<typename _T>
inline void graph_cli<_T>::print()
{
	std::cout << *m_graph;
}

template<typename _T>
inline void graph_cli<_T>::gen()
{
	m_graph->clear();

	auto gen = std::mt19937(std::random_device()());

	using uid_t = std::uniform_int_distribution<>;

	constexpr auto N = 5;
	auto n_dis = uid_t{ 1, N };
	auto vertex_num = static_cast<std::size_t>(n_dis(gen));
	
	for (auto i{ 1LLU }; i <= vertex_num; ++i)
	{
		auto adj_num = n_dis(gen);
		std::set<std::size_t> adj;
		for (std::size_t j{}; j < adj_num; ++j)
			adj.insert(n_dis(gen));

		// TODO: gen doors

		auto v_dis = std::uniform_real_distribution<>{ -100, 100 };
		auto vert = vertex<_T>{ static_cast<_T>(v_dis(gen)), static_cast<_T>(v_dis(gen)) };
		m_graph->add_vertex(i, { i == 1LLU ? node<_T>::type::exit : node<_T>::type::deadlock, vert, adj });
	}

	save();
}

template<typename _T>
inline void graph_cli<_T>::profile()
{
	auto index = get_index();

	::profile([&]() { m_graph->find_way(index); });
}

template<typename _T>
inline void graph_cli<_T>::save()
{
	m_fstream.seekp(std::ios::beg);
	m_fstream << *m_graph;
}
