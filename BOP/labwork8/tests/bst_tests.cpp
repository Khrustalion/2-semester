#include <gtest/gtest.h>
#include <lib/BinarySearchTree.h>
#include <vector>
#include <string>

TEST(INSERT, SimpleInsertCase) {
  BinarySearchTree<int, PreOrder> bst;
  bst.insert(1);
  bool answer = (bst.size() == 1) && (bst.find(1) != bst.end());

  ASSERT_TRUE(answer);
}

TEST(INSERT, StringInsertCase) {
  BinarySearchTree<std::string, PostOrder> bst;

  bst.insert("123");
  bst.insert("124");

  bool answer = (bst.size() == 2) && (bst.find("123") != bst.end()) && (bst.find("124") != bst.end());

  ASSERT_TRUE(answer);
}

TEST(INSERT, DoubleInsertCase) {
  BinarySearchTree<int, PreOrder> bst;
  bst.insert(1);
  bst.insert(1);
  bool answer = (bst.size() == 1) && (bst.find(1) != bst.end());

  ASSERT_TRUE(answer);
}

TEST(erase, SimpleExtrtact) {
  BinarySearchTree<int, PreOrder> bst;
  bst.insert(1);
  bst.erase(1);

  bool answer = (bst.find(1) == bst.end()) && (bst.size() == 0);

  ASSERT_TRUE(answer);
}

TEST(erase, DoubleErase) {
  BinarySearchTree<int, PreOrder> bst;
  bst.insert(1);

  bst.erase(1);
  bst.erase(1);

  bool answer = (bst.find(1) == bst.end()) && (bst.size() == 0);

  ASSERT_TRUE(answer);
}

TEST(erase, RootErase) {
  BinarySearchTree<int, PreOrder> bst;
  bst.insert(1);
  bst.insert(0);
  bst.insert(2);

  bst.erase(1); // Node(1) is root

  bool answer = (bst.find(1) == bst.end()) && (bst.size() == 2);

  ASSERT_TRUE(answer);
}

TEST(erase, HardNodEerase) {
  BinarySearchTree<int, PreOrder> bst;
  bst.insert(1);
  bst.insert(0);
  bst.insert(3);
  bst.insert(2);
  bst.insert(5);
  bst.insert(4);

  bst.erase(3); //next isn't neighbore

  bool answer = (bst.find(3) == bst.end()) && (bst.size() == 5);

  ASSERT_TRUE(answer);
}

class TestSuiteInt : public ::testing::TestWithParam<std::tuple<std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>, int, int>> {};

TEST_P(TestSuiteInt, EqualBst) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, PreOrder> bst_l{values.begin(), values.end(), std::less<int>()};
  BinarySearchTree<int, PreOrder> bst_r{values.begin(), values.end(), std::less<int>()};

  bool answer = (bst_l == bst_r);

  ASSERT_TRUE(answer);
}

TEST_P(TestSuiteInt, NotEqualBst) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, PreOrder> bst_l{values.begin(), values.end(), std::less<int>()};
  BinarySearchTree<int, PreOrder> bst_r{values.begin(), values.end(), std::less<int>()};

  bst_l.erase(std::get<4>(GetParam()));

  bool answer = bst_l == bst_r; //different sizes of bst

  ASSERT_FALSE(answer);
}

TEST_P(TestSuiteInt, HardNotEqualBst) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, PreOrder> bst_l{values.begin(), values.end(), std::less<int>()};
  BinarySearchTree<int, PreOrder>bst_r{values.begin(), values.end(), std::less<int>()};

  bst_l.erase(std::get<4>(GetParam()));
  bst_l.insert(std::get<5>(GetParam()));

  bool answer = bst_l == bst_r; //equal sizes, but different nodes

  ASSERT_FALSE(answer);
}


TEST_P(TestSuiteInt, distance) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, PreOrder> bst{values.begin(), values.end(), std::less<int>()};

  bool answer = (std::distance(bst.begin(), bst.end()) == bst.size());

  ASSERT_TRUE(answer);
}

TEST_P(TestSuiteInt, FoundCase) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, PreOrder> bst{values.begin(), values.end(), std::less<int>()};

  bool answer = bst.find(std::get<4>(GetParam())) != bst.end();
  ASSERT_TRUE(answer);
}

TEST_P(TestSuiteInt, NotFoundCase) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, PreOrder> bst{values.begin(), values.end(), std::less<int>()};

  bool answer = bst.find(std::get<5>(GetParam())) == bst.end();
  ASSERT_TRUE(answer);
}




