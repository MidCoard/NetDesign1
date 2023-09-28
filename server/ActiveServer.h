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

	TestConfig* testConfig;

	unsigned char *buffer;

	int client;

	bool canClose = true;

public:
	explicit ActiveServer(TestConfig* testConfig);

	~ActiveServer();

	TestResults test();

	void close() const;

	bool refreshTestConfig(TestConfig* testConfig);
};


#endif //NETDESIGN1_ACTIVESERVER_H
