#pragma once

#include "chain_spec.hpp"
#include "probe_spec.hpp"

#include <filesystem>

template<typename _Key1, typename _Key2>
class table
{
	using item_t = item<_Key1, _Key2>;

public:
	table(const std::size_t capacity, const std::filesystem::path path = "db.txt");
	~table();

	void load_from_file(std::filesystem::path path, const std::size_t capacity);
	void save_to_file();

	bool add(ptr_to_item_t<_Key1, _Key2> item, std::string_view data);

	bool remove_by_key1(const _Key1 key, const std::size_t index) noexcept;
	bool remove_all_by_key1(const _Key1 key) noexcept;
	bool remove_by_key2(const _Key2 key, const std::size_t index) noexcept;
	bool remove_all_by_key2(const _Key2 key) noexcept;

	[[nodiscard]]
	__forceinline auto find_by_key1(const _Key1 key) const
	{
		return m_chain.get(key);
	}

	[[nodiscard]]
	__forceinline auto find_by_key2(const _Key2 key) const
	{
		return m_probe.get(key);
	}

	void print(std::ostream& ostr = std::cout) const;

private:
	chain_hash_table<_Key1, item_t> m_chain;
	probe_hash_table<_Key2, item_t> m_probe;
	std::fstream					m_fstream;
};

template<typename _Key1, typename _Key2>
inline table<_Key1, _Key2>::table(const std::size_t capacity, const std::filesystem::path path /* = "db.txt" */) :
	m_chain(capacity, &item_t::index1),
	m_probe(capacity, &item_t::index2),
	m_fstream()
{
	load_from_file(path, capacity);
}

template<typename _Key1, typename _Key2>
inline table<_Key1, _Key2>::~table()
{
	m_fstream.seekp(std::ios::beg);

	save_to_file();

	m_fstream.close();
}

template<typename _Key1, typename _Key2>
inline void table<_Key1, _Key2>::load_from_file(std::filesystem::path path, const std::size_t capacity)
{
	bool load{};
	if (load = std::filesystem::exists(path); !load)
	{
		std::clog << "[INFO] Cannot load from file, creating new..." << std::endl;

		std::ofstream tmp(path);
		tmp.close();
	}

	m_fstream.open(path, std::ios::binary | std::ios::in | std::ios::out);
	if (!m_fstream.is_open())
	{
		std::cerr << "[ERROR] Cannot open db file" << std::endl;

		std::exit(1);
	}

	if (load)
	{
		m_fstream.seekg(std::ios::beg);

		std::size_t capacity,
					length;
		m_fstream.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
		m_fstream.read(reinterpret_cast<char*>(&length), sizeof(length));

		m_probe.resize(capacity);
		m_chain.resize(capacity, &item_t::index1);

		item_t item;
		for (size_t i{}; i < length; ++i)
		{
			m_fstream.read(reinterpret_cast<char*>(&item), sizeof(item));

			auto ptr = std::make_shared<item_t>(item);

			m_chain.add(ptr->key1, ptr, false);
			m_probe.add(ptr->key2, ptr, false);
		}
	}
	else
		m_probe.save(m_fstream);
}

template<typename _Key1, typename _Key2>
inline void table<_Key1, _Key2>::save_to_file()
{
	m_probe.save(m_fstream);
}

template<typename _Key1, typename _Key2>
inline bool table<_Key1, _Key2>::add(ptr_to_item_t<_Key1, _Key2> item, std::string_view data)
{
	// m_fstream.seekp(std::ios::end);
	item->offset = m_fstream.tellp();
	item->length = data.length();

	m_fstream.write(data.data(), item->length); // puts data in case even when m_probe is full

	if (!m_probe.add(item->key2, item))
		return false;

	m_chain.add(item->key1, item);

	return true;
}

template<typename _Key1, typename _Key2>
inline bool table<_Key1, _Key2>::remove_by_key1(const _Key1 key, const std::size_t index) noexcept
{
	auto item{ m_chain.get(key, index) };
	if (!item)
		return false;

	return (m_probe.remove(item->key2, item->index2) == m_chain.remove(key, index));
}

template<typename _Key1, typename _Key2>
inline bool table<_Key1, _Key2>::remove_all_by_key1(const _Key1 key) noexcept
{
	auto list{ m_chain.get(key) };
	auto del_num{ 0U };
	for (auto ptr = list.get(); ptr; ptr = ptr->next)
		if(m_probe.remove(ptr->data.key2, ptr->data.index2))
			del_num++;

	return (del_num == m_chain.remove(key));
}

template<typename _Key1, typename _Key2>
inline bool table<_Key1, _Key2>::remove_by_key2(const _Key2 key, const std::size_t index) noexcept
{
	auto item{ m_probe.get(key, index) };
	if (!item)
		return false;

	return (m_chain.remove(item->key1, item->index1) == m_probe.remove(key, index));
}

template<typename _Key1, typename _Key2>
inline bool table<_Key1, _Key2>::remove_all_by_key2(const _Key2 key) noexcept
{
	auto list{ m_probe.get(key) };
	auto del_num{ 0U };
	for (auto ptr = list.get(); ptr; ptr = ptr->next)
		if(m_chain.remove(ptr->data.key1, ptr->data.index1))
			del_num++;

	return (del_num == m_probe.remove(key));
}

template<typename _Key1, typename _Key2>
inline void table<_Key1, _Key2>::print(std::ostream& ostr /* = std::cout */) const
{
	ostr << "Separate Chaining(" << m_chain.length() << ")\n" << m_chain << std::endl << std::endl;

	ostr << "Linear Probing(" << m_probe.length() << "/" << m_probe.capacity() << ")\n" << m_probe << std::endl;
}