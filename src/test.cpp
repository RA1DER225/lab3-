#include <gtest/gtest.h>

#include "stack.h"
#include "TArithmeticExpression.h"

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

// ===================== Stack =====================

TEST(StackTest, NewStackIsEmpty) {
    Stack<int> s;

    EXPECT_TRUE(s.IsEmpty());
    EXPECT_EQ(s.size(), 0u);
}

TEST(StackTest, PushPopTopSize) {
    Stack<int> s;

    s.Push(10);
    EXPECT_FALSE(s.IsEmpty());
    EXPECT_EQ(s.size(), 1u);
    EXPECT_EQ(s.Top(), 10);

    s.Push(20);
    EXPECT_EQ(s.size(), 2u);
    EXPECT_EQ(s.Top(), 20);

    s.Pop();
    EXPECT_EQ(s.size(), 1u);
    EXPECT_EQ(s.Top(), 10);

    s.Pop();
    EXPECT_TRUE(s.IsEmpty());
    EXPECT_EQ(s.size(), 0u);
}

TEST(StackTest, PopEmptyStackDoesNothing) {
    Stack<int> s;

    EXPECT_NO_THROW(s.Pop());
    EXPECT_TRUE(s.IsEmpty());
    EXPECT_EQ(s.size(), 0u);
}

TEST(StackTest, StringStack) {
    Stack<std::string> s;

    s.Push("one");
    s.Push("two");

    EXPECT_EQ(s.Top(), "two");
    EXPECT_EQ(s.size(), 2u);

    s.Pop();
    EXPECT_EQ(s.Top(), "one");
}

// ===================== TArithmeticExpression: postfix =====================

TEST(TArithmeticExpressionPostfixTest, SimpleExpression) {
    TArithmeticExpression e("a*b+c-a");

    EXPECT_EQ(e.GetIndex(), "a*b+c-a");
    EXPECT_EQ(e.GetPostfix(), "ab*c+a-");
}

TEST(TArithmeticExpressionPostfixTest, Parentheses) {
    TArithmeticExpression e("(a+b)*c");

    EXPECT_EQ(e.GetPostfix(), "ab+c*");
}

TEST(TArithmeticExpressionPostfixTest, SpacesAreIgnored) {
    TArithmeticExpression e(" a + b * c ");

    EXPECT_EQ(e.GetPostfix(), "abc*+");
}

TEST(TArithmeticExpressionPostfixTest, OperatorPriority) {
    TArithmeticExpression e("a+b*c-d/e");

    EXPECT_EQ(e.GetPostfix(), "abc*+de/-");
}

TEST(TArithmeticExpressionPostfixTest, SingleOperand) {
    TArithmeticExpression e("a");

    EXPECT_EQ(e.GetPostfix(), "a");
    EXPECT_EQ(e.GetOperands(), std::vector<char>{'a'});
}

// ===================== TArithmeticExpression: operands =====================

TEST(TArithmeticExpressionOperandsTest, UniqueOperandsOnly) {
    TArithmeticExpression e("a+a*b");

    // map хранит ключи отсортированными, поэтому ожидаем a, b
    EXPECT_EQ(e.GetOperands(), (std::vector<char>{'a', 'b'}));
}

TEST(TArithmeticExpressionOperandsTest, OperandsAreSortedByMap) {
    TArithmeticExpression e("b+a");

    EXPECT_EQ(e.GetOperands(), (std::vector<char>{'a', 'b'}));
}

// ===================== TArithmeticExpression: calculate =====================

TEST(TArithmeticExpressionCalculateTest, SimpleExpression) {
    TArithmeticExpression e("a*b+c-a");

    std::map<char, double> values{
        {'a', 2.0},
        {'b', 3.0},
        {'c', 4.0}
    };

    // 2*3 + 4 - 2 = 8
    EXPECT_DOUBLE_EQ(e.Calculate(values), 8.0);
}

TEST(TArithmeticExpressionCalculateTest, Parentheses) {
    TArithmeticExpression e("(a+b)*c");

    std::map<char, double> values{
        {'a', 2.0},
        {'b', 3.0},
        {'c', 4.0}
    };

    // (2+3)*4 = 20
    EXPECT_DOUBLE_EQ(e.Calculate(values), 20.0);
}

TEST(TArithmeticExpressionCalculateTest, Division) {
    TArithmeticExpression e("a/b");

    std::map<char, double> values{
        {'a', 10.0},
        {'b', 2.0}
    };

    EXPECT_DOUBLE_EQ(e.Calculate(values), 5.0);
}

TEST(TArithmeticExpressionCalculateTest, OperatorPriority) {
    TArithmeticExpression e("a+b*c-d/e");

    std::map<char, double> values{
        {'a', 1.0},
        {'b', 2.0},
        {'c', 3.0},
        {'d', 8.0},
        {'e', 4.0}
    };

    // 1 + 2*3 - 8/4 = 1 + 6 - 2 = 5
    EXPECT_DOUBLE_EQ(e.Calculate(values), 5.0);
}

TEST(TArithmeticExpressionCalculateTest, MissingVariableUsesZero) {
    TArithmeticExpression e("a+b");

    std::map<char, double> values{
        {'a', 2.0}
    };

    // b по умолчанию 0.0, поэтому 2 + 0 = 2
    EXPECT_DOUBLE_EQ(e.Calculate(values), 2.0);
}

TEST(TArithmeticExpressionCalculateTest, UnknownVariableThrows) {
    TArithmeticExpression e("a+b");

    std::map<char, double> values{
        {'x', 1.0}
    };

    EXPECT_THROW(e.Calculate(values), std::out_of_range);
}