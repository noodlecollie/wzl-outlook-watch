#pragma once

#include <MinimalWatchy.h>

class WatchyOutlook : public CustomWatchy::MinimalWatchy
{
public:
	void drawWatchFace() override;

private:
	void drawBackground();
	void drawTime();
	void drawSideBar();
};
