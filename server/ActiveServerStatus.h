#ifndef NETDESIGN1_ACTIVESERVERSTATUS_H
#define NETDESIGN1_ACTIVESERVERSTATUS_H

#include "QObject"

namespace as {
	typedef enum {
		STOPPED,
		IDLE,
		TESTING,
		TESTED,
	} Status;
}

class ActiveServerStatus : public QObject{

	Q_OBJECT

private:

	as::Status status = as::STOPPED;

signals:
	void statusChanged(QString status);

public:

	void setStatus(as::Status status);

	as::Status getStatus() const;


};


#endif //NETDESIGN1_ACTIVESERVERSTATUS_H
