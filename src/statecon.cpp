#include "statecon.hpp"

namespace app {
	
StateCon* StateCon::stateCon = nullptr;
Log* StateCon::logger = nullptr;

StateCon::StateCon() {
	logger = Log::getLog();
}

StateCon::~StateCon() {}

StateCon* StateCon::getStateCon() {
	if (stateCon == nullptr)
		stateCon = new StateCon();
	return stateCon;
}

void StateCon::releaseStateCon() {
	if (stateCon != nullptr)
		delete stateCon;
	stateCon = nullptr;
}

void StateCon::setAppState(StatesId appState) {
	this->appState = appState;
}

StateCon::StatesId StateCon::getAppState() {
	return this->appState;
}

}