TEST_P(TestSuiteInt, PreorderTest) {

  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, PreOrder> bst{values.begin(), values.end(), std::less<int>()};
  auto first = bst.begin();
  auto second = bst.end();

  std::vector<int> answer;

  while (first != second) {
    answer.push_back(*first);
    ++first;
  }

  auto expected  = std::get<1>(GetParam());

  ASSERT_EQ(answer, expected);
}

TEST_P(TestSuiteInt, InorderTest) {

  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, InOrder> bst{values.begin(), values.end(), std::less<int>()};
  auto first = bst.begin();
  auto second = bst.end();

  std::vector<int> answer;

  while (first != second) {
    answer.push_back(*first);
    ++first;
  }

  auto expected  = std::get<2>(GetParam());

  ASSERT_EQ(answer, expected);
}

TEST_P(TestSuiteInt, PostorderTest) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<int, PostOrder> bst{values.begin(), values.end(), std::less<int>()};
  auto first = bst.begin();
  auto second = bst.end();

  std::vector<int> answer;

  while (first != second) {
    answer.push_back(*first);
    ++first;
  }

  auto expected  = std::get<3>(GetParam());

  ASSERT_EQ(answer, expected);
}

INSTANTIATE_TEST_SUITE_P(
  Group,
  TestSuiteInt,
  testing::Values(
    std::make_tuple(std::vector<int>{2}, std::vector<int>{2}, std::vector<int>{2}, std::vector<int>{2}, 2, 0),
    std::make_tuple(std::vector<int>{2, 1, 3}, std::vector<int>{2, 1, 3}, std::vector<int>{1, 2, 3}, std::vector<int>{1, 3, 2}, 2, 0),
    std::make_tuple(std::vector<int>{100 , 8 , 9 , 11 , 19}, std::vector<int>{ 100, 8, 9, 11, 19 }, std::vector<int>{ 8, 9, 11, 19, 100 }, std::vector<int>{ 19, 11, 9, 8, 100 }, 8, 101),
    std::make_tuple(std::vector<int>{ 110, 84, 477, 35, 122, 318, 331, 289, 21, 346, 380, 70, 468, 46, 370, 322, 323, 408, 441, 410 },
                    std::vector<int>{ 110, 84, 35, 21, 70, 46, 477, 122, 318, 289, 331, 322, 323, 346, 380, 370, 468, 408, 441, 410 }, 
                    std::vector<int>{ 21, 35, 46, 70, 84, 110, 122, 289, 318, 322, 323, 331, 346, 370, 380, 408, 410, 441, 468, 477 }, 
                    std::vector<int>{ 21, 46, 70, 35, 84, 289, 323, 322, 370, 410, 441, 408, 468, 380, 346, 331, 318, 122, 477, 110 }, 
                    410, 
                    111),
    std::make_tuple(std::vector<int>{ 23, 40, 11, 58, 88, 1, 29, 67, 85, 18, 55, 63, 90, 4, 31, 90, 64, 36, 66, 11 }, 
                    std::vector<int>{ 23, 11, 1, 4, 18, 40, 29, 31, 36, 58, 55, 88, 67, 63, 64, 66, 85, 90 }, 
                    std::vector<int>{ 1, 4, 11, 18, 23, 29, 31, 36, 40, 55, 58, 63, 64, 66, 67, 85, 88, 90 }, 
                    std::vector<int>{ 4, 1, 18, 11, 36, 31, 29, 55, 66, 64, 63, 85, 67, 90, 88, 58, 40, 23 }, 
                    11, 
                    12),
    std::make_tuple(std::vector<int>{ 1450, 30, 5718, 1535, 1296, 2949, 1549, 3265, 7663, 488, 1300, 7730, 748, 8908, 7380, 7522, 8531, 6795, 5976, 4181, 2395, 4277, 5327, 7069, 8450, 8298, 2288, 10, 3568, 8920, 8994, 2734, 9638, 6729, 3847, 8621, 4099, 4946, 6958, 7412, 3206, 4961, 9224, 7096, 4093, 8207, 9082, 6699, 5864, 7707, 3369, 7973, 3920, 9373, 9631, 4530, 8377, 8699, 5443, 2693, 2068, 4702, 7139, 7657, 5053, 8448, 865, 1748, 5023, 2297, 4699, 5918, 7306, 8734, 9692, 519, 3523, 8709, 2868, 814, 2678, 8970, 5220, 449, 463, 5334, 760, 9186, 5503, 6583, 2266, 6614, 1664, 6451, 6933, 5479, 1356, 7701, 997, 6226 }, 
                    std::vector<int>{ 1450, 30, 10, 1296, 488, 449, 463, 748, 519, 865, 814, 760, 997, 1300, 1356, 5718, 1535, 2949, 1549, 2395, 2288, 2068, 1748, 1664, 2266, 2297, 2734, 2693, 2678, 2868, 3265, 3206, 4181, 3568, 3369, 3523, 3847, 4099, 4093, 3920, 4277, 5327, 4946, 4530, 4702, 4699, 4961, 5053, 5023, 5220, 5443, 5334, 5503, 5479, 7663, 7380, 6795, 5976, 5864, 5918, 6729, 6699, 6583, 6451, 6226, 6614, 7069, 6958, 6933, 7096, 7139, 7306, 7522, 7412, 7657, 7730, 7707, 7701, 8908, 8531, 8450, 8298, 8207, 7973, 8377, 8448, 8621, 8699, 8734, 8709, 8920, 8994, 8970, 9638, 9224, 9082, 9186, 9373, 9631, 9692 }, 
                    std::vector<int>{ 10, 30, 449, 463, 488, 519, 748, 760, 814, 865, 997, 1296, 1300, 1356, 1450, 1535, 1549, 1664, 1748, 2068, 2266, 2288, 2297, 2395, 2678, 2693, 2734, 2868, 2949, 3206, 3265, 3369, 3523, 3568, 3847, 3920, 4093, 4099, 4181, 4277, 4530, 4699, 4702, 4946, 4961, 5023, 5053, 5220, 5327, 5334, 5443, 5479, 5503, 5718, 5864, 5918, 5976, 6226, 6451, 6583, 6614, 6699, 6729, 6795, 6933, 6958, 7069, 7096, 7139, 7306, 7380, 7412, 7522, 7657, 7663, 7701, 7707, 7730, 7973, 8207, 8298, 8377, 8448, 8450, 8531, 8621, 8699, 8709, 8734, 8908, 8920, 8970, 8994, 9082, 9186, 9224, 9373, 9631, 9638, 9692 }, 
                    std::vector<int>{ 10, 463, 449, 519, 760, 814, 997, 865, 748, 488, 1356, 1300, 1296, 30, 1664, 1748, 2266, 2068, 2297, 2288, 2678, 2693, 2868, 2734, 2395, 1549, 3206, 3523, 3369, 3920, 4093, 4099, 3847, 3568, 4699, 4702, 4530, 5023, 5220, 5053, 4961, 4946, 5334, 5479, 5503, 5443, 5327, 4277, 4181, 3265, 2949, 1535, 5918, 5864, 6226, 6451, 6614, 6583, 6699, 6729, 5976, 6933, 6958, 7306, 7139, 7096, 7069, 6795, 7412, 7657, 7522, 7380, 7701, 7707, 7973, 8207, 8448, 8377, 8298, 8450, 8709, 8734, 8699, 8621, 8531, 8970, 9186, 9082, 9631, 9373, 9224, 9692, 9638, 8994, 8920, 8908, 7730, 7663, 5718, 1450 }, 
                    9638, 
                    9000),
    std::make_tuple(std::vector<int>{ 1, 1, 2, 1, 2 }, 
                    std::vector<int>{ 1, 2 }, 
                    std::vector<int>{ 1, 2 }, 
                    std::vector<int>{ 2, 1 }, 
                    1, 
                    0)
  )
);


