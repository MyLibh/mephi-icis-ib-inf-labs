#include "cbst.hpp"

#include <string>

namespace test
{
	void cond1()
	{
		cbst<int, std::string> tree;

		tree.insert(10, "");
		tree.insert(9, "");
		tree.insert(11, "");

		std::cout << "COND1 TEST\n\n" << tree << std::endl;

		tree.fix_to_key(10);

		std::cout << tree << std::endl;
	}

	void cond2()
	{
		cbst<int, std::string> tree;

		tree.insert(10, "");
		tree.insert(13, "");
		tree.insert(14, "");

		std::cout << "COND2 TEST(root)\n\n" << tree << std::endl;
		tree.fix_to_key(17); // root case
		std::cout << tree << std::endl;
		
		tree.insert(16, "");
		tree.insert(17, "");
		std::cout << "COND2 TEST(node)\n\n" << tree << std::endl;
		tree.fix_to_key(15); // root case
		std::cout << tree << std::endl;
	}

	void cond3()
	{
		cbst<int, std::string> tree;

		tree.insert(12, "");
		tree.insert(7, "");
		tree.insert(10, "");

		std::cout << "COND3 TEST(root)\n\n" << tree << std::endl;
		tree.fix_to_key(10);
		std::cout << tree << std::endl;
		
		tree.fix_to_key(10);
		tree.insert(9, "");
		tree.insert(8, "");
		std::cout << "COND3 TEST(node)\n\n" << tree << std::endl;
		tree.fix_to_key(9);
		std::cout << tree << std::endl;
	}

	void cond4()
	{
		cbst<int, std::string> tree;

		tree.insert(10, "");
		tree.insert(7, "");
		tree.insert(8, "");
		tree.insert(6, "");

		tree.remove(7);

		std::cout << "COND4 TEST\n\n" << tree << std::endl;
		tree.fix_to_key(7);
		std::cout << tree << std::endl;
	}
}

signed main()
{
	test::cond3();
	
	/*cbst<int, std::string> tree;

	tree.insert(6, "a");
	tree.insert(4, "b");
	tree.insert(3, "c");
	tree.insert(5, "d");
	tree.insert(7, "z");

	std::cout << tree << std::endl;

	tree.fix_to_key(3); tree.fix_to_key(5);

	std::cout << tree << std::endl;
	if(tree.remove(3))
		std::cout << tree << std::endl;

	if (auto&& [val, suc] = tree.get(4); suc)
		std::cout << val << std::endl;
		*/
	return 0;
}