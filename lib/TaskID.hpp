#include<iostream>

class TaskID {
private:
  size_t n_;
public:
  TaskID(int n)
  :n_(n)
  {};

  size_t getID() {
    return n_;
  }

};