class TestSuitePair : public ::testing::TestWithParam<std::tuple<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>, std::pair<int, int>, std::pair<int, int>>> {};

template<class T>
class ComparePair {
public:
bool operator()(std::pair<T, T> lhs, std::pair<T, T> rhs) {
  if (lhs.first < rhs.first) {
    return true;
  } else if (rhs.first < lhs.first) {
    return false;
  } else if (lhs.second < rhs.second) {
    return true;
  } 
  return false;
}
};

TEST_P(TestSuitePair, FoundCase) {
  auto values = std::get<0>(GetParam());

  ComparePair<int> comp;

  auto bst = BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>>(values.begin(), values.end(), comp);

  bool answer = bst.find(std::get<4>(GetParam())) != bst.end();
  ASSERT_TRUE(answer);
}

TEST_P(TestSuitePair, NotFoundCase) {
  auto values = std::get<0>(GetParam());
 
  auto bst = BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>>(values.begin(), values.end(), ComparePair<int>());

  bool answer = bst.find(std::get<5>(GetParam())) == bst.end();
  ASSERT_TRUE(answer);
}

TEST_P(TestSuitePair, EqualBst) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>> bst_l(values.begin(), values.end(), ComparePair<int>());
  BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>> bst_r(values.begin(), values.end(), ComparePair<int>());

  bool answer = (bst_l == bst_r);

  ASSERT_TRUE(answer);
}

