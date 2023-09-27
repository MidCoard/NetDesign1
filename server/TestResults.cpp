#include "TestResults.h"

TestResults::TestResults(unsigned int totalTests) {
	results.resize(totalTests);
}

void TestResults::push(unsigned int testIndex, const TestResult &result) {
	results[testIndex].push_back(result);
}

TestResult TestResults::get(unsigned int testIndex, unsigned int resultIndex) {
	return results[testIndex][resultIndex];
}
