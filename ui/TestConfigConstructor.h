#ifndef NETDESIGN1_TESTCONFIGCONSTRUCTOR_H
#define NETDESIGN1_TESTCONFIGCONSTRUCTOR_H

#include "chrono"
#include "QObject"
#include <QtCore/QString>
#include "TestConfig.h"

class TestConfigConstructor : public QObject {

	Q_OBJECT

private:

	unsigned int totalTestCount = 0;
	unsigned int singleTestCount = 0;
	std::chrono::microseconds totalTestInterval = std::chrono::microseconds(0);
	std::chrono::microseconds singleTestInterval = std::chrono::microseconds(0);

	tc::TestNetworkType testNetworkType = tc::TestNetworkType::UDP;
	unsigned short sourcePort = 0;
	std::string_view destinationAddress;
	unsigned short destinationPort = 0;

	QString customData;
	unsigned int customDataLength = 0;

public:

	TestConfigConstructor();

	~TestConfigConstructor() override;

	TestConfig * build();

	TestConfig *construct();

	bool loadFromFile(const QString& path);

public slots:

	void setTotalTestCount(const QString& q);

	void setSingleTestCount(const QString& q);

	void setTotalTestInterval(const QString& q);

	void setSingleTestInterval(const QString& q);

	void setNetworkType(int index);

	void setSourcePort(const QString& q);

	void setDestinationAddress(const QString& q);

	void setDestinationPort(const QString& q);

	void setCustomData(const QString& q);

	void setCustomDataLength(const QString& q);
};


#endif //NETDESIGN1_TESTCONFIGCONSTRUCTOR_H
