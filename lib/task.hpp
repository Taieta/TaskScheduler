#include "future_result.hpp"
#include "TaskID.hpp"
#include <vector>

class TTaskScheduler {
private:
  std::vector<SubTask*> Tasks;

public:
  template <typename function>
  TaskID add(function func) {
    SubTask* new_task = new EmptySubTask<function>(func);
    Tasks.push_back(new_task);
    return TaskID(Tasks.size() - 1);
  }

  template <typename function, typename T>
  TaskID add(function func, T arg) {
    SubTask* new_task = new UnarySubTask<function, T>(func, arg);
    Tasks.push_back(new_task);
    return TaskID(Tasks.size() - 1);
  }

  template <typename function, typename T, typename U>
  TaskID add(function func, T arg1, U arg2) {
    SubTask* new_task = new BinarySubTask<function, T, U>(func, arg1, arg2);
    Tasks.push_back(new_task);
    return TaskID(Tasks.size() - 1);
  }

  template <typename T>
  FutureResult<T> getFutureResult(TaskID dependence) {
    return FutureResult<T>(Tasks[dependence.getID()]);
  }

  template <typename T>
  FutureResult<T> getResult(TaskID task) {
    return FutureResult<T>(Tasks[task.getID()]);
  }

  void executeAll() {
    for (size_t i = 0; i < Tasks.size(); ++i) {
      Tasks[i]->execute();
    }
  }
};
