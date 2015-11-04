#ifndef TEST_LIB
#define TEST_LIB
#include <chrono>

namespace test {
  class clock {
    typedef std::chrono::high_resolution_clock hsc;
    typedef hsc::time_point tp;
  public:
    void start();
    void stop();
    long long count();
  private:
    tp s,e;
  };

  void clock::start() {
    s = hsc::now();
  }

  void clock::stop() {
    e = hsc::now();
  }

  long long clock::count() {
    using namespace std;
    using namespace std::chrono;
    return duration_cast<milliseconds>(e-s).count();
  }

};


#endif
