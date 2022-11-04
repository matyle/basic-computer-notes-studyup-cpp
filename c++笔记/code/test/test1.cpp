#include <gtest/gtest.h>
#include <numeric>
#include <vector>
// 测试集为 MyTest，测试案例为 Sum
TEST(MyTest, Sum)
{
    std::vector<int> vec{1, 2, 3, 4, 5};
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    EXPECT_EQ(sum, 15);
    EXPECT_EQ(sum, 16);
}
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}