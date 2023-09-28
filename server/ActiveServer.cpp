#include "ActiveServer.h"

ActiveServer::ActiveServer(TestConfig* testConfig) : testConfig(testConfig) {
	this->buffer = new unsigned char[testConfig->getCustomDataLength()];
	this->client = socket(AF_INET, tc::convertNetworkType(testConfig->getTestNetworkType()), 0);
	if (client == -1)
		throw std::runtime_error("Failed to create socket at port " + std::to_string(testConfig->getSourcePort()));
	int option = 1;
	if (setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
		throw std::runtime_error("Failed to set socket options at port " + std::to_string(testConfig->getSourcePort()));
	if (setsockopt(client, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) == -1)
		throw std::runtime_error("Failed to set socket options at port " + std::to_string(testConfig->getSourcePort()));
	if (testConfig->getSourcePort() != 0) {
		struct sockaddr_in clientAddr{};
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_port = htons(testConfig->getSourcePort());
		clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(client, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == -1)
			throw std::runtime_error("Failed to bind socket at port " + std::to_string(testConfig->getSourcePort()));
	}
	if (testConfig->getTestNetworkType() == tc::TestNetworkType::TCP) {
		struct sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(testConfig->getDestinationPort());
		addr.sin_addr.s_addr = inet_addr(testConfig->getDestinationAddress().data());
		if (connect(client, (struct sockaddr *) &addr, sizeof(addr)) == -1)
			throw std::runtime_error("Failed to connect to " + std::string(testConfig->getDestinationAddress()) + ":" +
			                         std::to_string(testConfig->getDestinationPort()));
	}
}

TestResults ActiveServer::test() {
	struct sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(testConfig->getDestinationPort());
	serverAddr.sin_addr.s_addr = inet_addr(testConfig->getDestinationAddress().data());
	socklen_t addrlen = sizeof(serverAddr);
	this->canClose = false;
	TestResults results = TestResults(testConfig->getTotalTestCount());
	for (int i = 0; i < this->testConfig->getTotalTestCount(); i++) {
		for (int j = 0; j < this->testConfig->getSingleTestCount(); j++) {
			auto now = std::chrono::high_resolution_clock::now();
			if (this->testConfig->getTestNetworkType() == tc::TestNetworkType::TCP) {
				if (write(client, this->testConfig->getCustomData(), this->testConfig->getCustomDataLength()) == -1) {
					TestResult result = TestResult(tr::TestResultType::WRITE_FAILED, std::chrono::microseconds(0));
					results.push(i, result);
					continue;
				}
				if (read(client, this->buffer, this->testConfig->getCustomDataLength()) == -1) {
					TestResult result = TestResult(tr::TestResultType::READ_FAILED, std::chrono::microseconds(0));
					results.push(i, result);
					continue;
				}
			} else if (this->testConfig->getTestNetworkType() == tc::TestNetworkType::UDP) {
				if (sendto(client, this->testConfig->getCustomData(), this->testConfig->getCustomDataLength(), 0,
				           (struct sockaddr*) &serverAddr, addrlen) == -1) {
					TestResult result = TestResult(tr::TestResultType::WRITE_FAILED, std::chrono::microseconds(0));
					results.push(i, result);
					continue;
				}
				if (recvfrom(client, this->buffer, this->testConfig->getCustomDataLength(), 0, (struct sockaddr*) &serverAddr, &addrlen) == -1) {
					TestResult result = TestResult(tr::TestResultType::READ_FAILED, std::chrono::microseconds(0));
					results.push(i, result);
					continue;
				}
			}
			auto then = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(then - now);
			if (memcmp(this->testConfig->getCustomData(), this->buffer, this->testConfig->getCustomDataLength()) != 0) {
				TestResult result = TestResult(tr::TestResultType::DATA_MISMATCH, duration);
				results.push(i, result);
				continue;
			}
			TestResult result = TestResult(tr::TestResultType::NO_ERROR, duration);
			results.push(i, result);
			std::this_thread::sleep_for(testConfig->getSingleTestInterval());
		}
		std::this_thread::sleep_for(testConfig->getTotalTestInterval());
	}
	this->canClose = true;
	return results;
}

void ActiveServer::close() const {
	if (this->canClose) {
		shutdown(client, SHUT_RDWR);
		::close(client);
	}
}

ActiveServer::~ActiveServer() {
	delete[] this->buffer;
}

bool ActiveServer::refreshTestConfig(TestConfig *testConfig) {
	if (testConfig->getSourcePort() != testConfig->getSourcePort() || testConfig->getDestinationPort() != testConfig->getDestinationPort() ||
	    testConfig->getTestNetworkType() != testConfig->getTestNetworkType() || testConfig->getDestinationAddress() != testConfig->getDestinationAddress())
		return false;
	this->testConfig = testConfig;
	return true;
}


