#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include "TestConfig.h"
#include "PassiveServer.h"
#include "ActiveServer.h"
#include "TestConfigConstructor.h"

PassiveServer * passiveServer = nullptr;

ActiveServer * activeServer = nullptr;

static TestConfig * globalTestConfig = nullptr;

static TestConfigConstructor globalTestConfigConstructor;

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


static std::tuple<QLabel*, QLineEdit*, QWidget*> addHorizontalTextEditorWidgetInVerticalLayout(QVBoxLayout * verticalLayout, const QString& text, QWidget* widget) {
	auto* label = new QLabel(text);
	auto* editor = new QLineEdit();
	addHorizontalWidgetsInVerticalLayout(verticalLayout, {label, editor, widget});
	return {label, editor, widget};
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QWidget mainWindow;
	// config setup
	auto* configLabel = new QLabel("Global Config");
	auto* configButton = new QPushButton("Config");
	auto* configSaveButton = new QPushButton("Save Config");
	auto* configLayout = new QHBoxLayout;
	configLayout->addWidget(configLabel);
	configLayout->addWidget(configButton);
	configLayout->addWidget(configSaveButton);

	// config window setup

	auto* configWindow = new QWidget;
	auto* configWindowLayout = new QVBoxLayout;
	configWindow->setLayout(configWindowLayout);
	configWindow->setWindowTitle("Config");
	auto* configLoadButton = new QPushButton("Load Config");
	configWindowLayout->addWidget(configLoadButton);
	auto* totalCountEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Total Test Count(0-10000)").second;
	totalCountEditor->setValidator(new QIntValidator(0, 10000));
	QObject::connect(totalCountEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setTotalTestCount);
	auto* singleCountEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Single Test Count(0-10000)").second;
	singleCountEditor->setValidator(new QIntValidator(0, 10000));
	QObject::connect(singleCountEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setSingleTestCount);
	auto* totalIntervalEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Total Test Interval(0-100000000us)").second;
	totalIntervalEditor->setValidator(new QIntValidator(0, 100000000));
	QObject::connect(totalIntervalEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setTotalTestInterval);
	auto* singleIntervalEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Single Test Interval(0-100000000us)").second;
	singleIntervalEditor->setValidator(new QIntValidator(0, 100000000));
	QObject::connect(singleIntervalEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setSingleTestInterval);
	auto * configNetworkTypeComboBox = new QComboBox;
	configNetworkTypeComboBox->addItem("UDP");
	configNetworkTypeComboBox->addItem("TCP");
	addHorizontalLabelWidgetInVerticalLayout(configWindowLayout, "Network Type", configNetworkTypeComboBox);
	QObject::connect(configNetworkTypeComboBox, &QComboBox::currentIndexChanged, &globalTestConfigConstructor, &TestConfigConstructor::setNetworkType);
	auto* sourcePortEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Source Port").second;
	sourcePortEditor->setValidator(new QIntValidator(0, 65535));
	QObject::connect(sourcePortEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setSourcePort);
	auto* destinationAddressEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Destination Address").second;
	QObject::connect(destinationAddressEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setDestinationAddress);
	auto* destinationPortEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Destination Port").second;
	destinationPortEditor->setValidator(new QIntValidator(0, 65535));
	QObject::connect(destinationPortEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setDestinationPort);
	auto* customDataLengthEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Custom Data Length(0-1024)").second;
	customDataLengthEditor->setValidator(new QIntValidator(0, 1024));
	QObject::connect(customDataLengthEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setCustomDataLength);
	auto* customDataClearButton = new QPushButton("Clear");
	auto* customDataEditor = get<1>(addHorizontalTextEditorWidgetInVerticalLayout(configWindowLayout, "Custom Data", customDataClearButton));
	QObject::connect(customDataClearButton, &QPushButton::clicked, [customDataEditor]() {
		customDataEditor->setText("");
	});
	QObject::connect(customDataEditor, &QLineEdit::textChanged, [customDataLengthEditor](const QString& text) {
		if (text.length() != 0) {
			customDataLengthEditor->setText(QString::number(text.length()));
			customDataLengthEditor->setEnabled(false);
		} else {
			customDataLengthEditor->setText("");
			customDataLengthEditor->setEnabled(true);
		}
	});
	QObject::connect(customDataEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setCustomData);
	auto* configConfirmButton = new QPushButton("Confirm Config");
	QObject::connect(configConfirmButton, &QPushButton::clicked, [configWindow, totalCountEditor, singleCountEditor, totalIntervalEditor, singleIntervalEditor, configNetworkTypeComboBox, sourcePortEditor, destinationAddressEditor, destinationPortEditor, customDataLengthEditor, customDataEditor
																  ]() {
		bool ok = true;

		if (totalCountEditor->text().isEmpty() || totalCountEditor->text().toInt() > 10000)
			ok = false;

		if (singleCountEditor->text().isEmpty() || singleCountEditor->text().toInt() > 10000)
			ok = false;

		if (totalIntervalEditor->text().isEmpty() || totalIntervalEditor->text().toInt() > 100000000)
			ok = false;

		if (singleIntervalEditor->text().isEmpty() || singleIntervalEditor->text().toInt() > 100000000)
			ok = false;

		if (sourcePortEditor->text().isEmpty() || sourcePortEditor->text().toInt() > 65535)
			ok = false;

		if (destinationAddressEditor->text().isEmpty())
			ok = false;

		if (destinationPortEditor->text().isEmpty() || destinationPortEditor->text().toInt() > 65535)
			ok = false;

		if (customDataLengthEditor->text().isEmpty() || customDataLengthEditor->text().toInt() > 1024)
			ok = false;

		if (ok) {
			QMessageBox::information(nullptr, "Success", "Config confirmed");
			unsigned char* customData = nullptr;
			if (!customDataEditor->text().isEmpty()) {
				customData = new unsigned char[customDataEditor->text().length()];
				for (int i = 0; i < customDataEditor->text().length(); ++i)
					customData[i] = customDataEditor->text().at(i).toLatin1();
			}
			globalTestConfig = globalTestConfigConstructor.construct();
			configWindow->hide();
		} else {
			QMessageBox::critical(nullptr, "Error", "Config invalid");
		}

	});
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
