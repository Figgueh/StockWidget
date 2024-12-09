#pragma once

#include "include/controller/Authentication.h"
#include "include/model/twelvedata/TAuth.h"

class TAuthentication : public Authentication
{
	void const authenticate();

public:
	TAuth* m_auth;
};
