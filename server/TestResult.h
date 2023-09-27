#ifndef NETDESIGN1_TESTRESULT_H
#define NETDESIGN1_TESTRESULT_H
#include "iostream"
#include "chrono"

namespace tr {

	typedef enum {
		CONNECT_FAILED,
		WRITE_FAILED,
		READ_FAILED,
		DATA_MISMATCH,
		NO_ERROR,
	} TestResultType;
}

class TestResult {

public:

	TestResult(tr::TestResultType type, std::chrono::microseconds duration);

	tr::TestResultType type;
	std::chrono::microseconds duration;
};


std::ostream &operator<<(std::ostream &os, TestResult const &testResult);


#endif //NETDESIGN1_TESTRESULT_H
