#include "controller/Authentication.h"
#include "controller/WinHttp.h"
#include "utility/RequestError.h"
#include "utility/Toolbox.h"

Auth Authentication::getAuth()
{
	return m_authenticationData;
}

const char* AuthenticationError::what() const throw()
{
	return "The refresh token was refused. Please try again.";
}