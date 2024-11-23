
#include <gtest/gtest.h>
#include "inputValidation.h"

// Test case for valid numeric strings
TEST(IsNumericTest, ValidNumericStrings) {
    EXPECT_TRUE(isNumeric("12345"));  // Valid integer string
    EXPECT_TRUE(isNumeric("0"));      // Single digit
    EXPECT_TRUE(isNumeric("987654321")); // Large number
}

// Test case for invalid numeric strings
TEST(IsNumericTest, InvalidNumericStrings) {
    EXPECT_FALSE(isNumeric("123a5")); // Contains non-digit characters
    EXPECT_FALSE(isNumeric(""));      // Empty string
    EXPECT_FALSE(isNumeric("abcde")); // Non-numeric string
    EXPECT_FALSE(isNumeric("12 34")); // Contains spaces
    EXPECT_FALSE(isNumeric("123.45")); // Contains a decimal point
}

// Test case for strings with edge cases (e.g., leading zeros)
TEST(IsNumericTest, EdgeCaseStrings) {
    EXPECT_TRUE(isNumeric("000123")); // Leading zeros should be considered valid
    EXPECT_FALSE(isNumeric("12.34")); // A number with a decimal point is not numeric
    EXPECT_FALSE(isNumeric("-123"));  // Negative numbers should not be valid
}

// Test case for strings with different valid lengths
TEST(IsNumericTest, DifferentLengthStrings) {
    EXPECT_TRUE(isNumeric("1"));    // Single digit
    EXPECT_TRUE(isNumeric("123"));  // Multi-digit number
    EXPECT_TRUE(isNumeric("1234567890")); // Very large number
}


