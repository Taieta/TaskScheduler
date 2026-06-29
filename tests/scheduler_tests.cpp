#include <vector>
#include <gtest/gtest.h>
#include "lib/task.hpp"
#include <random>

std::vector<int32_t> GetRandomNumbers(size_t n) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type>
      dist(0, std::numeric_limits<uint32_t>::max());
  std::vector<int32_t> result(n);

  for (int32_t& element : result) {
    element = static_cast<int32_t>(dist(rng));
  }

  return result;
}

TEST(SchedulerUnitTestSuite, SchedulerTest1) {
  float a = 1;
  float b = -2;
  float c = 0;

  TTaskScheduler scheduler;

  auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);
  auto fr1 = scheduler.getFutureResult<float>(id1);

  auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, fr1);

  auto id3 =
      scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

  auto id4 =
      scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

  auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));

  auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<float>(id1), 0);
  ASSERT_EQ(scheduler.getResult<float>(id2), 4);
  ASSERT_EQ(scheduler.getResult<float>(id3), 4);
  ASSERT_EQ(scheduler.getResult<float>(id4), 0);
  ASSERT_EQ(scheduler.getResult<float>(id5), 2);
  ASSERT_EQ(scheduler.getResult<float>(id6), 0);
}

TEST(SchedulerUnitTestSuite, SchedulerTest2) {
  float a = 1;
  float b = -2;
  float c = 0;

  TTaskScheduler scheduler;

  auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);
  auto fr1 = scheduler.getFutureResult<float>(id1);

  auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, fr1);

  auto id3 =
      scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

  auto id4 =
      scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));

  auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));

  auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

  ASSERT_EQ(scheduler.getResult<float>(id4), 0);
  ASSERT_EQ(scheduler.getResult<float>(id5), 2);
  ASSERT_EQ(scheduler.getResult<float>(id2), 4);
  ASSERT_EQ(scheduler.getResult<float>(id6), 0);
  ASSERT_EQ(scheduler.getResult<float>(id3), 4);
  ASSERT_EQ(scheduler.getResult<float>(id1), 0);
}

TEST(SchedulerUnitTestSuite, SchedulerTest3) {
  size_t index = 228;
  size_t size = 1000;
  std::vector<int32_t> values_ = GetRandomNumbers(size);

  std::function<std::string(const int32_t&)> to_string = [&](const int32_t& value) -> std::string {
    return std::to_string(value);
  };

  TTaskScheduler scheduler;

  auto id1 =
      scheduler.add([](std::vector<int32_t>& vector, size_t index) { return vector[index]; }, values_, index);
  auto fr1 = scheduler.getFutureResult<int>(id1);

  auto id2 = scheduler.add(to_string, fr1);

  auto id3 = scheduler.add([](const std::string& a, const std::string& b) -> bool {
    return a == b;
  }, scheduler.getFutureResult<std::string>(id2), std::to_string(values_[index - 1]));

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<int>(id1), values_[index]);
  ASSERT_EQ(scheduler.getResult<std::string>(id2), std::to_string(values_[index]));
  ASSERT_FALSE(scheduler.getResult<bool>(id3));
}

TEST(SchedulerUnitTestSuite, SchedulerTest4) {
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([](int a) { return a; }, 1312);
  auto id2 = scheduler.add([](float a) { return a; }, 13.12);
  scheduler.executeAll();
  ASSERT_EQ(scheduler.getResult<int>(id1), 1312);
  ASSERT_LE(scheduler.getResult<float>(id2) - 13.12, 0.00001);
}
