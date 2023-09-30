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

static std::pair<QLabel*, QLineEdit*> addHorizontalTextEditorInVerticalLayout(QVBoxLayout * verticalLayout, const QString& text) {
	std::pair<QLabel*, QLineEdit*> pair = {new QLabel(text), new QLineEdit()};
	addHorizontalWidgetsInVerticalLayout(verticalLayout, {pair.first, pair.second});
	return pair;
}

static std::initializer_list<QWidget *> addHorizontalLabelWidgetInVerticalLayout(QVBoxLayout * verticalLayout, const QString& text, QWidget * widget) {
	return addHorizontalWidgetsInVerticalLayout(verticalLayout, {new QLabel(text), widget});
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QWidget mainWindow;
	// config setup
	auto* configLabel = new QLabel("Global Config");
	auto* configButton = new QPushButton("Config");
	auto* configLayout = new QHBoxLayout;
	configLayout->addWidget(configLabel);
	configLayout->addWidget(configButton);

	// config window setup

	auto* configWindow = new QWidget;
	auto* configWindowLayout = new QVBoxLayout;
	configWindow->setLayout(configWindowLayout);
	configWindow->setWindowTitle("Passive Server Config");
	auto* configLoadButton = new QPushButton("Load Config");
	configWindowLayout->addWidget(configLoadButton);
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Total Test Count(0-10000)").second->setValidator(new QIntValidator(0, 10000));
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Single Test Count(0-10000)").second->setValidator(new QIntValidator(0, 10000));
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Total Test Interval(0-100000000us)").second->setValidator(new QIntValidator(0, 100000000));
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Single Test Interval(0-100000000us)").second->setValidator(new QIntValidator(0, 100000000));
	auto * configNetworkTypeComboBox = new QComboBox;
	configNetworkTypeComboBox->addItem("UDP");
	configNetworkTypeComboBox->addItem("TCP");
	addHorizontalLabelWidgetInVerticalLayout(configWindowLayout, "Network Type", configNetworkTypeComboBox);
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Source Port").second->setValidator(new QIntValidator(0, 65535));
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Destination Address");
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Destination Port").second->setValidator(new QIntValidator(0, 65535));
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Custom Data Length(0-1024)").second->setValidator(new QIntValidator(0, 1024));
	addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Custom Data");
	auto* configConfirmButton = new QPushButton("Confirm");
	configWindowLayout->addWidget(configConfirmButton);
	configWindow->hide();

	// connect config window
	QObject::connect(configButton, &QPushButton::clicked, configWindow, &QWidget::show);


	// passive setup
	auto* passiveLabel = new QLabel("Passive Server");
	auto* passiveStartButton = new QPushButton("Start");
	auto* passiveStopButton = new QPushButton("Stop");
	auto* passiveStatus = new QLabel("Status: Stopped");
	auto* passiveLayout = new QHBoxLayout;
	passiveLayout->addWidget(passiveLabel);
	passiveLayout->addWidget(passiveStartButton);
	passiveLayout->addWidget(passiveStopButton);
	passiveLayout->addWidget(passiveStatus);

	auto* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(configLayout);
	mainLayout->addLayout(passiveLayout);

	mainWindow.setLayout(mainLayout);
	mainWindow.show();
	mainWindow.setWindowTitle("Network Design 1");


	// config page




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
