// test_inputValidationLogAppend.cpp
#include <gtest/gtest.h>
#include "test_inputValidationLogAppend.h"

// Test case for valid numeric strings
TEST(IsNumericTest, ValidNumericStrings) {
    EXPECT_TRUE(isNumeric("12345"));  // Valid integer string
    EXPECT_TRUE(isNumeric("0"));      // Single digit
    EXPECT_TRUE(isNumeric("987654321")); // Large number
}
