#ifndef NETDESIGN1_PASSIVESERVERSTATUS_H
#define NETDESIGN1_PASSIVESERVERSTATUS_H

#include "QObject"
#include "string"

namespace ps {
	typedef enum {
		STOPPED,
		RUNNING,
		RUNNING_WITH_ERROR,
	} Status;
}

class PassiveServerStatus : public QObject {

	Q_OBJECT

private:

	ps::Status status = ps::STOPPED;

signals:

	void statusChanged(QString status);

public:

	void setStatus(ps::Status status);

	ps::Status getStatus() const;

};


#endif //NETDESIGN1_PASSIVESERVERSTATUS_H
