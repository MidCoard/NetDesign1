#ifndef NETDESIGN1_ACTIVESERVER_H
#define NETDESIGN1_ACTIVESERVER_H

#include "TestConfig.h"
#include "thread"
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "TestResults.h"

class ActiveServer {

private:

	TestConfig testConfig;

	unsigned char *buffer;

public:
	explicit ActiveServer(const TestConfig& testConfig);

	~ActiveServer();

	TestResults test();
};


#endif //NETDESIGN1_ACTIVESERVER_H
