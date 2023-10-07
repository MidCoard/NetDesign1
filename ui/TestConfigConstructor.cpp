#include <QFile>
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
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	QDataStream stream(&file);
	stream >> this->singleTestCount;
	stream >> this->totalTestCount;
	long long tmp;
	stream >> tmp;
	this->singleTestInterval = std::chrono::microseconds(tmp);
	stream >> tmp;
	this->totalTestInterval = std::chrono::microseconds(tmp);
	int tmp1;
	stream >> tmp1;
	this->testNetworkType = (tc::TestNetworkType) tmp1;
	stream >> this->sourcePort;
	QString tmp2;
	stream >> tmp2;
	this->destinationAddress = tmp2.toStdString();
	stream >> this->destinationPort;
	stream >> this->customData;
	this->customDataLength = this->customData.length();
	if (file.error() != QFile::NoError || stream.status() != QDataStream::Ok)
		return false;
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
