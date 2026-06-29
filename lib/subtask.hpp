class SubTask {
public:
  virtual void execute() = 0;
  virtual void* getResult() = 0;
};

template <typename function>
class EmptySubTask : public SubTask {
private:
  bool executed = false;
  function func_;
  using return_type = decltype(func_());
  return_type answer;
  return_type* answerp = &answer;

public:
  EmptySubTask(function func)
  : func_(func)
  {};

  void execute() {
    executed = true;
    answer = func_();
  }

  void* getResult() {
    if (!executed) {
      execute();
    }
    return static_cast<void*>(answerp);
  }

};

template <typename function, typename argument>
class UnarySubTask : public SubTask {
private:
  bool executed = false;
  function func_;
  argument arg_;
  using return_type = decltype(func_(arg_));
  return_type answer;
  return_type* answerp = &answer;

public:
  UnarySubTask(function func, argument arg)
  : func_(func)
  , arg_(arg)
  {};

  void execute() {
    executed = true;
    answer = func_(arg_);
  }

  void* getResult() {
    if (!executed) {
      execute();
    }
    return static_cast<void*>(answerp);
  }
};

template <typename function, typename argument1, typename argument2>
class BinarySubTask : public SubTask {
private:
  bool executed = false;
  function func_;
  argument1 arg1_;
  argument2 arg2_;
  using return_type = decltype(func_(arg1_, arg2_));
  return_type answer;
  return_type* answerp = &answer;

public:
  BinarySubTask(function func, argument1 arg1, argument2 arg2)
  : func_(func)
  , arg1_(arg1)
  , arg2_(arg2)
  {};

  void execute() {
    executed = true;
    answer = func_(arg1_, arg2_);
  }

  void* getResult() {
    if (!executed) {
      execute();
    }
    return static_cast<void*>(answerp);
  }
};
