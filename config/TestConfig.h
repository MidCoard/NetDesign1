#ifndef NETDESIGN1_TESTCONFIG_H
#define NETDESIGN1_TESTCONFIG_H

#include "string_view"

namespace tc {

	typedef enum {
		UDP,
		TCP
	} TestNetworkType;

}


class TestConfig {

private:
	std::chrono::microseconds  singleTestCount;
	std::chrono::microseconds  totalTestCount;
	std::chrono::microseconds  singleTestInterval;
	std::chrono::microseconds  totalTestInterval;
	tc::TestNetworkType testNetworkType;

	unsigned short sourcePort;

	std::string_view destinationAddress;
	unsigned short destinationPort;

	unsigned char * customData;
	int customDataLength;




public:

	TestConfig(std::chrono::microseconds  singleTestCount, std::chrono::microseconds  totalTestCount, std::chrono::microseconds  singleTestInterval, std::chrono::microseconds  totalTestInterval,
	           tc::TestNetworkType testNetworkType, unsigned short sourcePort, std::string_view destinationAddress,
	           unsigned short destinationPort, unsigned char * customData, int customDataLength);

//	TestConfig(int singleTestCount, int totalTestCount,)

	TestConfig(std::chrono::microseconds  singleTestCount, std::chrono::microseconds  totalTestCount, unsigned short sourcePort, std::string_view destinationAddress,
	           unsigned short destinationPort);



};


#endif //NETDESIGN1_TESTCONFIG_H
