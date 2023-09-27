#ifndef NETDESIGN1_PASSIVESERVER_H
#define NETDESIGN1_PASSIVESERVER_H

#include "TestConfig.h"
#include <string>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>

class PassiveServer {
private:
	unsigned short port;

	tc::TestNetworkType testNetworkType;

	unsigned int customDataLength;

	int internal;

	unsigned char *buffer;

	std::thread *thread = nullptr;

	bool shouldClose = false;

public:

	explicit PassiveServer(const TestConfig& testConfig);

	~PassiveServer();

	void run();

	void close();
};


#endif //NETDESIGN1_PASSIVESERVER_H
