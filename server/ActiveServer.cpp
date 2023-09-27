#include "ActiveServer.h"

ActiveServer::ActiveServer(const TestConfig& testConfig) : testConfig(testConfig) {
	this->internal = socket(AF_INET, tc::convertNetworkType(testConfig.getTestNetworkType()), 0);
	if (this->internal == -1)
		throw std::runtime_error("Failed to create socket at port " + std::to_string(testConfig.getSourcePort()));
	this->buffer = new unsigned char[testConfig.getCustomDataLength()];
	struct sockaddr_in addr {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(testConfig.getSourcePort());
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->internal, (struct sockaddr *) &addr, sizeof(addr)) == -1)
		throw std::runtime_error("Failed to bind socket at port " + std::to_string(testConfig.getSourcePort()));
}

TestResults ActiveServer::test() {
	this->canClose = false;
	TestResults results = TestResults(testConfig.getTotalTestCount());
	for (int i = 0; i < this->testConfig.getTotalTestCount(); i++) {
		for (int j = 0; j < this->testConfig.getSingleTestCount(); j++) {
			struct sockaddr_in addr{};
			addr.sin_family = AF_INET;
			addr.sin_port = htons(testConfig.getDestinationPort());
			addr.sin_addr.s_addr = inet_addr(testConfig.getDestinationAddress().data());
			if (connect(this->internal, (struct sockaddr *) &addr, sizeof(addr)) == -1)
				continue;
			auto now = std::chrono::high_resolution_clock::now();
			if (write(this->internal, this->testConfig.getCustomData(), this->testConfig.getCustomDataLength()) == -1) {
				TestResult result = TestResult(tr::TestResultType::WRITE_FAILED, std::chrono::microseconds(0));
				results.push(i, result);
				continue;
			}
			if (read(this->internal, this->buffer, this->testConfig.getCustomDataLength()) == -1) {
				TestResult result = TestResult(tr::TestResultType::READ_FAILED, std::chrono::microseconds(0));
				results.push(i, result);
				continue;
			}
			auto then = std::chrono::high_resolution_clock::now();
			if (memcmp(this->testConfig.getCustomData(), this->buffer, this->testConfig.getCustomDataLength()) != 0) {
				TestResult result = TestResult(tr::TestResultType::DATA_MISMATCH, std::chrono::microseconds(0));
				results.push(i, result);
				continue;
			}
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(then - now);
			TestResult result = TestResult(tr::TestResultType::NO_ERROR, duration);
			std::this_thread::sleep_for(testConfig.getSingleTestInterval());
		}
		std::this_thread::sleep_for(testConfig.getTotalTestInterval());
	}
	this->canClose = true;
	return results;
}

void ActiveServer::close() const {
	if (!canClose)
		return;
	::close(this->internal);
}

ActiveServer::~ActiveServer() {
	delete[] this->buffer;
}


