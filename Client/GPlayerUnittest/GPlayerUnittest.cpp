#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING 1
#include <gtest/gtest.h>

class VecTest : public testing::Test {
protected:
};

TEST_F(VecTest, Basics2) {
    EXPECT_TRUE(1, 1) << "this is a test";
    EXPECT_TRUE(2, 2) << "this is a test";
}

//int main(int argc, char** argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
