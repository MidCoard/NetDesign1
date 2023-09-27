#include "TestResult.h"

TestResult::TestResult(tr::TestResultType type, std::chrono::microseconds duration) : type(type), duration(duration) {}