TEST_P(TestSuitePair, NotEqualBst) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>> bst_l(values.begin(), values.end(), ComparePair<int>());
  BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>> bst_r(values.begin(), values.end(), ComparePair<int>());

  bst_l.erase(std::get<4>(GetParam()));

  bool answer = bst_l == bst_r; //different sizes of bst

  ASSERT_FALSE(answer);
}

TEST_P(TestSuitePair, HardNotEqualBst) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>> bst_l(values.begin(), values.end(), ComparePair<int>());
  BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>> bst_r(values.begin(), values.end(), ComparePair<int>());

  bst_l.erase(std::get<4>(GetParam()));
  bst_l.insert(std::get<5>(GetParam()));

  bool answer = bst_l == bst_r; //equal sizes, but different nodes

  ASSERT_FALSE(answer);
}

TEST_P(TestSuitePair, distance) {
  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>> bst(values.begin(), values.end(), ComparePair<int>());

  bool answer = (std::distance(bst.begin(), bst.end()) == bst.size());

  ASSERT_TRUE(answer);
}

TEST_P(TestSuitePair, PreorderTest) {

  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<std::pair<int, int>, PreOrder, ComparePair<int>> bst(values.begin(), values.end(), ComparePair<int>());
  auto first = bst.begin();
  auto second = bst.end();

  std::vector<std::pair<int, int>> answer;

  while (first != second) {
    answer.push_back(*first);
    ++first;
  }

  auto expected  = std::get<1>(GetParam());

  ASSERT_EQ(answer, expected);
}

TEST_P(TestSuitePair, InorderTest) {

  auto values = std::get<0>(GetParam());
 
  BinarySearchTree<std::pair<int, int>, InOrder, ComparePair<int>> bst(values.begin(), values.end(), ComparePair<int>());
  auto first = bst.begin();
  auto second = bst.end();

  std::vector<std::pair<int, int>> answer;

  while (first != second) {
    answer.push_back(*first);
    ++first;
  }

  auto expected  = std::get<2>(GetParam());

  ASSERT_EQ(answer, expected);
}

TEST_P(TestSuitePair, PostorderTest) {

  auto values = std::get<0>(GetParam());

  BinarySearchTree<std::pair<int, int>, PostOrder, ComparePair<int>> bst(values.begin(), values.end(), ComparePair<int>());
  auto first = bst.begin();
  auto second = bst.end();

  std::vector<std::pair<int, int>> answer;

  while (first != second) {
    answer.push_back(*first);
    ++first;
  }

  auto expected  = std::get<3>(GetParam());

  ASSERT_EQ(answer, expected);
}

