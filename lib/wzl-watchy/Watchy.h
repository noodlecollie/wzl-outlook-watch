#pragma once

#include <Arduino.h>

struct watchySettings
{
	//Weather Settings
	String cityID;
	String weatherAPIKey;
	String weatherURL;
	String weatherUnit;
	String weatherLang;
	int8_t weatherUpdateInterval;
	//NTP Settings
	String ntpServer;
	int gmtOffset;
	int dstOffset;
};

class Watchy
{
public:
	explicit Watchy(const watchySettings&)
	{
	}

	void init()
	{
	}
};
