#include <gtest/gtest.h>
#include "lib/Scheduler.cpp"

class TestSuiteOneTask : public::testing::TestWithParam<std::tuple<int, int, int, int, double, double, double, double>> {};

TEST_P(TestSuiteOneTask, PlusInt) {
    Scheduler scheduler;

    int answer = std::get<2>(GetParam());

    auto task1 = scheduler.add( [] (int a, int b) {return a + b;}, std::get<0>(GetParam()), std::get<1>(GetParam()));

    int expected = scheduler.getResult<int>(task1);

    ASSERT_EQ(answer, expected);
}

TEST_P(TestSuiteOneTask, MinusInt) {
    Scheduler scheduler;

    int answer = std::get<3>(GetParam());

    auto task1 = scheduler.add([] (int a, int b) {return a - b;}, std::get<0>(GetParam()), std::get<1>(GetParam()));

    int expected = scheduler.getResult<int>(task1);

    ASSERT_EQ(answer, expected);
}

TEST_P(TestSuiteOneTask, PlusDouble) {
    Scheduler scheduler;

    double answer = std::get<6>(GetParam());

    auto task1 = scheduler.add([] (double a, double b) {return a + b;}, std::get<4>(GetParam()), std::get<5>(GetParam()));

    double expected = scheduler.getResult<double>(task1);

    ASSERT_DOUBLE_EQ(answer, expected);
}

TEST_P(TestSuiteOneTask, MinusDouble) {
    Scheduler scheduler;

    double answer = std::get<7>(GetParam());

    auto task1 = scheduler.add([] (double a, double b) {return a - b;}, std::get<4>(GetParam()), std::get<5>(GetParam()));

    double expected = scheduler.getResult<double>(task1);

    ASSERT_DOUBLE_EQ(answer, expected);
}

INSTANTIATE_TEST_SUITE_P (
    Group,
    TestSuiteOneTask,
    testing::Values(
        std::make_tuple(1, 3, 4, -2, 67.76, 98.34, 166.1, -30.58),
        std::make_tuple(956, 613, 1569, 343, 86.63, 92.69, 179.32, -6.06),
        std::make_tuple(976, 495, 1471, 481, 10.95, 5.77, 16.72, 5.18),
        std::make_tuple(6, 633, 639, -627, 54.61, 70.28, 124.89, -15.67),
        std::make_tuple(452, 876, 1328, -424, 46.5, 96.96, 143.46, -50.46),
        std::make_tuple(778, 681, 1459, 97, 59.55, 85.38, 144.93, -25.83),
        std::make_tuple(368, 412, 780, -44, 22.53, 95.76, 118.29, -73.23),
        std::make_tuple(935, 969, 1904, -34, 21.87, 6.91, 28.78, 14.96),
        std::make_tuple(776, 701, 1477, 75, 12.17, 77.83, 90.0, -65.66),
        std::make_tuple(732, 63, 795, 669, 63.62, 92.14, 155.76, -28.52)
    )
);

class TestSuiteManyTasks : public::testing::TestWithParam<std::tuple<int, int, int, int, int>> {};

TEST_P(TestSuiteManyTasks, SimpleExpression) {
    //Expression: 3*a + 4*b
    Scheduler scheduler;

    int a = std::get<0>(GetParam());
    int b = std::get<1>(GetParam());
    
    auto task1 = scheduler.add([] (int a) {return 3 * a;}, a);
    auto task2 = scheduler.add([] (int b) {return 4 * b;}, b);
    auto task3 = scheduler.add([] (int a, int b) {return a + b;}, scheduler.getFutureResult<int>(task1), scheduler.getFutureResult<int>(task2));

    int answer = scheduler.getResult<int>(task3);

    int expected = std::get<4>(GetParam());

    ASSERT_EQ(answer, expected);
}

TEST_P(TestSuiteManyTasks, Expression) {
    //Expression: 12ac + 4ab - 3bc
    Scheduler scheduler;

    int a = std::get<0>(GetParam());
    int b = std::get<1>(GetParam());
    int c = std::get<2>(GetParam());

    auto funcM = [] (int a, int b) {return a * b;};

    auto task1 = scheduler.add(funcM, a, 4);
    auto task2 = scheduler.add(funcM, c, 3);
    auto task3 = scheduler.add(funcM, scheduler.getFutureResult<int>(task1), scheduler.getFutureResult<int>(task2));
    auto task4 = scheduler.add(funcM, scheduler.getFutureResult<int>(task1), b);
    auto task5 = scheduler.add(funcM, scheduler.getFutureResult<int>(task2), b);
    auto task6 = scheduler.add([] (int a, int b) {return a + b;}, scheduler.getFutureResult<int>(task3), scheduler.getFutureResult<int>(task4));
    auto task7 = scheduler.add([] (int a, int b) {return a - b;}, scheduler.getFutureResult<int>(task6), scheduler.getFutureResult<int>(task5));

    int answer = scheduler.getResult<int>(task7);

    int expected = std::get<3>(GetParam());

    ASSERT_EQ(answer, expected);
}

INSTANTIATE_TEST_SUITE_P (
    Group,
    TestSuiteManyTasks,
    testing::Values(
        std::make_tuple(535, 468, 209, 2049864, 3477),
        std::make_tuple(399, 665, 648, 2871204, 3857),
        std::make_tuple(952, 597, 500, 7089876, 5244),
        std::make_tuple(636, 402, 903, 6825366, 3516),
        std::make_tuple(999, 722, 344, 6263880, 5885),
        std::make_tuple(54, 671, 609, -686349, 2846),
        std::make_tuple(524, 677, 748, 4603228, 4280),
        std::make_tuple(524, 677, 748, 4603228, 4280),
        std::make_tuple(776, 362, 866, 8247364, 3776),
        std::make_tuple(393, 658, 272, 1780200, 3811)
    )
);