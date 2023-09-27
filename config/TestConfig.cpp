#include "TestConfig.h"

TestConfig::TestConfig(std::chrono::microseconds  singleTestCount, std::chrono::microseconds  totalTestCount, std::chrono::microseconds  singleTestInterval, std::chrono::microseconds  totalTestInterval,
                       tc::TestNetworkType testNetworkType, unsigned short sourcePort,
                       std::string_view destinationAddress, unsigned short destinationPort, unsigned char *customData,
                       int customDataLength) : singleTestCount(singleTestCount), totalTestCount(totalTestCount),
											   singleTestInterval(singleTestInterval),
											   totalTestInterval(totalTestInterval),
											   testNetworkType(testNetworkType), sourcePort(sourcePort),
											   destinationAddress(destinationAddress),
											   destinationPort(destinationPort), customData(customData),
											   customDataLength(customDataLength) {}

TestConfig::TestConfig(std::chrono::microseconds  singleTestCount, std::chrono::microseconds  totalTestCount, unsigned short sourcePort, std::string_view destinationAddress, unsigned short destinationPort) : TestConfig(singleTestCount, totalTestCount, std::chrono::microseconds(100), std::chrono::microseconds(100),
																			 tc::TestNetworkType::TCP, sourcePort, destinationAddress, destinationPort, nullptr,
																			 0) {}




