#ifndef NETDESIGN1_ACTIVESERVER_H
#define NETDESIGN1_ACTIVESERVER_H

#include "TestConfig.h"
#include "thread"
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "TestResults.h"
#include <chrono>

class ActiveServer {

private:

	TestConfig* testConfig;

	unsigned char *buffer;

	int client;

	bool canClose = true;

	void init();

public:
	explicit ActiveServer(TestConfig* testConfig);

	~ActiveServer();

	TestResults test();

	bool stop() const;

	bool refreshConfig(TestConfig* testConfig);
};


#endif //NETDESIGN1_ACTIVESERVER_H
