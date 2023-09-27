#include "TestResult.h"

TestResult::TestResult(tr::TestResultType type, std::chrono::microseconds duration) : type(type), duration(duration) {}

std::ostream &operator<<(std::ostream &os, TestResult const &testResult) {
	os << "TestResult: type=" << testResult.type << " duration=" << testResult.duration.count() << "us";
	return os;
}
