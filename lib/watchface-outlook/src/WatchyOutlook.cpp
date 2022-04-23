#include "WatchyOutlook.h"
#include "DSEG7_Classic_Bold_53.h"
#include "images/maple_threshold.h"
#include "images/icon_bt.h"

void WatchyOutlook::drawWatchFace()
{
	m_Display.fillScreen(GxEPD_WHITE);
	m_Display.setTextColor(GxEPD_BLACK);

	drawBackground();
	drawTime();
	drawSideBar();
}

void WatchyOutlook::drawBackground()
{
	m_Display.drawBitmap(
		0,
		0,
		Image_maple_threshold::DATA,
		Image_maple_threshold::WIDTH,
		Image_maple_threshold::HEIGHT,
		GxEPD_BLACK
	);
}

void WatchyOutlook::drawTime()
{
	m_Display.setFont(&DSEG7_Classic_Bold_53);
	m_Display.setCursor(5, DISPLAY_HEIGHT);

	m_Display.fillRect(
		0,
		Image_maple_threshold::HEIGHT,
		DISPLAY_WIDTH,
		(DISPLAY_HEIGHT - Image_maple_threshold::HEIGHT),
		GxEPD_WHITE
	);

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

void WatchyOutlook::drawSideBar()
{
	static constexpr uint16_t SIDEBAR_WIDTH = 20;
	static constexpr uint16_t SIDEBAR_X = DISPLAY_WIDTH - SIDEBAR_WIDTH;
	static constexpr uint16_t CIRCLE_RADIUS = 9;
	static constexpr uint16_t CIRCLE_MARGIN = (SIDEBAR_WIDTH - (2 * CIRCLE_RADIUS)) / 2;

	m_Display.fillRect(SIDEBAR_X, 0, SIDEBAR_WIDTH, DISPLAY_HEIGHT, GxEPD_WHITE);

	m_Display.fillCircle(
		SIDEBAR_X + CIRCLE_RADIUS + CIRCLE_MARGIN,
		CIRCLE_RADIUS + CIRCLE_MARGIN,
		CIRCLE_RADIUS,
		GxEPD_BLACK
	);

	m_Display.drawBitmap(
		SIDEBAR_X + (Image_icon_bt::WIDTH / 2),
		2,
		Image_icon_bt::DATA,
		Image_icon_bt::WIDTH,
		Image_icon_bt::HEIGHT,
		GxEPD_WHITE
	);
}
