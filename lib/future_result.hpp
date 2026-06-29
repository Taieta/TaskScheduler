#include "subtask.hpp"

template <typename T>
class FutureResult {
private:
  SubTask* dependence_; 

public:
  FutureResult(SubTask* dependence) 
  : dependence_(dependence)
  {};

  operator const T&() const {
    const T* temp = reinterpret_cast<const T*>(dependence_->getResult());
    return static_cast<const T&>(*temp);
  }
};
