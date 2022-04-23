#pragma once

#include <Watchy.h>

class WatchyOutlook : public Watchy
{
public:
	WatchyOutlook();

	void drawWatchFace() override;

private:
	void drawTime();
};
