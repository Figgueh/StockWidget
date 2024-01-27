#include "RequestError.h"

const char* RequestError::what() const throw()
{
	return "A mistake was made with the sent request";
}