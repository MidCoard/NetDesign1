#ifndef NETDESIGN1_TESTRESULTS_H
#define NETDESIGN1_TESTRESULTS_H

#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "TestResult.h"

using json = nlohmann::json;

class TestResults {

private:

	std::vector<std::vector<TestResult>> results;

public:

	explicit TestResults(unsigned int totalTests);

	void push(unsigned int testIndex, const TestResult &result);

	TestResult get(unsigned int testIndex, unsigned int resultIndex);

	json toJson();

	json toJsonWithDuration();
};


#endif //NETDESIGN1_TESTRESULTS_H
