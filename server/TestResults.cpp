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

json TestResults::toJson() {
	json ret = json::array();
	for (auto &result : results) {
		json once = json::array();
		for (auto &test : result) {
			json testResult = json::object();
			testResult["type"] = test.getTypeAsString();
			testResult["duration"] = test.duration.count();
			once.push_back(testResult);
		}
		ret.push_back(once);
	}
	return ret;
}

json TestResults::toJsonWithDuration() {
	json ret = json::array();
	for (auto &result : results) {
		json once = json::array();
		for (auto &test : result) {
			once.push_back(test.duration.count());
		}
		ret.push_back(once);
	}
	return ret;
}


