#include <time.h>
#include "MinimalWatchyRTC.h"

namespace CustomWatchy
{
	static constexpr uint8_t RTC_DS_ADDR = 0x68;
	static constexpr uint8_t RTC_PCF_ADDR = 0x51;

	MinimalWatchyRTC::MinimalWatchyRTC()
		: rtc_ds(false)
	{
	}

	void MinimalWatchyRTC::init()
	{
		byte error;
		Wire.beginTransmission(RTC_DS_ADDR);
		error = Wire.endTransmission();

		if(error == 0)
		{
			rtcType = RTCType::DS3231;
		}
		else
		{
			Wire.beginTransmission(RTC_PCF_ADDR);
			error = Wire.endTransmission();

			if(error == 0)
			{
				rtcType = RTCType::PCF8563;
			}
			else
			{
				//RTC Error
			}
		}
	}

	// String datetime format is YYYY:MM:DD:HH:MM:SS
	void MinimalWatchyRTC::config(String datetime)
	{
		if(rtcType == RTCType::DS3231)
		{
			_DSConfig(datetime);
		}
		else
		{
			_PCFConfig(datetime);
		}
	}

	void MinimalWatchyRTC::clearAlarm()
	{
		if(rtcType == RTCType::DS3231)
		{
			rtc_ds.alarm(DS3232RTC::ALARM_2);
		}
		else
		{
			int nextAlarmMinute = 0;
			rtc_pcf.clearAlarm(); //resets the alarm flag in the RTC
			nextAlarmMinute = rtc_pcf.getMinute();
			nextAlarmMinute = (nextAlarmMinute == 59) ? 0 : (nextAlarmMinute + 1); //set alarm to trigger 1 minute from now
			rtc_pcf.setAlarm(nextAlarmMinute, 99, 99, 99);
		}
	}

	void MinimalWatchyRTC::read(tmElements_t &tm)
	{
		if(rtcType == RTCType::DS3231)
		{
			rtc_ds.read(tm);
		}
		else
		{
			tm.Year = y2kYearToTm(rtc_pcf.getYear());
			tm.Month = rtc_pcf.getMonth();
			tm.Day = rtc_pcf.getDay();
			tm.Wday = rtc_pcf.getWeekday() + 1;  //TimeLib & DS3231 has Wday range of 1-7, but PCF8563 stores day of week in 0-6 range
			tm.Hour = rtc_pcf.getHour();
			tm.Minute = rtc_pcf.getMinute();
			tm.Second = rtc_pcf.getSecond();
		}
	}

	void MinimalWatchyRTC::set(tmElements_t tm)
	{
		if(rtcType == RTCType::DS3231)
		{
			time_t t = makeTime(tm);
			rtc_ds.set(t);
		}
		else
		{
			time_t t = makeTime(tm); //make and break to calculate tm.Wday
			breakTime(t, tm);
			//day, weekday, month, century(1=1900, 0=2000), year(0-99)
			rtc_pcf.setDate(tm.Day, tm.Wday - 1, tm.Month, 0, tmYearToY2k(tm.Year)); //TimeLib & DS3231 has Wday range of 1-7, but PCF8563 stores day of week in 0-6 range
			//hr, min, sec
			rtc_pcf.setTime(tm.Hour, tm.Minute, tm.Second);
			clearAlarm();
		}
	}

	uint8_t MinimalWatchyRTC::temperature()
	{
		if(rtcType == RTCType::DS3231)
		{
			return rtc_ds.temperature();
		}
		else
		{
			return 255; //error
		}
	}

	// String datetime is YYYY:MM:DD:HH:MM:SS
	void MinimalWatchyRTC::_DSConfig(String datetime)
	{
		if(datetime != "")
		{
			tmElements_t tm;
			tm.Year = CalendarYrToTm(_getValue(datetime, ':', 0).toInt()); //YYYY - 1970
			tm.Month = _getValue(datetime, ':', 1).toInt();
			tm.Day = _getValue(datetime, ':', 2).toInt();
			tm.Hour = _getValue(datetime, ':', 3).toInt();
			tm.Minute = _getValue(datetime, ':', 4).toInt();
			tm.Second = _getValue(datetime, ':', 5).toInt();
			time_t t = makeTime(tm);
			rtc_ds.set(t);
		}

		//https://github.com/JChristensen/DS3232RTC
		rtc_ds.squareWave(DS3232RTC::SQWAVE_NONE); //disable square wave output
		rtc_ds.setAlarm(DS3232RTC::ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
		rtc_ds.alarmInterrupt(DS3232RTC::ALARM_2, true); //enable alarm interrupt
	}

	// String datetime is YYYY:MM:DD:HH:MM:SS
	void MinimalWatchyRTC::_PCFConfig(String datetime)
	{
		if(datetime != "")
		{
			tmElements_t tm;
			tm.Year = CalendarYrToTm(_getValue(datetime, ':', 0).toInt()); //YYYY - 1970
			tm.Month = _getValue(datetime, ':', 1).toInt();
			tm.Day = _getValue(datetime, ':', 2).toInt();
			tm.Hour = _getValue(datetime, ':', 3).toInt();
			tm.Minute = _getValue(datetime, ':', 4).toInt();
			tm.Second = _getValue(datetime, ':', 5).toInt();
			time_t t = makeTime(tm);  //make and break to calculate tm.Wday
			breakTime(t, tm);
			//day, weekday, month, century(1=1900, 0=2000), year(0-99)
			rtc_pcf.setDate(tm.Day, tm.Wday - 1, tm.Month, 0, tmYearToY2k(tm.Year)); //TimeLib & DS3231 has Wday range of 1-7, but PCF8563 stores day of week in 0-6 range
			//hr, min, sec
			rtc_pcf.setTime(tm.Hour, tm.Minute, tm.Second);
		}

		//on POR event, PCF8563 sets month to 0, which will give an error since months are 1-12
		clearAlarm();
	}

	String MinimalWatchyRTC::_getValue(String data, char separator, int index)
	{
		int found = 0;
		int strIndex[] = {0, -1};
		int maxIndex = data.length()-1;

		for(int i=0; i<=maxIndex && found<=index; i++){
			if(data.charAt(i)==separator || i==maxIndex){
				found++;
				strIndex[0] = strIndex[1]+1;
				strIndex[1] = (i == maxIndex) ? i+1 : i;
			}
		}

		return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
	}
}
