#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include "TestConfig.h"
#include "PassiveServer.h"
#include "ActiveServer.h"

PassiveServer * passiveServer = nullptr;

ActiveServer * activeServer = nullptr;

void close() {
	if (activeServer != nullptr) {
		activeServer->close();
		delete activeServer;
	}
	if (passiveServer != nullptr) {
		passiveServer->close();
		delete passiveServer;
	}
}

TestConfig * createTestConfigByTerminal() {
	unsigned int totalTestCount, singleTestCount;
	std::cout << "Please input total test count: ";
	std::cin >> totalTestCount;
	std::cout << "Please input single test count: ";
	std::cin >> singleTestCount;
	unsigned int totalTestInterval, singleTestInterval;
	std::cout << "Please input total test interval(us): ";
	std::cin >> totalTestInterval;
	std::cout << "Please input single test interval(us): ";
	std::cin >> singleTestInterval;
	unsigned short networkType;
	std::cout << "Please input network type(0 for UDP, other for TCP): ";
	std::cin >> networkType;
	unsigned short sourcePort;
	std::cout << "Please input source port: ";
	std::cin >> sourcePort;
	std::string destinationAddress;
	std::cout << "Please input destination address: ";
	std::cin >> destinationAddress;
	unsigned short destinationPort;
	std::cout << "Please input destination port: ";
	std::cin >> destinationPort;
	std::string customData;
	std::cout << "Please input custom data: ";
	std::cin >> customData;
	unsigned char * customDataArray = new unsigned char[customData.length()];
	for (int i = 0; i < customData.length(); i++) {
		customDataArray[i] = customData[i];
	}
	return new TestConfig(singleTestCount, totalTestCount, std::chrono::microseconds(singleTestInterval),
	                      std::chrono::microseconds(totalTestInterval), networkType == 0 ? tc::TestNetworkType::UDP : tc::TestNetworkType::TCP, sourcePort,
	                      destinationAddress, destinationPort, customDataArray, customData.length());
}

TestConfig * createSimpleTestConfig() {
	return new TestConfig(100, 1, std::chrono::microseconds(1000), std::chrono::microseconds(100000),
	                      tc::TestNetworkType::UDP, 24001, "127.0.0.1", 24000, nullptr, 0);
}

static std::initializer_list<QWidget *> addHorizontalWidgetsInVerticalLayout(QVBoxLayout * verticalLayout, std::initializer_list<QWidget *> widgets) {
	auto* layout = new QHBoxLayout;
	for (auto* widget : widgets)
		layout->addWidget(widget);
	verticalLayout->addLayout(layout);
	return widgets;
}

static std::initializer_list<QWidget *> addHorizontalWidgetInVerticalLayout(QVBoxLayout * verticalLayout, QWidget * widget) {
	return addHorizontalWidgetsInVerticalLayout(verticalLayout, {widget});
}

static std::initializer_list<QWidget *> addHorizontalTextEditorInVerticalLayout(QVBoxLayout * verticalLayout, const QString& text) {
	return addHorizontalWidgetsInVerticalLayout(verticalLayout, {new QLabel(text), new QLineEdit});
}

static std::initializer_list<QWidget *> addHorizontalLabelWidgetInVerticalLayout(QVBoxLayout * verticalLayout, const QString& text, QWidget * widget) {
	return addHorizontalWidgetsInVerticalLayout(verticalLayout, {new QLabel(text), widget});
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QWidget mainWindow;
	// passive part
	auto* passiveLabel = new QLabel("Passive Server");
	auto* passiveStartButton = new QPushButton("Start");
	auto* passiveStopButton = new QPushButton("Stop");
	auto* passiveConfigButton = new QPushButton("Config");
	auto* passiveStatus = new QLabel("Status: Stopped");
	auto* passiveLayout = new QHBoxLayout;
	passiveLayout->addWidget(passiveLabel);
	passiveLayout->addWidget(passiveStartButton);
	passiveLayout->addWidget(passiveStopButton);
	passiveLayout->addWidget(passiveConfigButton);
	passiveLayout->addWidget(passiveStatus);

	auto* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(passiveLayout);

	mainWindow.setLayout(mainLayout);
	mainWindow.show();
	mainWindow.setWindowTitle("Network Design 1");


	// passive config page
	auto* passiveConfigWindow = new QWidget;
	auto* passiveConfigLayout = new QVBoxLayout;
	passiveConfigWindow->setLayout(passiveConfigLayout);
	passiveConfigWindow->setWindowTitle("Passive Server Config");
	auto* passiveConfigLoadButton = new QPushButton("Load");
	passiveConfigLayout->addWidget(passiveConfigLoadButton);
	addHorizontalTextEditorInVerticalLayout(passiveConfigLayout, "Total Test Count");
	addHorizontalTextEditorInVerticalLayout(passiveConfigLayout, "Single Test Count");
	addHorizontalTextEditorInVerticalLayout(passiveConfigLayout, "Total Test Interval(us)");
	addHorizontalTextEditorInVerticalLayout(passiveConfigLayout, "Single Test Interval(us)");
	auto * networkTypeComboBox = new QComboBox;
	networkTypeComboBox->addItem("UDP");
	networkTypeComboBox->addItem("TCP");
	addHorizontalLabelWidgetInVerticalLayout(passiveConfigLayout, "Network Type", networkTypeComboBox);
	addHorizontalTextEditorInVerticalLayout(passiveConfigLayout, "Source Port");
	addHorizontalTextEditorInVerticalLayout(passiveConfigLayout, "Destination Address");
	addHorizontalTextEditorInVerticalLayout(passiveConfigLayout, "Destination Port");
	addHorizontalTextEditorInVerticalLayout(passiveConfigLayout, "Custom Data");
	auto* passiveConfigConfirmButton = new QPushButton("Confirm");
	passiveConfigLayout->addWidget(passiveConfigConfirmButton);
	passiveConfigWindow->hide();

	// make config button work
	QObject::connect(passiveConfigButton, &QPushButton::clicked, passiveConfigWindow, &QWidget::show);



	return QApplication::exec();
//	TestConfig * testConfig = nullptr;
//	while (true) {
//		int op;
//		std::cout << "Please input your operation: " << std::endl;
//		std::cin >> op;
//		if (op == -1) {
//			delete testConfig;
//			testConfig = createSimpleTestConfig();
//		} else if (op == 0) {
//			delete testConfig;
//			testConfig = createTestConfigByTerminal();
//		} else if (op == 1) {
//			passiveServer = new PassiveServer(*testConfig);
//			passiveServer->run();
//		} else if (op == 2) {
//			passiveServer->close();
//			delete passiveServer;
//		} else if (op == 3) {
//			activeServer = new ActiveServer(testConfig);
//		} else if (op == 4) {
//			TestResults results = activeServer->test();
//			for (int i = 0; i < testConfig->getTotalTestCount(); i ++) {
//				std::cout << "Test " << i << " result: " << std::endl;
//				for (int j = 0; j < testConfig->getSingleTestCount(); j++) {
//					std::cout << "Case " << j << " result: " << results.get(i,j) << std::endl;
//				}
//				std::cout << "==========================================" << std::endl;
//			}
//		} else if (op == 5) {
//			activeServer->close();
//			delete activeServer;
//		} else {
//			close();
//			delete testConfig;
//			break;
//		}
//	}
}
