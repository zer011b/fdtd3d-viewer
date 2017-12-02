#ifndef ASSERT_H
#define ASSERT_H

#include <cassert>

#ifdef ENABLE_ASSERTS
#define ASSERT(x) \
  { \
    assert (x); \
  }
#else
#define ASSERT(x)
#endif

#define UNREACHABLE \
  { \
    ASSERT (0); \
  }

#ifdef FLOAT_VALUES
#define FPValue float
#define FPMOD "%f"
#endif
#ifdef DOUBLE_VALUES
#define FPValue double
#define FPMOD "%lf"
#endif
#ifdef LONG_DOUBLE_VALUES
#define FPValue long double
#define FPMOD "%llf"
#endif

#define STOI(str) std::stoi (str)
#define STOF(str) std::stof (str)

#endif /* !ASSERT_H */
