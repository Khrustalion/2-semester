#include <gtest/gtest.h>
#include <lib/Adapters.h>
#include <vector>
#include <map>


class TestSuiteOneAdapter : public::testing::TestWithParam<std::tuple<std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>, int>> {};

TEST_P (TestSuiteOneAdapter, Transform) {
    auto values = std::get<0>(GetParam()) | transform([](int n) {return n * n;});

    auto expected = std::get<1>(GetParam());

    std::vector<int> answer;

    for (auto v : values) {
        answer.push_back(v);
    }

    ASSERT_EQ(answer, expected);
}

TEST_P (TestSuiteOneAdapter, Filter) {
    auto values = std::get<0>(GetParam()) | filter([](int n) {return n % 2;});

    auto expected = std::get<2>(GetParam());

    std::vector<int> answer;

    for (auto v: values) {
        answer.push_back(v);
    }

    ASSERT_EQ(answer, expected);
}

TEST_P (TestSuiteOneAdapter, Take) {
    auto number = std::get<5>(GetParam());
    auto values = std::get<0>(GetParam()) | take(number);

    auto expected = std::get<3>(GetParam());

    std::vector<int> answer;

    for (auto v: values) {
        answer.push_back(v);
    }

    ASSERT_EQ(answer, expected);
}

TEST_P (TestSuiteOneAdapter, Drop) {
    auto number = std::get<5>(GetParam());
    auto values = std::get<0>(GetParam()) | drop(number);

    auto expected = std::get<4>(GetParam());

    std::vector<int> answer;

    for (auto v: values) {
        answer.push_back(v);
    }

    ASSERT_EQ(answer, expected);
}

INSTANTIATE_TEST_SUITE_P (
    Group,
    TestSuiteOneAdapter, 
    testing::Values(
        std::make_tuple(std::vector<int>{1, 2, 3}, std::vector<int>{1, 4, 9}, std::vector<int>{1, 3}, std::vector<int>{1, 2}, std::vector<int>{3}, 2),
        std::make_tuple(std::vector<int>{-10, 9, -20, 13}, std::vector<int>{100, 81, 400, 169}, std::vector<int>{9, 13}, std::vector<int>{-10, 9, -20}, std::vector<int>{13}, 3),
        std::make_tuple(std::vector<int>{655, -711, 512, 723, -817, -355, 552, 517, -146, -630, -983, -307, 766, -558, 4, 76, 841, 145, -25, -2},
                        std::vector<int>{429025, 505521, 262144, 522729, 667489, 126025, 304704, 267289, 21316, 396900, 966289, 94249, 586756, 311364, 16, 5776, 707281, 21025, 625, 4},
                        std::vector<int>{655, -711, 723, -817, -355, 517, -983, -307, 841, 145, -25},
                        std::vector<int>{655, -711, 512, 723, -817, -355, 552, 517, -146, -630, -983, -307}, 
                        std::vector<int>{766, -558, 4, 76, 841, 145, -25, -2},
                        12),
        std::make_tuple(std::vector<int>{156, -915, 741, -531, -116, 288, 679, 232, 579, 484, -31, -685, 360, 673, -569, 446, -629, 844, -438, -781, 994, 597, 643, 372, -13, 380, 954, -804, -997, 413, 807, -890, -604, -110, -263, -927, 395, 501, 951, -385, -985, 249, -964, -998, -724, -710, -393, 284, -702, -992},
                        std::vector<int>{24336, 837225, 549081, 281961, 13456, 82944, 461041, 53824, 335241, 234256, 961, 469225, 129600, 452929, 323761, 198916, 395641, 712336, 191844, 609961, 988036, 356409, 413449, 138384, 169, 144400, 910116, 646416, 994009, 170569, 651249, 792100, 364816, 12100, 69169, 859329, 156025, 251001, 904401, 148225, 970225, 62001, 929296, 996004, 524176, 504100, 154449, 80656, 492804, 984064},
                        std::vector<int>{-915, 741, -531, 679, 579, -31, -685, 673, -569, -629, -781, 597, 643, -13, -997, 413, 807, -263, -927, 395, 501, 951, -385, -985, 249, -393},
                        std::vector<int>{156, -915, 741, -531, -116, 288, 679, 232, 579, 484, -31, -685, 360, 673, -569, 446, -629, 844, -438, -781, 994, 597, 643, 372, -13, 380, 954, -804, },
                        std::vector<int>{-997, 413, 807, -890, -604, -110, -263, -927, 395, 501, 951, -385, -985, 249, -964, -998, -724, -710, -393, 284, -702, -992},
                        28)

    )
);

