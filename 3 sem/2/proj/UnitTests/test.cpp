#include "pch.hpp"

#define _USE_MATH_DEFINES
#include "../../src/Ellipse.hpp"

struct EllipseTest : testing::Test
{
	std::unique_ptr<Ellipse> ellipse;

	inline EllipseTest() :
		ellipse(std::make_unique<Ellipse>(3., 5.))
	{ }
};

TEST_F(EllipseTest, UpdateParams)
{
	ellipse->a = 5.;

	EXPECT_DOUBLE_EQ(ellipse->a, 5.);

	ellipse->b = 4.;

	EXPECT_DOUBLE_EQ(ellipse->b, 4.);
}

TEST_F(EllipseTest, Focus)
{
	EXPECT_DOUBLE_EQ(ellipse->c, 4.);

	ellipse->a = 5.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->c, 4.);
}

TEST_F(EllipseTest, Eccentricity)
{
	EXPECT_DOUBLE_EQ(ellipse->e, 4. / 5);

	ellipse->a = 5.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->e, 4. / 5);
}

TEST_F(EllipseTest, Length)
{
	EXPECT_FLOAT_EQ(ellipse->length, 25.5269988633981);

	ellipse->a = 5.;
	ellipse->b = 3.;

	EXPECT_FLOAT_EQ(ellipse->length, 25.5269988633981);
}

TEST_F(EllipseTest, Area)
{
	EXPECT_DOUBLE_EQ(ellipse->area, 47.1238898038469);

	ellipse->a = 5.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->area, 47.1238898038469);
}

TEST_F(EllipseTest, MinMaxDistanceToFociSum)
{
	{
		auto&& [min, max] { ellipse->getMinMaxDistToFoci(0., 5.) };

		EXPECT_DOUBLE_EQ(min + max, 2 * 5.);
	}

	ellipse->a = 5.;
	ellipse->b = 3.;

	auto&& [min, max] { ellipse->getMinMaxDistToFoci(5., 0.) };

	//EXPECT_DOUBLE_EQ(min + max, 2 * 5.);
}

TEST_F(EllipseTest, GetCartesianY)
{
	EXPECT_DOUBLE_EQ(ellipse->getCartesianY(0.).first, -5.);
	EXPECT_DOUBLE_EQ(ellipse->getCartesianY(0.).second, 5.);

	ellipse->a = 5.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->getCartesianY(5.).first, -0.);
	EXPECT_DOUBLE_EQ(ellipse->getCartesianY(5.).second, 0.);
}

//========================================================

TEST_F(EllipseTest, FocusCircle)
{
	ellipse->a = 3.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->c, 0.);
}

TEST_F(EllipseTest, EccentricityCircle)
{
	ellipse->a = 3.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->e, 0.);
}

TEST_F(EllipseTest, LengthCircle)
{
	ellipse->a = 3.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->length, 2 * M_PI * ellipse->a);
}

TEST_F(EllipseTest, AreaCircle)
{
	ellipse->a = 3.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->area, M_PI * ellipse->a * ellipse->a);
}

TEST_F(EllipseTest, MinMaxDistanceToFociSumCircle)
{
	ellipse->a = 3.;
	ellipse->b = 3.;

	const auto&& [min, max] { ellipse->getMinMaxDistToFoci(0., 3.) };

	EXPECT_DOUBLE_EQ(min + max, 2 * ellipse->a);
}

TEST_F(EllipseTest, GetCartesianYCircle)
{
	ellipse->a = 3.;
	ellipse->b = 3.;

	EXPECT_DOUBLE_EQ(ellipse->getCartesianY(0.).first, -3.);
	EXPECT_DOUBLE_EQ(ellipse->getCartesianY(0.).second, 3.);
}