#pragma once
#include "include/model/Auth.h"

class TAuth : public Auth {
// No additional functions needed. Twelvedata authentication only contains a key.
public:
	TAuth(std::string apiKey) : Auth(apiKey) {};
};