class TestSuiteOndeAdapterAssociative : public::testing::TestWithParam<std::tuple<std::map<double, int>, std::vector<double>, std::vector<int>>> {};

TEST_P (TestSuiteOndeAdapterAssociative, Keys) {
    auto values = std::get<0>(GetParam()) | keys();

    auto expected = std::get<1>(GetParam());

    std::vector<double> answer;

    for (auto v : values) {
        answer.push_back(v);
    }

    ASSERT_EQ(answer, expected);
}

TEST_P (TestSuiteOndeAdapterAssociative, Values) {
    auto values_ = std::get<0>(GetParam()) | values();

    auto expected = std::get<2>(GetParam());

    std::vector<int> answer;

    for (auto v : values_) {
        answer.push_back(v);
    }

    ASSERT_EQ(answer, expected);
}

INSTANTIATE_TEST_SUITE_P (
    Group,
    TestSuiteOndeAdapterAssociative,
    testing::Values(
        std::make_tuple(std::map<double, int>{{1.2, 1}, {1.3, 2}, {1.4, 3}, {1.5, 4}}, std::vector<double>{1.2, 1.3, 1.4, 1.5}, std::vector<int>{1, 2, 3, 4}),
        std::make_tuple(std::map<double, int>{{9.5, 614}, {6.1, 384}, {1.6, 463}, {2.0, 571}, {4.9, 950}, {4.7, 5}, {5.9, 819}, {9.4, 774}, {2.7, 764}, {2.2, 122}, {8.8, 776}, {1.9, 604}, {8.3, 624}, {9.6, 525}, {2.1, 826}, {6.8, 589}, {4.2, 940}, {7.1, 785}, {7.5, 920}, {2.5, 904}, {7.0, 896}, {2.4, 643}, {4.3, 402}, {8.6, 363}}, 
        std::vector<double>{1.6, 1.9, 2.0, 2.1, 2.2, 2.4, 2.5, 2.7, 4.2, 4.3, 4.7, 4.9, 5.9, 6.1, 6.8, 7.0, 7.1, 7.5, 8.3, 8.6, 8.8, 9.4, 9.5, 9.6}, 
        std::vector<int>{463, 604, 571, 826, 122, 643, 904, 764, 940, 402, 5, 950, 819, 384, 589, 896, 785, 920, 624, 363, 776, 774, 614, 525}),
        std::make_tuple(std::map<double, int>{{0.0, 988}, {0.1, 852}, {0.2, 348}, {0.3, 16}, {0.4, 534}, {0.5, 198}, {0.6, 506}, {0.7, 928}, {0.8, 258}, {0.9, 450}, {1.0, 107}, {1.1, 366}, {1.2, 202}, {1.3, 687}, {1.4, 515}, {1.5, 113}, {1.6, 291}, {1.7, 586}, {1.8, 25}, {1.9, 433}, {2.0, 766}, {2.1, 22}, {2.2, 787}, {2.3, 747}, {2.4, 143}, {2.5, 728}, {2.6, 285}, {2.7, 713}, {2.8, 455}, {2.9, 782}, {3.0, 450}, {3.1, 683}, {3.2, 797}, {3.3, 567}, {3.4, 716}, {3.5, 649}, {3.6, 864}, {3.7, 217}, {3.8, 68}, {3.9, 642}, {4.0, 725}, {4.1, 376}, {4.2, 561}, {4.3, 587}, {4.4, 822}, {4.5, 25}, {4.6, 578}, {4.7, 605}, {4.8, 785}, {4.9, 50}, {5.0, 941}, {5.1, 704}, {5.2, 645}, {5.3, 643}, {5.4, 558}, {5.5, 820}, {5.6, 262}, {5.7, 869}, {5.8, 668}, {5.9, 564}, {6.0, 768}, {6.1, 776}, {6.2, 800}, {6.3, 110}, {6.4, 538}, {6.5, 247}, {6.6, 92}, {6.7, 755}, {6.8, 133}, {6.9, 690}, {7.0, 870}, {7.1, 473}, {7.2, 868}, {7.3, 489}, {7.4, 741}, {7.5, 240}, {7.6, 8}, {7.7, 123}, {7.8, 0}, {7.9, 362}, {8.0, 49}, {8.1, 69}, {8.2, 423}, {8.3, 261}, {8.4, 59}, {8.5, 674}, {8.6, 893}, {8.7, 99}, {8.8, 531}, {8.9, 790}, {9.0, 114}, {9.1, 214}, {9.2, 160}, {9.3, 960}, {9.4, 49}, {9.5, 180}, {9.6, 332}, {9.7, 797}, {9.8, 798}, {9.9, 399}}, 
        std::vector<double>{0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.0, 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8, 6.9, 7.0, 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8, 7.9, 8.0, 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8, 8.9, 9.0, 9.1, 9.2, 9.3, 9.4, 9.5, 9.6, 9.7, 9.8, 9.9, }, 
        std::vector<int>{988, 852, 348, 16, 534, 198, 506, 928, 258, 450, 107, 366, 202, 687, 515, 113, 291, 586, 25, 433, 766, 22, 787, 747, 143, 728, 285, 713, 455, 782, 450, 683, 797, 567, 716, 649, 864, 217, 68, 642, 725, 376, 561, 587, 822, 25, 578, 605, 785, 50, 941, 704, 645, 643, 558, 820, 262, 869, 668, 564, 768, 776, 800, 110, 538, 247, 92, 755, 133, 690, 870, 473, 868, 489, 741, 240, 8, 123, 0, 362, 49, 69, 423, 261, 59, 674, 893, 99, 531, 790, 114, 214, 160, 960, 49, 180, 332, 797, 798, 399})
    )
 );

