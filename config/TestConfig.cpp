#include "TestConfig.h"

static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

static unsigned int generateCustomDataLength() {
	std::uniform_int_distribution<int> distribution(1,1024);
	return distribution(generator);
}

static unsigned char * generateCustomData(unsigned int customDataLength) {
	auto * customData = new unsigned char[customDataLength];
	std::uniform_int_distribution<int> distribution(48,122);
	for (unsigned int i = 0; i < customDataLength; i++)
		customData[i] = distribution(generator);
	return customData;
}

TestConfig::TestConfig(unsigned int singleTestCount, unsigned int totalTestCount, std::chrono::microseconds  singleTestInterval, std::chrono::microseconds  totalTestInterval,
                       tc::TestNetworkType testNetworkType, unsigned short sourcePort,
                       std::string_view destinationAddress, unsigned short destinationPort, unsigned char *customData,
                       unsigned int customDataLength) : singleTestCount(singleTestCount), totalTestCount(totalTestCount),
											   singleTestInterval(singleTestInterval),
											   totalTestInterval(totalTestInterval),
											   testNetworkType(testNetworkType), sourcePort(sourcePort),
											   destinationAddress(destinationAddress),
											   destinationPort(destinationPort), customData(customData),
											   customDataLength(customDataLength) {
	if (customData == nullptr) {
		this->customDataLength = generateCustomDataLength();
		this->customData = generateCustomData(this->customDataLength);
	}
}

TestConfig::TestConfig(unsigned int singleTestCount, unsigned int totalTestCount,
                       tc::TestNetworkType testNetworkType, unsigned short sourcePort,
                       std::string_view destinationAddress, unsigned short destinationPort, unsigned char *customData,
                       unsigned int customDataLength) : TestConfig(singleTestCount, totalTestCount, std::chrono::microseconds(100), std::chrono::microseconds(100),
														   testNetworkType, sourcePort, destinationAddress, destinationPort,
														   customData, customDataLength) {}

TestConfig::TestConfig(unsigned int singleTestCount, unsigned int totalTestCount,
                       tc::TestNetworkType testNetworkType, unsigned short sourcePort,
                       std::string_view destinationAddress, unsigned short destinationPort) : TestConfig(singleTestCount, totalTestCount,
																								   testNetworkType, sourcePort, destinationAddress, destinationPort, nullptr,
																								   0) {}

TestConfig::TestConfig(unsigned int singleTestCount, unsigned int totalTestCount, unsigned short sourcePort, std::string_view destinationAddress, unsigned short destinationPort) : TestConfig(singleTestCount, totalTestCount,
                                                                                                                                                                                                                           tc::TestNetworkType::TCP, sourcePort, destinationAddress, destinationPort) {}

unsigned int TestConfig::getSingleTestCount() const {
	return singleTestCount;
}

unsigned int TestConfig::getTotalTestCount() const {
	return totalTestCount;
}

std::chrono::microseconds TestConfig::getSingleTestInterval() const {
	return singleTestInterval;
}

std::chrono::microseconds TestConfig::getTotalTestInterval() const {
	return totalTestInterval;
}

tc::TestNetworkType TestConfig::getTestNetworkType() const {
	return testNetworkType;
}

unsigned short TestConfig::getSourcePort() const {
	return sourcePort;
}

std::string_view TestConfig::getDestinationAddress() const {
	return destinationAddress;
}

unsigned short TestConfig::getDestinationPort() const {
	return destinationPort;
}

unsigned char *TestConfig::getCustomData() const {
	return customData;
}

unsigned int TestConfig::getCustomDataLength() const {
	return customDataLength;
}

TestConfig::~TestConfig() {
	delete[] customData;
}

int tc::convertNetworkType(tc::TestNetworkType testNetworkType) {
	switch (testNetworkType) {
		case tc::TestNetworkType::TCP:
			return SOCK_STREAM;
		case tc::TestNetworkType::UDP:
			return SOCK_DGRAM;
		default:
			return -1;
	}
}


