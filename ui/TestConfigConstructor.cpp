#include <QFile>
#include <QTextStream>
#include <fstream>
#include "TestConfigConstructor.h"
#include "iostream"

void TestConfigConstructor::setTotalTestCount(const QString& q) {
	this->totalTestCount = q.toUInt();
}

void TestConfigConstructor::setSingleTestCount(const QString &q) {
	this->singleTestCount = q.toUInt();
}

void TestConfigConstructor::setTotalTestInterval(const QString &q) {
	this->totalTestInterval = std::chrono::microseconds(q.toUInt());
}

void TestConfigConstructor::setSingleTestInterval(const QString &q) {
	this->singleTestInterval = std::chrono::microseconds(q.toUInt());
}

void TestConfigConstructor::setNetworkType(int index) {
	this->testNetworkType = (tc::TestNetworkType) index;
}

void TestConfigConstructor::setSourcePort(const QString &q) {
	this->sourcePort = q.toUShort();
}

void TestConfigConstructor::setDestinationAddress(const QString &q) {
	this->destinationAddress = q.toStdString();
}

void TestConfigConstructor::setDestinationPort(const QString &q) {
	this->destinationPort = q.toUShort();
}

void TestConfigConstructor::setCustomData(const QString& q) {
	this->customData = q;
}

void TestConfigConstructor::setCustomDataLength(const QString &q) {
	this->customDataLength = q.toUInt();
}

TestConfig *TestConfigConstructor::build() {
	if (this->customData.isEmpty())
		return new TestConfig(this->singleTestCount, this->totalTestCount, this->singleTestInterval, this->totalTestInterval,
		                      this->testNetworkType, this->sourcePort, this->destinationAddress, this->destinationPort, nullptr, this->customDataLength);
	else {
		auto * _customData = new unsigned char[this->customDataLength];
		std::memcpy(_customData, this->customData.toStdString().c_str(), this->customDataLength);
		return new TestConfig(this->singleTestCount, this->totalTestCount, this->singleTestInterval, this->totalTestInterval,
		                      this->testNetworkType, this->sourcePort, this->destinationAddress, this->destinationPort, _customData, this->customDataLength);
	}
}

TestConfig *TestConfigConstructor::construct() {
	return build();
}

bool TestConfigConstructor::loadFromFile(const QString& path) {
    std::ifstream stream(path.toStdString());
    if (!stream.is_open()) return false;

    json rawJson;
    stream >> rawJson;

    std::uint64_t singleTestIntVal;
    std::uint64_t totalTestIntVal;
    std::string customDataString;

	try {
		rawJson.at("singleTestCount").get_to(this->singleTestCount);
		rawJson.at("totalTestCount").get_to(this->totalTestCount);
		rawJson.at("singleTestInterval").get_to(singleTestIntVal);
		rawJson.at("totalTestInterval").get_to(totalTestIntVal);
		rawJson.at("testNetworkType").get_to(this->testNetworkType);
		rawJson.at("sourcePort").get_to(this->sourcePort);
		rawJson.at("destinationAddress").get_to(this->destinationAddress);
		rawJson.at("destinationPort").get_to(this->destinationPort);
		rawJson.at("customData").get_to(customDataString);
		rawJson.at("customDataLength").get_to(this->customDataLength);
	} catch (json::exception& e) {
		return false;
	}

    this->singleTestInterval = std::chrono::microseconds(singleTestIntVal);
    this->totalTestInterval = std::chrono::microseconds(totalTestIntVal);
    this->customData = QString::fromStdString(customDataString);

	emitEvent();
	return true;
}

void TestConfigConstructor::emitEvent() {
	emit valueChanged();
}

QString TestConfigConstructor::getTotalTestCount() const {
	return QString::number(this->totalTestCount);
}

QString TestConfigConstructor::getSingleTestCount() const {
	return QString::number(this->singleTestCount);
}

QString TestConfigConstructor::getTotalTestInterval() const {
	return QString::number(this->totalTestInterval.count());
}

QString TestConfigConstructor::getSingleTestInterval() const {
	return QString::number(this->singleTestInterval.count());
}

int TestConfigConstructor::getNetworkType() const {
	return (int) this->testNetworkType;
}

QString TestConfigConstructor::getSourcePort() const {
	return QString::number(this->sourcePort);
}

QString TestConfigConstructor::getDestinationAddress() const {
	return QString::fromStdString(this->destinationAddress);
}

QString TestConfigConstructor::getDestinationPort() const {
	return QString::number(this->destinationPort);
}

QString TestConfigConstructor::getCustomData() const {
	return this->customData;
}

QString TestConfigConstructor::getCustomDataLength() const {
	return QString::number(this->customDataLength);
}

TestConfigConstructor::TestConfigConstructor() = default;

TestConfigConstructor::~TestConfigConstructor() = default;
