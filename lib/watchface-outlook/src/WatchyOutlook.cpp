#include "WatchyOutlook.h"
#include "DSEG7_Classic_Bold_53.h"
#include "GeosansLight10pt7b.h"

void WatchyOutlook::drawWatchFace()
{
	m_Display.fillScreen(GxEPD_WHITE);
	m_Display.setTextColor(GxEPD_BLACK);

	drawTime();
	drawMessage();
	drawBattery();
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
	static constexpr int16_t TOP_MARGIN = 60;

	m_Display.setFont(&GeosansLight10pt7b);
	m_Display.setCursor(0, FONT_HEIGHT + TOP_MARGIN);
	m_Display.print("Go into the following\ndays neutrally, with an\nopen mind and without\nexpectation.");
}

void WatchyOutlook::drawBattery()
{
	float voltage = getBatteryVoltage();
	const char* batteryLevel = nullptr;

	if ( voltage > 4.1f )
	{
		batteryLevel = "Charged";
	}
	else if ( voltage > 3.95f && voltage <= 4.1f )
	{
		batteryLevel = "Good";
	}
	else if ( voltage > 3.80f && voltage <= 3.95f )
	{
		batteryLevel = "Low";
	}
	else if ( voltage <= 3.80f )
	{
		batteryLevel = "Very Low";
	}

	m_Display.setFont(&GeosansLight10pt7b);
	m_Display.setCursor(0, CustomWatchy::MinimalWatchy::DISPLAY_HEIGHT - 5);
	m_Display.print("Battery level: ");
	m_Display.print(batteryLevel);
}

float WatchyOutlook::getBatteryVoltage() const
{
	if ( m_RTC.rtcType == CustomWatchy::MinimalWatchyRTC::RTCType::DS3231 )
	{
		return analogReadMilliVolts(V10_ADC_PIN) / 1000.0f * 2.0f; // Battery voltage goes through a 1/2 divider.
	}
	else
	{
		return analogReadMilliVolts(V15_ADC_PIN) / 1000.0f * 2.0f;
	}
}
