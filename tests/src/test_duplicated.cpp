#include "test_scheduler.hpp"

namespace ssts
{

class Duplicated : public SchedulerTest { };

TEST_F(Duplicated, DuplicatesNotAllowed)
{
    n_tasks = 8;
    InitScheduler(4u);
    force_duplicate_allowed(false);

    StartAllTasksEvery(10ms, 2s);
    Sleep(1s);
    EXPECT_EQ(s->size(), 1u);
}

TEST_F(Duplicated, DuplicatesAllowed)
{
    n_tasks = 8;
    InitScheduler(4u);
    force_duplicate_allowed(true);

    StartAllTasksEvery(10ms, 2s);
    Sleep(1s);
    EXPECT_EQ(s->size(), 8u);
}

}
