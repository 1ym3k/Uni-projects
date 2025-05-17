#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>

// Declare validate_input as an external C function
extern "C" {
    int validate_input(const char *file_path, FILE *output);
    const char* number_to_words(int num);
}

// Testy dla funkcji number_to_words
TEST(NumberToWordsTest, Eleven) {
    EXPECT_STREQ(number_to_words(11), "jedenaście figur");
}

TEST(NumberToWordsTest, One) {
    EXPECT_STREQ(number_to_words(1), "jedna figura");
}

TEST(NumberToWordsTest, Twenty) {
    EXPECT_STREQ(number_to_words(20), "dwadzieścia figur");
}

TEST(NumberToWordsTest, OneHundred) {
    EXPECT_STREQ(number_to_words(100), "sto figur");
}


//pozostale testy
// Test valid input with single configuration
TEST(InputValidationTest, ValidInput) {
    // Create a temporary input file
    FILE* input = fopen("test_valid_input.txt", "w");
    ASSERT_TRUE(input != NULL);
    
    // Write valid test data
    fprintf(input, "1\n5 3 Qa1 Qb2 Qc3\n");
    fclose(input);

    // Open output file
    FILE* output = fopen("test_output.tex", "w");
    ASSERT_TRUE(output != NULL);

    // Validate input
    int result = validate_input("test_valid_input.txt", output);
    
    // Close output file
    fclose(output);

    // Check validation result
    EXPECT_EQ(result, 0);  // Expect successful validation

    // Clean up test files
    remove("test_valid_input.txt");
    remove("test_output.tex");
}

// Test input with too many configurations
TEST(InputValidationTest, TooManyConfigurations) {
    // Create a temporary input file
    FILE* input = fopen("test_many_configs.txt", "w");
    ASSERT_TRUE(input != NULL);
    
    // Write invalid configuration count
    fprintf(input, "101\n5 3 Qa1 Qb2 Qc3\n");
    fclose(input);

    // Open output file
    FILE* output = fopen("test_output.tex", "w");
    ASSERT_TRUE(output != NULL);

    // Validate input
    int result = validate_input("test_many_configs.txt", output);
    
    // Close output file
    fclose(output);

    // Check validation result
    EXPECT_EQ(result, 1);  // Expect validation failure

    // Clean up test files
    remove("test_many_configs.txt");
    remove("test_output.tex");
}

// Test input with invalid board size
TEST(InputValidationTest, InvalidBoardSize) {
    // Create a temporary input file
    FILE* input = fopen("test_board_size.txt", "w");
    ASSERT_TRUE(input != NULL);
    
    // Write invalid board size
    fprintf(input, "1\n10 3 Qa1 Qb2 Qc3\n");
    fclose(input);

    // Open output file
    FILE* output = fopen("test_output.tex", "w");
    ASSERT_TRUE(output != NULL);

    // Validate input
    int result = validate_input("test_board_size.txt", output);
    
    // Close output file
    fclose(output);

    // Check validation result
    EXPECT_EQ(result, 1);  // Expect validation failure

    // Clean up test files
    remove("test_board_size.txt");
    remove("test_output.tex");
}

// Test input with too many queens
TEST(InputValidationTest, TooManyQueens) {
    // Create a temporary input file
    FILE* input = fopen("test_many_queens.txt", "w");
    ASSERT_TRUE(input != NULL);
    
    // Write configuration with too many queens
    fprintf(input, "1\n5 26 Qa1 Qa2 Qa3 Qa4 Qa5 Qb1 Qb2 Qb3 Qb4 Qb5 Qc1 Qc2 Qc3 Qc4 Qc5 Qd1 Qd2 Qd3 Qd4 Qd5 Qe1 Qe2 Qe3 Qe4 Qe5 Qf1\n");
    fclose(input);

    // Open output file
    FILE* output = fopen("test_output.tex", "w");
    ASSERT_TRUE(output != NULL);

    // Validate input
    int result = validate_input("test_many_queens.txt", output);
    
    // Close output file
    fclose(output);

    // Check validation result
    EXPECT_EQ(result, 1);  // Expect validation failure

    // Clean up test files
    remove("test_many_queens.txt");
    remove("test_output.tex");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

