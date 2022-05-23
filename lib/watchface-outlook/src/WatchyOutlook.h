#pragma once

#include <MinimalWatchy.h>

class WatchyOutlook : public CustomWatchy::MinimalWatchy
{
public:
	void drawWatchFace() override;

private:
	void drawTime();
	void drawMessage();
	void drawBattery();

	float getBatteryVoltage() const;
};
