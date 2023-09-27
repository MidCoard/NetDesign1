#ifndef NETDESIGN1_TESTRESULT_H
#define NETDESIGN1_TESTRESULT_H

#include "chrono"

namespace tr {

	typedef enum {
		WRITE_FAILED,
		READ_FAILED,
		DATA_MISMATCH,
		NO_ERROR,
	} TestResultType;
}

class TestResult {

private:

	tr::TestResultType type;

	std::chrono::microseconds duration;

public:

	TestResult(tr::TestResultType type, std::chrono::microseconds duration);
};


#endif //NETDESIGN1_TESTRESULT_H
