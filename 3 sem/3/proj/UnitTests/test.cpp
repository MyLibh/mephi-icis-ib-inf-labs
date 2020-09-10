#include "pch.hpp"
#include "../../src/DiceGroup.hpp"
#include "../../src/DiceGroup.cpp"

struct DiceGroupTest : testing::Test
{
	std::unique_ptr<DiceGroup> dg;

	inline DiceGroupTest() :
		dg(std::make_unique<DiceGroup>())
	{ }
};



TEST_F(DiceGroupTest, AddDice)
{
	*dg += 5;

	EXPECT_EQ(dg->getSum(), 5);
}

TEST_F(DiceGroupTest, AddDices)
{
	*dg += 5;
	*dg += 6;

	EXPECT_EQ(dg->getSum(), 11);
}

TEST_F(DiceGroupTest, RemoveExistingDice)
{
	*dg += 2;

	*dg -= 2;

	EXPECT_EQ(dg->getSum(), 0);
}

TEST_F(DiceGroupTest, RemoveNonExistingDice)
{
	*dg += 2;

	*dg -= 1;

	EXPECT_EQ(dg->getSum(), 2);
}

TEST_F(DiceGroupTest, RemoveExistingDicesMulti)
{
	*dg += 2;
	*dg += 5;
	*dg += 2;

	*dg -= 2;

	EXPECT_EQ(dg->getSum(), 5);
}

TEST_F(DiceGroupTest, GetDice)
{
	*dg += 2;
	*dg += 5;
	*dg += 6;

	EXPECT_EQ((*dg)[0], 2);
	EXPECT_EQ((*dg)[1], 5);
	EXPECT_EQ((*dg)[2], 6);
}

TEST_F(DiceGroupTest, DefaultSum)
{
	EXPECT_EQ(dg->getSum(), 0);
}

TEST_F(DiceGroupTest, Sum)
{
	*dg += 1;
	*dg += 3;
	*dg += 6;

	EXPECT_EQ(dg->getSum(), 10);
}

TEST_F(DiceGroupTest, Concurrence)
{
	*dg += 1;
	*dg += 3;

	EXPECT_EQ(dg->hasConcurrence(), false);

	*dg += 1;

	EXPECT_EQ(dg->hasConcurrence(), true);
}