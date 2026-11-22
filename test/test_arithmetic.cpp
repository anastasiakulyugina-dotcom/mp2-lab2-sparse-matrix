#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "arithmetic.h"

TEST(TestArithmetic, Plus_Minus) {
	Arithmetic<double> calc("10 - 9 + 8");
	calc.lexm();

	ASSERT_TRUE(calc.isCorrect()); 

	calc.OPZ();

	EXPECT_EQ(calc.calculate(), 9.0);
}

TEST(TestArithmetic, Plus_MinusInt) {
	Arithmetic<int> calc("10 - 9 / 8");
	calc.lexm();

	ASSERT_TRUE(calc.isCorrect());

	calc.OPZ();

	EXPECT_EQ(calc.calculate(), 9);
}


TEST(TestArithmetic, Mul_Div) {
	Arithmetic<double> calc("5 * 6 / 2");
	calc.lexm();

	ASSERT_TRUE(calc.isCorrect());
	calc.OPZ();

	EXPECT_EQ(calc.calculate(), 15.0);
}

TEST(TestArithmetic, Brascket_prior_correct) {
	Arithmetic<double> calc("(8+1)*9");
	calc.lexm();

	ASSERT_TRUE(calc.isCorrect());

	calc.OPZ();
	EXPECT_EQ(calc.calculate(), 81.0);
}

TEST(TestArithmetic, Correct_Brascket) {
	Arithmetic<double> calc("( 8 + 33");
	calc.lexm();

	EXPECT_FALSE(calc.isCorrect());
}

TEST(TestArithmetic, Correct_Brascket2) {
	Arithmetic<double> calc("( ) *8");
	calc.lexm();

	EXPECT_FALSE(calc.isCorrect()); 
}

TEST(TestArithmetic, Operators_Correct) {
	Arithmetic<double> calc("34 */ 7");
	calc.lexm();

	EXPECT_FALSE(calc.isCorrect()); 
}

TEST(TestArithmetic, Operators_and_brasket_Correct) {
	Arithmetic<double> calc("3( 8 - 2");
	calc.lexm();

	EXPECT_FALSE(calc.isCorrect()); 
}

TEST(TestArithmetic, OPZ_is_Correct) {
	Arithmetic<double> calc("3 + 4 - 1");
	calc.lexm();
	ASSERT_TRUE(calc.isCorrect());
	calc.OPZ();

	std::vector<std::string> etalon = { "3", "4", "+", "1", "-" }; 
	EXPECT_EQ(calc.getPostfix(), etalon);
}

TEST(TestArithmetic, Spaces_and_Empty) {
	Arithmetic<double> calc1("   2   +    3   * 4   ");
	calc1.lexm();

	ASSERT_TRUE(calc1.isCorrect());

	calc1.OPZ();

	EXPECT_DOUBLE_EQ(calc1.calculate(), 14.0);

	Arithmetic<double> calc2("   ");
	calc2.lexm();

	EXPECT_FALSE(calc2.isCorrect());
}

TEST(TestArithmetic, RuntimeErrors) {
	Arithmetic<double> calc1("a + 5");
	calc1.lexm();

	ASSERT_TRUE(calc1.isCorrect());
	calc1.OPZ();

	EXPECT_THROW(calc1.calculate(), std::runtime_error);
}

TEST(TestArithmetic, Unar1) {
	Arithmetic<double> calc("-5 + (+3)");
	calc.lexm();

	ASSERT_TRUE(calc.isCorrect());
	calc.OPZ();

	std::vector<std::string> expected = { "5", "~", "3", "#", "+" };

	EXPECT_EQ(calc.getPostfix(), expected);
	EXPECT_DOUBLE_EQ(calc.calculate(), -2.0);
}