INSTANTIATE_TEST_SUITE_P(
  UnusualCompare,
  TestSuitePair,
  testing::Values(
    make_tuple(std::vector<std::pair<int, int>>{ std::pair(1, 2)}, std::vector<std::pair<int, int>>{std::pair(1, 2)}, std::vector<std::pair<int, int>>{std::pair(1, 2)}, std::vector<std::pair<int, int>>{std::pair(1, 2)}, std::pair(1, 2), std::pair(1, 3)),
    make_tuple(std::vector<std::pair<int, int>>{ std::pair<int, int>(4, 6), std::pair<int, int>(6, 3), std::pair<int, int>(4, 4), std::pair<int, int>(7, 9), std::pair<int, int>(4, 2), std::pair<int, int>(3, 2), std::pair<int, int>(7, 9), std::pair<int, int>(2, 3), std::pair<int, int>(2, 0), std::pair<int, int>(3, 0), }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(4, 6), std::pair<int, int>(4, 4), std::pair<int, int>(4, 2), std::pair<int, int>(3, 2), std::pair<int, int>(2, 3), std::pair<int, int>(2, 0), std::pair<int, int>(3, 0), std::pair<int, int>(6, 3), std::pair<int, int>(7, 9) }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(2, 0), std::pair<int, int>(2, 3), std::pair<int, int>(3, 0), std::pair<int, int>(3, 2), std::pair<int, int>(4, 2), std::pair<int, int>(4, 4), std::pair<int, int>(4, 6), std::pair<int, int>(6, 3), std::pair<int, int>(7, 9) }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(2, 0), std::pair<int, int>(3, 0), std::pair<int, int>(2, 3), std::pair<int, int>(3, 2), std::pair<int, int>(4, 2), std::pair<int, int>(4, 4), std::pair<int, int>(7, 9), std::pair<int, int>(6, 3), std::pair<int, int>(4, 6) }, 
               std::pair<int, int>(7, 9), 
               std::pair<int, int>(7, 0)),
    make_tuple(std::vector<std::pair<int, int>>{ std::pair<int, int>(2, 7), std::pair<int, int>(8, 8), std::pair<int, int>(9, 4), std::pair<int, int>(9, 3), std::pair<int, int>(8, 0), std::pair<int, int>(5, 6), std::pair<int, int>(5, 5), std::pair<int, int>(7, 7), std::pair<int, int>(6, 1), std::pair<int, int>(1, 3), std::pair<int, int>(7, 4), std::pair<int, int>(8, 8), std::pair<int, int>(1, 4), std::pair<int, int>(4, 2), std::pair<int, int>(3, 4), std::pair<int, int>(9, 6), std::pair<int, int>(2, 9), std::pair<int, int>(2, 8), std::pair<int, int>(6, 8), std::pair<int, int>(10, 0), }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(2, 7), std::pair<int, int>(1, 3), std::pair<int, int>(1, 4), std::pair<int, int>(8, 8), std::pair<int, int>(8, 0), std::pair<int, int>(5, 6), std::pair<int, int>(5, 5), std::pair<int, int>(4, 2), std::pair<int, int>(3, 4), std::pair<int, int>(2, 9), std::pair<int, int>(2, 8), std::pair<int, int>(7, 7), std::pair<int, int>(6, 1), std::pair<int, int>(7, 4), std::pair<int, int>(6, 8), std::pair<int, int>(9, 4), std::pair<int, int>(9, 3), std::pair<int, int>(9, 6), std::pair<int, int>(10, 0) }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(1, 3), std::pair<int, int>(1, 4), std::pair<int, int>(2, 7), std::pair<int, int>(2, 8), std::pair<int, int>(2, 9), std::pair<int, int>(3, 4), std::pair<int, int>(4, 2), std::pair<int, int>(5, 5), std::pair<int, int>(5, 6), std::pair<int, int>(6, 1), std::pair<int, int>(6, 8), std::pair<int, int>(7, 4), std::pair<int, int>(7, 7), std::pair<int, int>(8, 0), std::pair<int, int>(8, 8), std::pair<int, int>(9, 3), std::pair<int, int>(9, 4), std::pair<int, int>(9, 6), std::pair<int, int>(10, 0) }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(1, 4), std::pair<int, int>(1, 3), std::pair<int, int>(2, 8), std::pair<int, int>(2, 9), std::pair<int, int>(3, 4), std::pair<int, int>(4, 2), std::pair<int, int>(5, 5), std::pair<int, int>(6, 8), std::pair<int, int>(7, 4), std::pair<int, int>(6, 1), std::pair<int, int>(7, 7), std::pair<int, int>(5, 6), std::pair<int, int>(8, 0), std::pair<int, int>(9, 3), std::pair<int, int>(10, 0), std::pair<int, int>(9, 6), std::pair<int, int>(9, 4), std::pair<int, int>(8, 8), std::pair<int, int>(2, 7) }, 
               std::pair<int, int>(8, 8), 
               std::pair<int, int>(8, 9)),
    make_tuple(std::vector<std::pair<int, int>>{ std::pair<int, int>(3, 7), std::pair<int, int>(8, 1), std::pair<int, int>(5, 3), std::pair<int, int>(4, 0), std::pair<int, int>(5, 2), std::pair<int, int>(3, 3), std::pair<int, int>(2, 3), std::pair<int, int>(8, 3), std::pair<int, int>(9, 2), std::pair<int, int>(9, 6), std::pair<int, int>(8, 6), std::pair<int, int>(9, 4), std::pair<int, int>(1, 9), std::pair<int, int>(6, 3), std::pair<int, int>(5, 5), std::pair<int, int>(4, 8), std::pair<int, int>(9, 3), std::pair<int, int>(7, 7), std::pair<int, int>(2, 4), std::pair<int, int>(5, 6), std::pair<int, int>(2, 2), std::pair<int, int>(4, 5), std::pair<int, int>(2, 7), std::pair<int, int>(8, 9), std::pair<int, int>(7, 4), std::pair<int, int>(3, 2), std::pair<int, int>(8, 2), std::pair<int, int>(4, 6), std::pair<int, int>(4, 9), std::pair<int, int>(5, 1), std::pair<int, int>(8, 0), std::pair<int, int>(1, 3), std::pair<int, int>(3, 8), std::pair<int, int>(4, 4), std::pair<int, int>(9, 5), std::pair<int, int>(1, 1), std::pair<int, int>(7, 2), std::pair<int, int>(1, 7), std::pair<int, int>(6, 5), std::pair<int, int>(4, 2), std::pair<int, int>(5, 9), std::pair<int, int>(6, 0), std::pair<int, int>(2, 1), std::pair<int, int>(5, 8), std::pair<int, int>(4, 1), std::pair<int, int>(1, 4), std::pair<int, int>(3, 4), std::pair<int, int>(7, 1), std::pair<int, int>(8, 4), std::pair<int, int>(7, 5), std::pair<int, int>(3, 1), std::pair<int, int>(5, 4), std::pair<int, int>(6, 6), std::pair<int, int>(3, 5), std::pair<int, int>(1, 0), std::pair<int, int>(2, 0), std::pair<int, int>(4, 7), std::pair<int, int>(3, 0), std::pair<int, int>(1, 5), std::pair<int, int>(4, 3), std::pair<int, int>(7, 6), std::pair<int, int>(8, 7), std::pair<int, int>(6, 1), std::pair<int, int>(2, 8), std::pair<int, int>(2, 5), std::pair<int, int>(9, 0), std::pair<int, int>(9, 8), std::pair<int, int>(1, 6), std::pair<int, int>(1, 8), std::pair<int, int>(8, 5), std::pair<int, int>(5, 7), std::pair<int, int>(3, 9), std::pair<int, int>(2, 9), std::pair<int, int>(10, 0), std::pair<int, int>(3, 6), std::pair<int, int>(6, 8), std::pair<int, int>(7, 3), std::pair<int, int>(9, 7), std::pair<int, int>(9, 9), std::pair<int, int>(6, 9), std::pair<int, int>(2, 6), std::pair<int, int>(7, 0), std::pair<int, int>(5, 0), std::pair<int, int>(7, 8), std::pair<int, int>(7, 9), std::pair<int, int>(6, 2), std::pair<int, int>(6, 7), std::pair<int, int>(6, 4), std::pair<int, int>(1, 2), std::pair<int, int>(8, 8) }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(3, 7), std::pair<int, int>(3, 3), std::pair<int, int>(2, 3), std::pair<int, int>(1, 9), std::pair<int, int>(1, 3), std::pair<int, int>(1, 1), std::pair<int, int>(1, 0), std::pair<int, int>(1, 2), std::pair<int, int>(1, 7), std::pair<int, int>(1, 4), std::pair<int, int>(1, 5), std::pair<int, int>(1, 6), std::pair<int, int>(1, 8), std::pair<int, int>(2, 2), std::pair<int, int>(2, 1), std::pair<int, int>(2, 0), std::pair<int, int>(2, 4), std::pair<int, int>(2, 7), std::pair<int, int>(2, 5), std::pair<int, int>(2, 6), std::pair<int, int>(3, 2), std::pair<int, int>(3, 1), std::pair<int, int>(3, 0), std::pair<int, int>(2, 8), std::pair<int, int>(2, 9), std::pair<int, int>(3, 4), std::pair<int, int>(3, 5), std::pair<int, int>(3, 6), std::pair<int, int>(8, 1), std::pair<int, int>(5, 3), std::pair<int, int>(4, 0), std::pair<int, int>(3, 8), std::pair<int, int>(3, 9), std::pair<int, int>(5, 2), std::pair<int, int>(4, 8), std::pair<int, int>(4, 5), std::pair<int, int>(4, 4), std::pair<int, int>(4, 2), std::pair<int, int>(4, 1), std::pair<int, int>(4, 3), std::pair<int, int>(4, 6), std::pair<int, int>(4, 7), std::pair<int, int>(4, 9), std::pair<int, int>(5, 1), std::pair<int, int>(5, 0), std::pair<int, int>(6, 3), std::pair<int, int>(5, 5), std::pair<int, int>(5, 4), std::pair<int, int>(5, 6), std::pair<int, int>(5, 9), std::pair<int, int>(5, 8), std::pair<int, int>(5, 7), std::pair<int, int>(6, 0), std::pair<int, int>(6, 1), std::pair<int, int>(6, 2), std::pair<int, int>(7, 7), std::pair<int, int>(7, 4), std::pair<int, int>(7, 2), std::pair<int, int>(6, 5), std::pair<int, int>(6, 4), std::pair<int, int>(7, 1), std::pair<int, int>(6, 6), std::pair<int, int>(6, 8), std::pair<int, int>(6, 7), std::pair<int, int>(6, 9), std::pair<int, int>(7, 0), std::pair<int, int>(7, 3), std::pair<int, int>(7, 5), std::pair<int, int>(7, 6), std::pair<int, int>(8, 0), std::pair<int, int>(7, 8), std::pair<int, int>(7, 9), std::pair<int, int>(8, 3), std::pair<int, int>(8, 2), std::pair<int, int>(9, 2), std::pair<int, int>(8, 6), std::pair<int, int>(8, 4), std::pair<int, int>(8, 5), std::pair<int, int>(8, 9), std::pair<int, int>(8, 7), std::pair<int, int>(8, 8), std::pair<int, int>(9, 0), std::pair<int, int>(9, 6), std::pair<int, int>(9, 4), std::pair<int, int>(9, 3), std::pair<int, int>(9, 5), std::pair<int, int>(9, 8), std::pair<int, int>(9, 7), std::pair<int, int>(10, 0), std::pair<int, int>(9, 9) }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(1, 0), std::pair<int, int>(1, 1), std::pair<int, int>(1, 2), std::pair<int, int>(1, 3), std::pair<int, int>(1, 4), std::pair<int, int>(1, 5), std::pair<int, int>(1, 6), std::pair<int, int>(1, 7), std::pair<int, int>(1, 8), std::pair<int, int>(1, 9), std::pair<int, int>(2, 0), std::pair<int, int>(2, 1), std::pair<int, int>(2, 2), std::pair<int, int>(2, 3), std::pair<int, int>(2, 4), std::pair<int, int>(2, 5), std::pair<int, int>(2, 6), std::pair<int, int>(2, 7), std::pair<int, int>(2, 8), std::pair<int, int>(2, 9), std::pair<int, int>(3, 0), std::pair<int, int>(3, 1), std::pair<int, int>(3, 2), std::pair<int, int>(3, 3), std::pair<int, int>(3, 4), std::pair<int, int>(3, 5), std::pair<int, int>(3, 6), std::pair<int, int>(3, 7), std::pair<int, int>(3, 8), std::pair<int, int>(3, 9), std::pair<int, int>(4, 0), std::pair<int, int>(4, 1), std::pair<int, int>(4, 2), std::pair<int, int>(4, 3), std::pair<int, int>(4, 4), std::pair<int, int>(4, 5), std::pair<int, int>(4, 6), std::pair<int, int>(4, 7), std::pair<int, int>(4, 8), std::pair<int, int>(4, 9), std::pair<int, int>(5, 0), std::pair<int, int>(5, 1), std::pair<int, int>(5, 2), std::pair<int, int>(5, 3), std::pair<int, int>(5, 4), std::pair<int, int>(5, 5), std::pair<int, int>(5, 6), std::pair<int, int>(5, 7), std::pair<int, int>(5, 8), std::pair<int, int>(5, 9), std::pair<int, int>(6, 0), std::pair<int, int>(6, 1), std::pair<int, int>(6, 2), std::pair<int, int>(6, 3), std::pair<int, int>(6, 4), std::pair<int, int>(6, 5), std::pair<int, int>(6, 6), std::pair<int, int>(6, 7), std::pair<int, int>(6, 8), std::pair<int, int>(6, 9), std::pair<int, int>(7, 0), std::pair<int, int>(7, 1), std::pair<int, int>(7, 2), std::pair<int, int>(7, 3), std::pair<int, int>(7, 4), std::pair<int, int>(7, 5), std::pair<int, int>(7, 6), std::pair<int, int>(7, 7), std::pair<int, int>(7, 8), std::pair<int, int>(7, 9), std::pair<int, int>(8, 0), std::pair<int, int>(8, 1), std::pair<int, int>(8, 2), std::pair<int, int>(8, 3), std::pair<int, int>(8, 4), std::pair<int, int>(8, 5), std::pair<int, int>(8, 6), std::pair<int, int>(8, 7), std::pair<int, int>(8, 8), std::pair<int, int>(8, 9), std::pair<int, int>(9, 0), std::pair<int, int>(9, 2), std::pair<int, int>(9, 3), std::pair<int, int>(9, 4), std::pair<int, int>(9, 5), std::pair<int, int>(9, 6), std::pair<int, int>(9, 7), std::pair<int, int>(9, 8), std::pair<int, int>(9, 9), std::pair<int, int>(10, 0) }, 
               std::vector<std::pair<int, int>>{ std::pair<int, int>(1, 0), std::pair<int, int>(1, 2), std::pair<int, int>(1, 1), std::pair<int, int>(1, 6), std::pair<int, int>(1, 5), std::pair<int, int>(1, 4), std::pair<int, int>(1, 8), std::pair<int, int>(1, 7), std::pair<int, int>(1, 3), std::pair<int, int>(2, 0), std::pair<int, int>(2, 1), std::pair<int, int>(2, 2), std::pair<int, int>(1, 9), std::pair<int, int>(2, 6), std::pair<int, int>(2, 5), std::pair<int, int>(2, 9), std::pair<int, int>(2, 8), std::pair<int, int>(3, 0), std::pair<int, int>(3, 1), std::pair<int, int>(3, 2), std::pair<int, int>(2, 7), std::pair<int, int>(2, 4), std::pair<int, int>(2, 3), std::pair<int, int>(3, 6), std::pair<int, int>(3, 5), std::pair<int, int>(3, 4), std::pair<int, int>(3, 3), std::pair<int, int>(3, 9), std::pair<int, int>(3, 8), std::pair<int, int>(4, 1), std::pair<int, int>(4, 3), std::pair<int, int>(4, 2), std::pair<int, int>(4, 4), std::pair<int, int>(4, 7), std::pair<int, int>(4, 6), std::pair<int, int>(4, 5), std::pair<int, int>(5, 0), std::pair<int, int>(5, 1), std::pair<int, int>(4, 9), std::pair<int, int>(4, 8), std::pair<int, int>(5, 2), std::pair<int, int>(4, 0), std::pair<int, int>(5, 4), std::pair<int, int>(5, 7), std::pair<int, int>(5, 8), std::pair<int, int>(6, 2), std::pair<int, int>(6, 1), std::pair<int, int>(6, 0), std::pair<int, int>(5, 9), std::pair<int, int>(5, 6), std::pair<int, int>(5, 5), std::pair<int, int>(6, 4), std::pair<int, int>(6, 7), std::pair<int, int>(7, 0), std::pair<int, int>(6, 9), std::pair<int, int>(6, 8), std::pair<int, int>(6, 6), std::pair<int, int>(7, 1), std::pair<int, int>(6, 5), std::pair<int, int>(7, 3), std::pair<int, int>(7, 2), std::pair<int, int>(7, 6), std::pair<int, int>(7, 5), std::pair<int, int>(7, 4), std::pair<int, int>(7, 9), std::pair<int, int>(7, 8), std::pair<int, int>(8, 0), std::pair<int, int>(7, 7), std::pair<int, int>(6, 3), std::pair<int, int>(5, 3), std::pair<int, int>(8, 2), std::pair<int, int>(8, 5), std::pair<int, int>(8, 4), std::pair<int, int>(8, 8), std::pair<int, int>(8, 7), std::pair<int, int>(9, 0), std::pair<int, int>(8, 9), std::pair<int, int>(8, 6), std::pair<int, int>(9, 3), std::pair<int, int>(9, 5), std::pair<int, int>(9, 4), std::pair<int, int>(9, 7), std::pair<int, int>(9, 9), std::pair<int, int>(10, 0), std::pair<int, int>(9, 8), std::pair<int, int>(9, 6), std::pair<int, int>(9, 2), std::pair<int, int>(8, 3), std::pair<int, int>(8, 1), std::pair<int, int>(3, 7) }, 
               std::pair<int, int>(4, 0), 
               std::pair<int, int>(0, 0))
  )
);
