#include "TestResult.h"

static const char *testResultTypeStrings[] = {
		"WRITE_FAILED",
		"READ_FAILED",
		"DATA_MISMATCH",
		"CONNECT_FAILED",
		"NO_ERROR",
};

TestResult::TestResult(tr::TestResultType type, std::chrono::microseconds duration) : type(type), duration(duration) {}

std::string TestResult::getTypeAsString() const {
	return testResultTypeStrings[type];
}

std::ostream &operator<<(std::ostream &os, TestResult const &testResult) {
	os << "TestResult: type=" << testResultTypeStrings[testResult.type] << " duration=" << testResult.duration.count() << "us";
	return os;
}
