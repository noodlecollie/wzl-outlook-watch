#include "WatchyOutlook.h"
#include "images/maple_threshold.h"

static const watchySettings SETTINGS
{
	"",
	"",
	"",
	"metric",
	"en",
	30,
	"uk.pool.ntp.org",
	0,
	3600
};

WatchyOutlook::WatchyOutlook() :
	Watchy(settings)
{
}

void WatchyOutlook::drawWatchFace()
{
	display.fillScreen(GxEPD_WHITE);
	display.setTextColor(GxEPD_BLACK);

	display.drawBitmap(0, 0, Image_maple_threshold::DATA, Image_maple_threshold::WIDTH, Image_maple_threshold::HEIGHT, GxEPD_BLACK);

	drawTime();
}

void WatchyOutlook::drawTime()
{
	display.setFont(&DSEG7_Classic_Bold_53);
	display.setCursor(5, 200);

	display.fillRect(0, Image_maple_threshold::HEIGHT, 200, (200 - Image_maple_threshold::HEIGHT), GxEPD_WHITE);

	int displayHour = currentTime.Hour;

	if ( HOUR_12_24 == 12 )
	{
		displayHour = ((displayHour + 11) % 12) + 1;
	}

	if ( displayHour < 10 )
	{
		display.print("0");
	}

	display.print(displayHour);
	display.print(":");

	if ( currentTime.Minute < 10 )
	{
		display.print("0");
	}

	display.println(currentTime.Minute);
}
