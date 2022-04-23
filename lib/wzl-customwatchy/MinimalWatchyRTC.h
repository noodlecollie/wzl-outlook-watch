#ifndef WATCHY_RTC_H
#define WATCHY_RTC_H

#include <DS3232RTC.h>
#include <Rtc_Pcf8563.h>
#include <TimeLib.h>
#include "config.h"

namespace CustomWatchy
{
	class MinimalWatchyRTC
	{
	public:
		enum class RTCType
		{
			DS3231,
			PCF8563
		};

		MinimalWatchyRTC();
		void init();
		void config(String datetime); //String datetime format is YYYY:MM:DD:HH:MM:SS
		void clearAlarm();
		void read(tmElements_t &tm);
		void set(tmElements_t tm);
		uint8_t temperature();

		DS3232RTC rtc_ds;
		Rtc_Pcf8563 rtc_pcf;
		RTCType rtcType;
	private:
		void _DSConfig(String datetime);
		void _PCFConfig(String datetime);
		int _getDayOfWeek(int d, int m, int y);
		String _getValue(String data, char separator, int index);
	};
}

#endif
