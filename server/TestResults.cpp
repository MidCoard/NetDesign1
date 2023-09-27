#include "TestResults.h"

TestResults::TestResults(unsigned int totalTests) {
	results.resize(totalTests);
}

void TestResults::push(unsigned int testIndex, const TestResult &result) {
	results[testIndex].push_back(result);
}
