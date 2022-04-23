#pragma once

#include <SilenceESP32Warnings.h>
#include <Arduino.h>
#include <UnsilenceESP32Warnings.h>

#include <GxEPD2_BW.h>
#include <TimeLib.h>
#include "MinimalWatchyRTC.h"

namespace CustomWatchy
{
	class MinimalWatchy
	{
	public:
		void init(String datetime = "");
		void deepSleep();

	protected:
		// Override this method for different watch faces
		virtual void drawWatchFace();

		static MinimalWatchyRTC m_RTC;
		static GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> m_Display;

		tmElements_t m_CurrentTime;

	private:
		void showWatchFace(bool partialRefresh);
		void bmaConfig();
	};
}