class TestSuiteTwoAdapters : public::testing::TestWithParam<std::tuple<std::vector<int>, std::vector<int>, std::vector<int>, int>> {};

TEST_P (TestSuiteTwoAdapters, TransfromFilter) {
    auto values = std::get<0>(GetParam()) | filter([] (int n) {return n % 2 == 0;}) | transform([] (int n) {return n / 2;});

    auto expected = std::get<1>(GetParam());

    std::vector<int> answer;

    for (auto v : values) {
        answer.push_back(v);
    }

    ASSERT_EQ(answer, expected);
 }

 TEST_P (TestSuiteTwoAdapters, DropFilter) {
    int n = std::get<3>(GetParam());
    auto values = std::get<0>(GetParam()) | drop(n) | filter([] (int n) {return n % 2 == 0;});

    auto expected = std::get<2>(GetParam());

    std::vector<int> answer;

    for (auto v : values) {
        answer.push_back(v);
    }

    ASSERT_EQ(answer, expected);
 }

 INSTANTIATE_TEST_SUITE_P (
    Group,
    TestSuiteTwoAdapters,
    testing::Values(
        std::make_tuple(std::vector<int>{1, 2, 3, 4}, std::vector<int>{1, 2}, std::vector<int>{4}, 2),
        std::make_tuple(std::vector<int>{948, 681, 294, 904, 309, 629, 844, 264, 707, 635, 987, 564, 52, 585, 264, 90, 4, 306, 467, 156, 963, 523, 833, 60, 389, 604, 304, 424, 12, 336, 566, 861, 641, 643, 297, 490, 247, 126, 525, 269}, 
        std::vector<int>{474, 147, 452, 422, 132, 282, 26, 132, 45, 2, 153, 78, 30, 302, 152, 212, 6, 168, 283, 245, 63},
        std::vector<int>{90, 4, 306, 156, 60, 604, 304, 424, 12, 336, 566, 490, 126}, 
        15),
        std::make_tuple(std::vector<int>{138, 138, 727, 261, 33, 544, 516, 541, 494, 436, 481, 656, 999, 97, 598, 279, 218, 141, 681, 898, 855, 763, 443, 604, 573, 515, 592, 721, 591, 340, 889, 304, 188, 603, 480, 952, 432, 661, 294, 880, 739, 133, 999, 1000, 272, 412, 454, 181, 186, 162, 681, 399, 107}, 
        std::vector<int>{69, 69, 272, 258, 247, 218, 328, 299, 109, 449, 302, 296, 170, 152, 94, 240, 476, 216, 147, 440, 500, 136, 206, 227, 93, 81},
        std::vector<int>{604, 592, 340, 304, 188, 480, 952, 432, 294, 880, 1000, 272, 412, 454, 186, 162, }, 
        20) 
    )
 );
