#pragma once
#include "controller/Authentication.h"
#include "model/questrade/QAuth.h"

/**
* This class is the authentication point for questrade.
* The QAuth class is what is used for data access.
*/
class QAuthentication :public Authentication
{
	void const authenticate();
public:
	QAuth m_auth;
};
