#include <iostream>
#include "TestConfig.h"
#include "PassiveServer.h"
#include "ActiveServer.h"

PassiveServer * passiveServer = nullptr;

ActiveServer * activeServer = nullptr;

void close() {
	if (passiveServer != nullptr) {
		passiveServer->close();
		delete passiveServer;
	}
	if (activeServer != nullptr) {
		activeServer->close();
		delete activeServer;
	}
}

TestConfig * createTestConfigByTerminal() {

}

int main() {
	TestConfig * testConfig;
	while (true) {
		int op;
		std::cin >> op;
		if (op == 0) {
			testConfig = createTestConfigByTerminal();
		} else if (op == 1) {
			passiveServer = new PassiveServer(*testConfig);
			passiveServer->run();
		} else if (op == 2) {
			passiveServer->close();
			delete passiveServer;
		} else if (op == 3) {
			activeServer = new ActiveServer(*testConfig);
			activeServer->test();
		} else if (op == 4) {
			activeServer->close();
			delete activeServer;
		} else {
			close();
		}
	}
}
