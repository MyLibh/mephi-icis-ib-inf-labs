#include "cbst.hpp"
#include "thread_poll.hpp"

#include <string>
#include <iostream>

constexpr auto MIN_OPS = 1;
constexpr auto MAX_OPS = 5000;
constexpr auto THREADS_NUM = 4U;
constexpr auto MIN_KEY = 0;
constexpr auto MAX_KEY = 10;
constexpr auto DATA_LENGTH = 5U;

#pragma warning(disable : 26444)
// Avoid unnamed objects with custom construction and destruction.

signed main()
{
	try
	{
		cbst<int, std::string> cbst;
		
		auto num_ops = generator::get_int(MIN_OPS, MAX_OPS);
		std::cout << "Ops for each thread: " << num_ops << std::endl;

		thread_pool<std::function<void()>> tp(THREADS_NUM);
		[[maybe_unused]] std::mutex mutex;
		tp.add_job([&, num_ops]() mutable
			{
				while (num_ops--)
				{
					auto&& key = generator::get_int(MIN_KEY, MAX_KEY);
					auto&& data = generator::get_str(DATA_LENGTH);
					cbst.insert(std::move(key), std::move(data));

#ifdef _DEBUG
					std::lock_guard lock(mutex);
					std::cout << "{" << std::this_thread::get_id() << "} Insert " << key << " " << data << std::endl;
#endif /* _DEBUG */
				}
			});
		tp.add_job([&, num_ops]() mutable 
			{
				while (num_ops--)
				{
					auto&& key = generator::get_int(MIN_KEY, MAX_KEY);
					cbst.remove(std::move(key));

#ifdef _DEBUG
					std::lock_guard lock(mutex);
					std::cout << "{" << std::this_thread::get_id() << "} Remove " << key << std::endl;
#endif /* _DEBUG */
				}
			});
		tp.add_job([&, num_ops]() mutable
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
			});
		//tp.add_job([&, num_ops]() mutable { while (num_ops--) cbst.fix_to_key(generator::get_int(MIN_KEY, MAX_KEY)); });
		
		tp.wait_all();
		
		std::cout << cbst << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown exception" << std::endl;
	}

	return 0;
}