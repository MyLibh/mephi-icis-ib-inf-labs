#include "cbst.hpp"
#include "thread_poll.hpp"

#pragma warning(disable : 26444)
// Avoid unnamed objects with custom construction and destruction.

#include <string>
#include <iostream>

constexpr auto MIN_OPS = 1;
constexpr auto MAX_OPS = 10000;
constexpr auto THREADS_NUM = 4U;
constexpr auto MIN_KEY = 0;
constexpr auto MAX_KEY = 1000;
constexpr auto DATA_LENGTH = 5U;

template<typename _Kty, typename _Ty>
void inserter(cbst<_Kty, _Ty>& cbst, int num_ops, [[maybe_unused]] std::mutex& mutex);

template<typename _Kty, typename _Ty>
void deleter(cbst<_Kty, _Ty>& cbst, int num_ops, [[maybe_unused]] std::mutex& mutex);

template<typename _Kty, typename _Ty>
void reader(cbst<_Kty, _Ty>& cbst, int num_ops, [[maybe_unused]] std::mutex& mutex);

template<typename _Kty, typename _Ty>
void rebalancer(cbst<_Kty, _Ty>& cbst, int num_ops, [[maybe_unused]] std::mutex& mutex, const bool det);

signed main()
{
	try
	{
		cbst<int, std::string> cbst;
		auto num_ops = generator::get_int(MIN_OPS, MAX_OPS);

		thread_pool<std::function<void()>> tp(THREADS_NUM);
		[[maybe_unused]] std::mutex mutex;
		tp.add_job([&, num_ops]() mutable { inserter(cbst, num_ops, mutex); });
		tp.add_job([&, num_ops]() mutable { deleter(cbst, num_ops, mutex); });
		tp.add_job([&, num_ops]() mutable { reader(cbst, num_ops, mutex); });
		tp.add_job([&, num_ops]() mutable { rebalancer(cbst, num_ops, mutex, true); });

		tp.wait_all();
		
		std::cout << "Ops for each thread: " << num_ops << std::endl << cbst << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown exception" << std::endl;
	}

	return 0;
}

template<typename _Kty, typename _Ty>
void inserter(cbst<_Kty, _Ty>& cbst, int num_ops, [[maybe_unused]] std::mutex& mutex)
{
	while (num_ops--)
	{
		auto&& key = generator::get_int(MIN_KEY, MAX_KEY);
		auto&& data = generator::get_str(DATA_LENGTH);
		[[maybe_unused]] bool res = cbst.insert(std::move(key), std::move(data));

#ifdef _DEBUG
		std::lock_guard lock(mutex);
		std::cout << "{" << std::this_thread::get_id() << "} Insert " << key << " \"" << data << "\" " << std::boolalpha << res << std::endl;
#endif /* _DEBUG */
	}
}

template<typename _Kty, typename _Ty>
void deleter(cbst<_Kty, _Ty>& cbst, int num_ops, [[maybe_unused]] std::mutex& mutex)
{
	while (num_ops--)
	{
		auto&& key = generator::get_int(MIN_KEY, MAX_KEY);
		[[maybe_unused]] bool res = cbst.remove(std::move(key));

#ifdef _DEBUG
		std::lock_guard lock(mutex);
		std::cout << "{" << std::this_thread::get_id() << "} Remove " << key << " " << std::boolalpha << res << std::endl;
#endif /* _DEBUG */
	}
}

template<typename _Kty, typename _Ty>
void reader(cbst<_Kty, _Ty>& cbst, int num_ops, [[maybe_unused]] std::mutex& mutex)
{
	while (num_ops--)
	{
		auto&& key = generator::get_int(MIN_KEY, MAX_KEY);
		auto&& [data, suc] = cbst.get(std::move(key));

#ifdef _DEBUG
		std::lock_guard lock(mutex);
		std::cout << "{" << std::this_thread::get_id() << "} Get " << key << " " << std::boolalpha << suc << std::endl;
#endif /* _DEBUG */
	}
}

template<typename _Kty, typename _Ty>
void rebalancer(cbst<_Kty, _Ty>& cbst, int num_ops, std::mutex& mutex, const bool det)
{
	while (num_ops--)
	{
		auto&& key = generator::get_int(MIN_KEY, MAX_KEY);
		auto viols_num = cbst.fix_to_key(std::move(key), det);

#ifdef _DEBUG
		std::lock_guard lock(mutex);
		std::cout << "{" << std::this_thread::get_id() << "} Rebalance to " << key << " fixed " << viols_num << " violations" << std::endl;
#endif /* _DEBUG */
	}
}
