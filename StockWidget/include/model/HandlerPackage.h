#pragma once
#include <controller/ConfigHandler.h>
#include "controller/RequestHandler.h"

struct HandlerPackage 
{
	Questrade::RequestHandler requestHandler;
	ConfigHandler* configurationHandler;
};
