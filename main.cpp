#include <iostream>
#include "TestConfig.h"
#include "PassiveServer.h"
#include "ActiveServer.h"

PassiveServer * passiveServer = nullptr;

ActiveServer * activeServer = nullptr;

void close() {
	if (activeServer != nullptr) {
		activeServer->close();
		delete activeServer;
	}
	if (passiveServer != nullptr) {
		passiveServer->close();
		delete passiveServer;
	}
}

TestConfig * createTestConfigByTerminal() {
	unsigned int totalTestCount, singleTestCount;
	std::cout << "Please input total test count: ";
	std::cin >> totalTestCount;
	std::cout << "Please input single test count: ";
	std::cin >> singleTestCount;
	unsigned int totalTestInterval, singleTestInterval;
	std::cout << "Please input total test interval(us): ";
	std::cin >> totalTestInterval;
	std::cout << "Please input single test interval(us): ";
	std::cin >> singleTestInterval;
	unsigned short networkType;
	std::cout << "Please input network type(0 for UDP, other for TCP): ";
	std::cin >> networkType;
	unsigned short sourcePort;
	std::cout << "Please input source port: ";
	std::cin >> sourcePort;
	std::string destinationAddress;
	std::cout << "Please input destination address: ";
	std::cin >> destinationAddress;
	unsigned short destinationPort;
	std::cout << "Please input destination port: ";
	std::cin >> destinationPort;
	std::string customData;
	std::cout << "Please input custom data: ";
	std::cin >> customData;
	unsigned char * customDataArray = new unsigned char[customData.length()];
	for (int i = 0; i < customData.length(); i++) {
		customDataArray[i] = customData[i];
	}
	return new TestConfig(singleTestCount, totalTestCount, std::chrono::microseconds(singleTestInterval),
	                      std::chrono::microseconds(totalTestInterval), networkType == 0 ? tc::TestNetworkType::UDP : tc::TestNetworkType::TCP, sourcePort,
	                      destinationAddress, destinationPort, customDataArray, customData.length());
}

TestConfig * createSimpleTestConfig() {
	return new TestConfig(100, 1, std::chrono::microseconds(1000), std::chrono::microseconds(100000),
	                      tc::TestNetworkType::UDP, 24001, "127.0.0.1", 24000, nullptr, 0);
}

int main() {
	TestConfig * testConfig = nullptr;
	while (true) {
		int op;
		std::cout << "Please input your operation: " << std::endl;
		std::cin >> op;
		if (op == -1) {
			delete testConfig;
			testConfig = createSimpleTestConfig();
		} else if (op == 0) {
			delete testConfig;
			testConfig = createTestConfigByTerminal();
		} else if (op == 1) {
			passiveServer = new PassiveServer(*testConfig);
			passiveServer->run();
		} else if (op == 2) {
			passiveServer->close();
			delete passiveServer;
		} else if (op == 3) {
			activeServer = new ActiveServer(testConfig);
		} else if (op == 4) {
			TestResults results = activeServer->test();
			for (int i = 0; i < testConfig->getTotalTestCount(); i ++) {
				std::cout << "Test " << i << " result: " << std::endl;
				for (int j = 0; j < testConfig->getSingleTestCount(); j++) {
					std::cout << "Case " << j << " result: " << results.get(i,j) << std::endl;
				}
				std::cout << "==========================================" << std::endl;
			}
		} else if (op == 5) {
			activeServer->close();
			delete activeServer;
		} else {
			close();
			delete testConfig;
			break;
		}
	}
}
