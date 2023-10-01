#include "TestResult.h"

static const char *testResultTypeStrings[] = {
		"WRITE_FAILED",
		"READ_FAILED",
		"DATA_MISMATCH",
		"NO_ERROR",
};

TestResult::TestResult(tr::TestResultType type, std::chrono::microseconds duration) : type(type), duration(duration) {}


std::ostream &operator<<(std::ostream &os, TestResult const &testResult) {
	os << "TestResult: type=" << testResultTypeStrings[testResult.type] << " duration=" << testResult.duration.count() << "us";
	return os;
}
