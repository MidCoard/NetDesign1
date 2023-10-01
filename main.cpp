#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QTableWidget>
#include "QFileDialog"
#include "QThread"
#include "TestConfig.h"
#include "PassiveServer.h"
#include "ActiveServer.h"
#include "TestConfigConstructor.h"
#include "PassiveServerStatus.h"
#include "ActiveServerStatus.h"
#include "TestResults.h"
#include <fstream>
#include <nlohmann/json.hpp>

static PassiveServer * passiveServer = nullptr;

static ActiveServer * activeServer = nullptr;

static TestConfig * globalTestConfig = nullptr;

static TestConfigConstructor globalTestConfigConstructor;

static PassiveServerStatus passiveServerStatus;

static ActiveServerStatus activeServerStatus;

static TestResults * currentTestResults;

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
	auto* configFileLabel = new QLabel("Path");
	auto* configFilePathLineEditor = new QLineEdit;
	auto* configFileSelectButton = new QPushButton("Select File");
	auto* configLoadButton = new QPushButton("Load Config");
	addHorizontalWidgetsInVerticalLayout(configWindowLayout, {configFileLabel, configFilePathLineEditor, configFileSelectButton, configLoadButton});

	QObject::connect(configFileSelectButton, &QPushButton::clicked, [configFilePathLineEditor]() {
		configFilePathLineEditor->setText(QFileDialog::getOpenFileName(nullptr, "Select Config File", "", "Config File(*.nd1)"));
	});

	QObject::connect(configLoadButton, &QPushButton::clicked, [configFilePathLineEditor]() {
		bool ok = globalTestConfigConstructor.loadFromFile(configFilePathLineEditor->text());
		if (!ok)
			QMessageBox::critical(nullptr, "Error", "Load Config Failed");
	});


	auto* totalCountEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Total Test Count(0-10000)").second;
	totalCountEditor->setValidator(new QIntValidator(0, 10000));
	QObject::connect(totalCountEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setTotalTestCount);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [totalCountEditor]() {
		totalCountEditor->setText(globalTestConfigConstructor.getTotalTestCount());
	});
	auto* singleCountEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Single Test Count(0-10000)").second;
	singleCountEditor->setValidator(new QIntValidator(0, 10000));
	QObject::connect(singleCountEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setSingleTestCount);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [singleCountEditor]() {
		singleCountEditor->setText(globalTestConfigConstructor.getSingleTestCount());
	});
	auto* totalIntervalEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Total Test Interval(0-100000000us)").second;
	totalIntervalEditor->setValidator(new QIntValidator(0, 100000000));
	QObject::connect(totalIntervalEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setTotalTestInterval);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [totalIntervalEditor]() {
		totalIntervalEditor->setText(globalTestConfigConstructor.getTotalTestInterval());
	});
	auto* singleIntervalEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Single Test Interval(0-100000000us)").second;
	singleIntervalEditor->setValidator(new QIntValidator(0, 100000000));
	QObject::connect(singleIntervalEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setSingleTestInterval);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [singleIntervalEditor]() {
		singleIntervalEditor->setText(globalTestConfigConstructor.getSingleTestInterval());
	});
	auto * configNetworkTypeComboBox = new QComboBox;
	configNetworkTypeComboBox->addItem("UDP");
	configNetworkTypeComboBox->addItem("TCP");
	addHorizontalLabelWidgetInVerticalLayout(configWindowLayout, "Network Type", configNetworkTypeComboBox);
	QObject::connect(configNetworkTypeComboBox, &QComboBox::currentIndexChanged, &globalTestConfigConstructor, &TestConfigConstructor::setNetworkType);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [configNetworkTypeComboBox]() {
		configNetworkTypeComboBox->setCurrentIndex(globalTestConfigConstructor.getNetworkType());
	});
	auto* sourcePortEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Source Port").second;
	sourcePortEditor->setValidator(new QIntValidator(0, 65535));
	QObject::connect(sourcePortEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setSourcePort);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [sourcePortEditor]() {
		sourcePortEditor->setText(globalTestConfigConstructor.getSourcePort());
	});
	auto* destinationAddressEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Destination Address").second;
	QObject::connect(destinationAddressEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setDestinationAddress);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [destinationAddressEditor]() {
		destinationAddressEditor->setText(globalTestConfigConstructor.getDestinationAddress());
	});
	auto* destinationPortEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Destination Port").second;
	destinationPortEditor->setValidator(new QIntValidator(0, 65535));
	QObject::connect(destinationPortEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setDestinationPort);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [destinationPortEditor]() {
		destinationPortEditor->setText(globalTestConfigConstructor.getDestinationPort());
	});
	auto* customDataLengthEditor = addHorizontalTextEditorInVerticalLayout(configWindowLayout, "Custom Data Length(0-1024)").second;
	customDataLengthEditor->setValidator(new QIntValidator(0, 1024));
	QObject::connect(customDataLengthEditor, &QLineEdit::textChanged, &globalTestConfigConstructor, &TestConfigConstructor::setCustomDataLength);
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [customDataLengthEditor]() {
		customDataLengthEditor->setText(globalTestConfigConstructor.getCustomDataLength());
	});
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
	QObject::connect(&globalTestConfigConstructor, &TestConfigConstructor::valueChanged, [customDataEditor]() {
		customDataEditor->setText(globalTestConfigConstructor.getCustomData());
	});
	auto* configConfirmButton = new QPushButton("Confirm Config");

	auto* testResultsTable = new QTableWidget;
	QObject::connect(configConfirmButton, &QPushButton::clicked, [testResultsTable, configWindow, totalCountEditor, singleCountEditor, totalIntervalEditor, singleIntervalEditor, configNetworkTypeComboBox, sourcePortEditor, destinationAddressEditor, destinationPortEditor, customDataLengthEditor
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
			globalTestConfig = globalTestConfigConstructor.construct();
			if (activeServer == nullptr) {
				activeServer = new ActiveServer(globalTestConfig);
				activeServerStatus.setStatus(as::Status::IDLE);
			} else {
				if (!activeServer->refreshConfig(globalTestConfig)) {
					delete activeServer;
					activeServer = new ActiveServer(globalTestConfig);
					activeServerStatus.setStatus(as::Status::IDLE);
				}
				if (passiveServer != nullptr && !passiveServer->checkConfig(globalTestConfig)) {
					QMessageBox::critical(nullptr, "Error", "Config confirmed but passive server need to restart");
					passiveServerStatus.setStatus(ps::Status::RUNNING_WITH_ERROR);
					configWindow->hide();
					return;
				}
			}
			testResultsTable->clear();
			testResultsTable->setRowCount(globalTestConfig->getTotalTestCount());
			testResultsTable->setColumnCount(globalTestConfig->getSingleTestCount());
			QMessageBox::information(nullptr, "Success", "Config confirmed");
			configWindow->hide();
			return;
		} else {
			QMessageBox::critical(nullptr, "Error", "Config invalid");
		}

	});
	configWindowLayout->addWidget(configConfirmButton);
	configWindow->hide();

	// connect config window
	QObject::connect(configButton, &QPushButton::clicked, [configWindow](){
		globalTestConfigConstructor.emitEvent();
		configWindow->show();
	});

	// config save window
	QWidget* saveConfigWindow = new QWidget;

	auto* saveConfigPathLineEditor = new QLineEdit;
	auto* saveConfigFileSelectButton = new QPushButton("Select File");
	auto* saveConfigSaveButton = new QPushButton("Save Config");

	auto* saveConfigWindowLayout = new QHBoxLayout;
	saveConfigWindowLayout->addWidget(saveConfigPathLineEditor);
	saveConfigWindowLayout->addWidget(saveConfigFileSelectButton);
	saveConfigWindowLayout->addWidget(saveConfigSaveButton);

	QObject::connect(saveConfigFileSelectButton, &QPushButton::clicked, [saveConfigPathLineEditor]() {
		QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Config", "", "Config File (*.nd1)");
		if (!fileName.isEmpty())
			saveConfigPathLineEditor->setText(fileName);
	});

	QObject::connect(saveConfigSaveButton, &QPushButton::clicked, [saveConfigPathLineEditor, saveConfigWindow]() {
		if (saveConfigPathLineEditor->text().isEmpty()) {
			QMessageBox::critical(nullptr, "Error", "File path is empty");
			return;
		}
		if (globalTestConfig == nullptr) {
			QMessageBox::critical(nullptr, "Error", "Config is empty");
			return;
		}
		if (!globalTestConfig->saveToFile(saveConfigPathLineEditor->text())) {
			QMessageBox::critical(nullptr, "Error", "Save failed");
			return;
		}
		QMessageBox::information(nullptr, "Success", "Save success");
		saveConfigWindow->hide();
	});

	saveConfigWindow->setLayout(saveConfigWindowLayout);
	saveConfigWindow->setWindowTitle("Save Config");
	saveConfigWindow->hide();

	QObject::connect(configSaveButton, &QPushButton::clicked, saveConfigWindow, &QWidget::show);


	// passive setup
	auto* passiveLabel = new QLabel("Passive Server");
	auto* passiveStartButton = new QPushButton("Start");
	auto* passiveStopButton = new QPushButton("Stop");
	passiveStopButton->setEnabled(false);
	auto* passiveStatus = new QLabel("Status: Stopped");
	auto* passiveLayout = new QHBoxLayout;
	passiveLayout->addWidget(passiveLabel);
	passiveLayout->addWidget(passiveStartButton);
	passiveLayout->addWidget(passiveStopButton);
	passiveLayout->addWidget(passiveStatus);

	QObject::connect(&passiveServerStatus, &PassiveServerStatus::statusChanged, passiveStatus, &QLabel::setText);

	QObject::connect(&passiveServerStatus, &PassiveServerStatus::statusChanged, [passiveStartButton, passiveStopButton]() {
		if (passiveServerStatus.getStatus() == ps::Status::RUNNING || passiveServerStatus.getStatus() == ps::Status::RUNNING_WITH_ERROR) {
			passiveStartButton->setEnabled(false);
			passiveStopButton->setEnabled(true);
		} else {
			passiveStartButton->setEnabled(true);
			passiveStopButton->setEnabled(false);
		}
	});

	QObject::connect(passiveStartButton, &QPushButton::clicked, []() {
		if (globalTestConfig == nullptr) {
			QMessageBox::critical(nullptr, "Error", "Config is empty");
			return;
		}
		passiveServer = new PassiveServer(*globalTestConfig);
		passiveServer->start();
		passiveServerStatus.setStatus(ps::Status::RUNNING);
	});

	QObject::connect(passiveStopButton, &QPushButton::clicked, []() {
		passiveServer->stop();
		passiveServerStatus.setStatus(ps::Status::STOPPED);
		delete passiveServer;
		passiveServer = nullptr;
	});

	// active setup

	auto* activeLabel = new QLabel("Test Client");
	auto* activeTestButton = new QPushButton("Test");
	auto* activeStatus = new QLabel("Status: Stopped");
	auto* activeLayout = new QHBoxLayout;
	activeLayout->addWidget(activeLabel);
	activeLayout->addWidget(activeTestButton);
	activeLayout->addWidget(activeStatus);

	QObject::connect(&activeServerStatus, &ActiveServerStatus::statusChanged, activeStatus, &QLabel::setText);


	QObject::connect(activeTestButton, &QPushButton::clicked, []() {
		if (activeServer == nullptr || globalTestConfig == nullptr) {
			QMessageBox::critical(nullptr, "Error", "Config is empty");
			return;
		}
		activeServerStatus.setStatus(as::Status::TESTING);
		new std::thread([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds (500));
			delete currentTestResults;
			currentTestResults = new TestResults(activeServer->test());
			activeServerStatus.setStatus(as::Status::TESTED);
		});
	});

	QObject::connect(&activeServerStatus, &ActiveServerStatus::statusChanged, &activeServerStatus, [testResultsTable]() {
		if (activeServerStatus.getStatus() == as::Status::TESTED) {
			QMessageBox::information(nullptr, "Success", "Test finished");
			activeServerStatus.setStatus(as::Status::IDLE);
			testResultsTable->clear();
			for (int i = 0; i < globalTestConfig->getTotalTestCount(); i++)
				for (int j = 0; j < globalTestConfig->getSingleTestCount(); j++)
					testResultsTable->setItem(i, j, new QTableWidgetItem(QString::number(currentTestResults->get(i, j).duration.count())));
		}
	},Qt::QueuedConnection);

	// export setup

	auto* exportAllButton = new QPushButton("Export");
	auto* exportDurationButton = new QPushButton("Export Duration");
	auto* exportLayout = new QHBoxLayout;
	exportLayout->addWidget(exportAllButton);
	exportLayout->addWidget(exportDurationButton);

	// export all window setup

	auto* exportAllWindow = new QWidget;
	auto* exportAllWindowLayout = new QHBoxLayout;
	auto* exportAllPathLineEditor = new QLineEdit;
	auto* exportAllPathButton = new QPushButton("Select File");

	QObject::connect(exportAllPathButton, &QPushButton::clicked, [exportAllPathLineEditor]() {
		QString fileName = QFileDialog::getSaveFileName(nullptr, "Select File", "", "JSON (*.json)");
		if (!fileName.isEmpty())
			exportAllPathLineEditor->setText(fileName);
	});

	auto* exportAllSaveButton = new QPushButton("Export");

	QObject::connect(exportAllSaveButton, &QPushButton::clicked, [exportAllWindow]() {

		exportAllWindow->hide();
	});

	exportAllWindowLayout->addWidget(exportAllPathLineEditor);
	exportAllWindowLayout->addWidget(exportAllPathButton);
	exportAllWindowLayout->addWidget(exportAllSaveButton);
	exportAllWindow->setLayout(exportAllWindowLayout);
	exportAllWindow->setWindowTitle("Export All");
	exportAllWindow->hide();

	QObject::connect(exportAllButton, &QPushButton::clicked, [exportAllWindow]() {
		if (currentTestResults == nullptr) {
			QMessageBox::critical(nullptr, "Error", "No test results");
			return;
		}
		exportAllWindow->show();
	});

	// export duration window setup

	auto* exportDurationWindow = new QWidget;
	auto* exportDurationWindowLayout = new QHBoxLayout;
	auto* exportDurationPathLineEditor = new QLineEdit;
	auto* exportDurationPathButton = new QPushButton("Select File");

	QObject::connect(exportDurationPathButton, &QPushButton::clicked, [exportDurationPathLineEditor]() {
		QString fileName = QFileDialog::getSaveFileName(nullptr, "Select File", "", "JSON (*.json)");
		if (!fileName.isEmpty())
			exportDurationPathLineEditor->setText(fileName);
	});

	auto* exportDurationSaveButton = new QPushButton("Export");
	exportDurationWindowLayout->addWidget(exportDurationPathLineEditor);
	exportDurationWindowLayout->addWidget(exportDurationPathButton);
	exportDurationWindowLayout->addWidget(exportDurationSaveButton);
	exportDurationWindow->setLayout(exportDurationWindowLayout);
	exportDurationWindow->setWindowTitle("Export Duration");
	exportDurationWindow->hide();

	QObject::connect(exportDurationButton, &QPushButton::clicked, [exportDurationWindow]() {
		if (currentTestResults == nullptr) {
			QMessageBox::critical(nullptr, "Error", "No test results");
			return;
		}
		exportDurationWindow->show();
	});


	auto* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(configLayout);
	mainLayout->addLayout(passiveLayout);
	mainLayout->addLayout(activeLayout);
	mainLayout->addWidget(testResultsTable);
	mainLayout->addLayout(exportLayout);

	mainWindow.setLayout(mainLayout);
	mainWindow.show();
	mainWindow.setWindowTitle("Network Design 1");
	int r = QApplication::exec();
	delete passiveServer;
	delete activeServer;
	delete currentTestResults;
	return r;
}
