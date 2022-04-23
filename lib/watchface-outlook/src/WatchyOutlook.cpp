#include "WatchyOutlook.h"
#include "images/maple_threshold.h"
#include "DSEG7_Classic_Bold_53.h"

void WatchyOutlook::drawWatchFace()
{
	m_Display.fillScreen(GxEPD_WHITE);
	m_Display.setTextColor(GxEPD_BLACK);

	m_Display.drawBitmap(0, 0, Image_maple_threshold::DATA, Image_maple_threshold::WIDTH, Image_maple_threshold::HEIGHT, GxEPD_BLACK);

	drawTime();
}

void WatchyOutlook::drawTime()
{
	m_Display.setFont(&DSEG7_Classic_Bold_53);
	m_Display.setCursor(5, 200);

	m_Display.fillRect(0, Image_maple_threshold::HEIGHT, 200, (200 - Image_maple_threshold::HEIGHT), GxEPD_WHITE);

	int displayHour = m_CurrentTime.Hour;

	if ( HOUR_12_24 == 12 )
	{
		displayHour = ((displayHour + 11) % 12) + 1;
	}

	if ( displayHour < 10 )
	{
		m_Display.print("0");
	}

	m_Display.print(displayHour);
	m_Display.print(":");

	if ( m_CurrentTime.Minute < 10 )
	{
		m_Display.print("0");
	}

	m_Display.println(m_CurrentTime.Minute);
}
