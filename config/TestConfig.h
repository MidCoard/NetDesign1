#ifndef NETDESIGN1_TESTCONFIG_H
#define NETDESIGN1_TESTCONFIG_H

#include <cstring>
#include <chrono>
#include <random>
#include <string_view>
#include <sys/socket.h>
#include "QString"


namespace tc {

	typedef enum {
		UDP,
		TCP
	} TestNetworkType;

	int convertNetworkType(TestNetworkType testNetworkType);

}


class TestConfig {

private:
	unsigned int singleTestCount;
	unsigned int totalTestCount;
	std::chrono::microseconds  singleTestInterval;
	std::chrono::microseconds  totalTestInterval;
	tc::TestNetworkType testNetworkType;

	unsigned short sourcePort;

	std::string_view destinationAddress;
	unsigned short destinationPort;

	unsigned char * customData;
	unsigned int customDataLength;

public:

	TestConfig(unsigned int singleTestCount, unsigned int totalTestCount, std::chrono::microseconds singleTestInterval, std::chrono::microseconds totalTestInterval,
	           tc::TestNetworkType testNetworkType, unsigned short sourcePort, std::string_view destinationAddress,
	           unsigned short destinationPort, unsigned char * customData, unsigned int customDataLength);

	TestConfig(unsigned int singleTestCount, unsigned int totalTestCount,
	           tc::TestNetworkType testNetworkType, unsigned short sourcePort, std::string_view destinationAddress,
	           unsigned short destinationPort, unsigned char * customData, unsigned int customDataLength);

	TestConfig(unsigned int singleTestCount, unsigned int totalTestCount,
	           tc::TestNetworkType testNetworkType, unsigned short sourcePort, std::string_view destinationAddress,
	           unsigned short destinationPort);

	TestConfig(unsigned int singleTestCount, unsigned int totalTestCount, unsigned short sourcePort, std::string_view destinationAddress,
	           unsigned short destinationPort);

	~TestConfig();

	unsigned int getSingleTestCount() const;

	unsigned int getTotalTestCount() const;

	std::chrono::microseconds getSingleTestInterval() const;

	std::chrono::microseconds getTotalTestInterval() const;

	tc::TestNetworkType getTestNetworkType() const;

	unsigned short getSourcePort() const;

	std::string_view getDestinationAddress() const;

	unsigned short getDestinationPort() const;

	unsigned char * getCustomData() const;

	unsigned int getCustomDataLength() const;

	bool saveToFile(const QString& path);


};


#endif //NETDESIGN1_TESTCONFIG_H
