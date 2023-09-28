#include "PassiveServer.h"
#include "iostream"


PassiveServer::PassiveServer(const TestConfig& testConfig) : port(testConfig.getDestinationPort()), testNetworkType(testConfig.getTestNetworkType()), customDataLength(testConfig.getCustomDataLength()) {
	this->internal = socket(AF_INET, tc::convertNetworkType(testNetworkType), 0);
	if (this->internal == -1)
		throw std::runtime_error("Failed to create socket at port " + std::to_string(port));
	this->buffer = new unsigned char[testConfig.getCustomDataLength()];
	int option = 1;
	if (setsockopt(this->internal, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
		throw std::runtime_error("Failed to set socket options at port " + std::to_string(port));
	if (testNetworkType == tc::TestNetworkType::TCP) {
		option = customDataLength;
		if (setsockopt(this->internal, SOL_SOCKET, SO_RCVBUF, &option, sizeof(option)) == -1)
			throw std::runtime_error("Failed to set socket size options at port " + std::to_string(port));
	}
	struct sockaddr_in addr {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->internal, (struct sockaddr *) &addr, sizeof(addr)) == -1)
		throw std::runtime_error("Failed to bind socket at port " + std::to_string(port));
	if (testNetworkType == tc::TestNetworkType::TCP)
		listen(this->internal, testConfig.getSingleTestCount());
}

void PassiveServer::run() {
	this->thread = new std::thread([this]() {
		while (true) {
			if (this->shouldClose)
				break;
			struct sockaddr_in addr {};
			socklen_t addrlen = sizeof(addr);
			if (this->testNetworkType == tc::TestNetworkType::TCP) {
				int client = accept(this->internal, (struct sockaddr *) &addr, &addrlen);
				if (client == -1)
					continue;
				unsigned int length;
				while ((length = read(client, this->buffer, customDataLength)) != -1)
					write(client, this->buffer, length);
				shutdown(client, SHUT_RDWR);
				::close(client);
			} else {
				unsigned int length = recvfrom(this->internal, this->buffer, customDataLength, 0, (struct sockaddr *) &addr, &addrlen);
				if (length != 0)
					sendto(this->internal, this->buffer, length, 0, (struct sockaddr *) &addr, addrlen);
			}
		}
	});
}

void PassiveServer::close() {
	if (this->thread == nullptr)
		return;
	if (this->shouldClose)
		return;
	::close(this->internal);
	this->shouldClose = true;
	this->thread->join();
	delete this->thread;
	this->thread = nullptr;
}

PassiveServer::~PassiveServer() {
	this->close();
	delete[] this->buffer;
}


