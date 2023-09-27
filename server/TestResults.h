#ifndef NETDESIGN1_TESTRESULTS_H
#define NETDESIGN1_TESTRESULTS_H

#include "vector"
#include "TestResult.h"

class TestResults {

private:

	std::vector<std::vector<TestResult>> results;

public:

	explicit TestResults(unsigned int totalTests);

	void push(unsigned int testIndex, const TestResult &result);

	TestResult get(unsigned int testIndex, unsigned int resultIndex);
};


#endif //NETDESIGN1_TESTRESULTS_H
