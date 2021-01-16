#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << ssts::version() << std::endl;

    // ::testing::GTEST_FLAG(filter) = "At.*";
    // ::testing::GTEST_FLAG(filter) = "Disable.*";
    // ::testing::GTEST_FLAG(filter) = "Every.*";
    // ::testing::GTEST_FLAG(filter) = "In.*";
    // ::testing::GTEST_FLAG(filter) = "Missing.*";
    // ::testing::GTEST_FLAG(filter) = "Remove.*";
    // ::testing::GTEST_FLAG(filter) = "Stop.*";
    // ::testing::GTEST_FLAG(filter) = "Duplicated.*";

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
