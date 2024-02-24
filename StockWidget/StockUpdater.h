#pragma once
#include <atomic>

struct StockUpdater
{
public:
	StockUpdater();

	void accept();
	void terminate();

	bool is_terminated;
	
};

