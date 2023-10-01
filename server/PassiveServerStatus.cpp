#include "PassiveServerStatus.h"

static QString statusStrings[] = {
	"Status: Stopped",
	"Status: Running",
	"Status: Running With Error",
};

void PassiveServerStatus::setStatus(ps::Status status) {
	if (this->status == status)
		return;
	this->status = status;
	emit statusChanged(statusStrings[status]);
}

ps::Status PassiveServerStatus::getStatus() const {
	return this->status;
}
