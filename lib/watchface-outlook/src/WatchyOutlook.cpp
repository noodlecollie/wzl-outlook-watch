#include "WatchyOutlook.h"
#include "DSEG7_Classic_Bold_53.h"
#include "GeosansLight10pt7b.h"

void WatchyOutlook::drawWatchFace()
{
	m_Display.fillScreen(GxEPD_WHITE);
	m_Display.setTextColor(GxEPD_BLACK);

	drawTime();
	drawMessage();
}

void WatchyOutlook::drawTime()
{
	static constexpr int16_t FONT_HEIGHT = 53;
	static constexpr int16_t LEFT_MARGIN = 5;
	static constexpr int16_t TOP_MARGIN = 0;

	m_Display.setFont(&DSEG7_Classic_Bold_53);
	m_Display.setCursor(LEFT_MARGIN, FONT_HEIGHT + TOP_MARGIN);

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

void WatchyOutlook::drawMessage()
{
	static constexpr int16_t FONT_HEIGHT = 14;
	static constexpr int16_t TOP_MARGIN = 55;

	m_Display.setFont(&GeosansLight10pt7b);
	m_Display.setCursor(0, FONT_HEIGHT + TOP_MARGIN);
	m_Display.print("This is a very long string that won't fit on the screen");
}
