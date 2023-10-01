#include "ActiveServerStatus.h"

static QString statusStrings[] = {
	"Status: Stopped",
	"Status: Idle",
	"Status: Testing",
	"Status: Tested",
};

void ActiveServerStatus::setStatus(as::Status status) {
	if (this->status == status)
		return;
	this->status = status;
	emit statusChanged(statusStrings[status]);
}

as::Status ActiveServerStatus::getStatus() const {
	return this->status;
}
