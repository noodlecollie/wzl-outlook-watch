#include <bma.h>
#include "MinimalWatchy.h"
#include "DSEG7_Classic_Bold_53.h"

RTC_DATA_ATTR bool g_DisplayFullInit = true;
RTC_DATA_ATTR BMA423 g_Sensor;

namespace CustomWatchy
{
	MinimalWatchyRTC MinimalWatchy::m_RTC;
	GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> MinimalWatchy::m_Display(GxEPD2_154_D67(CS, DC, RESET, BUSY));

	static void displayBusyCallback(const void*)
	{
		gpio_wakeup_enable((gpio_num_t)BUSY, GPIO_INTR_LOW_LEVEL);
		esp_sleep_enable_gpio_wakeup();
		esp_light_sleep_start();
	}

	static uint16_t readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
	{
		Wire.beginTransmission(address);
		Wire.write(reg);
		Wire.endTransmission();
		Wire.requestFrom((uint8_t)address, (uint8_t)len);

		uint8_t i = 0;

		while ( Wire.available() )
		{
			data[i++] = Wire.read();
		}

		return 0;
	}

	static uint16_t writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
	{
		Wire.beginTransmission(address);
		Wire.write(reg);
		Wire.write(data, len);

		return Wire.endTransmission() != 0;
	}

	void MinimalWatchy::init(String datetime)
	{
		esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

		//init i2c
		Wire.begin(SDA, SCL);

		m_RTC.init();

		// Init the m_Display here for all cases, if unused, it will do nothing
		m_Display.init(0, g_DisplayFullInit, 10, true); // 10ms by spec, and fast pulldown reset
		m_Display.epd2.setBusyCallback(displayBusyCallback);

		switch ( wakeup_reason )
		{
			case ESP_SLEEP_WAKEUP_EXT0: //m_RTC Alarm
			{
				m_RTC.read(m_CurrentTime);
				showWatchFace(true); //partial updates on tick

				break;
			}

			case ESP_SLEEP_WAKEUP_EXT1: //button Press
			{
				// TODO: Handle button press
				break;
			}

			default: //reset
			{
				m_RTC.config(datetime);
				bmaConfig();
				m_RTC.read(m_CurrentTime);
				showWatchFace(false); //full update on reset

				break;
			}
		}

		deepSleep();
	}

	void MinimalWatchy::deepSleep()
	{
		m_Display.hibernate();
		g_DisplayFullInit = false; // Notify not to init it again
		m_RTC.clearAlarm(); // resets the alarm flag in the m_RTC

		// Set pins 0-39 to input to avoid power leaking out
		for ( uint8_t pin = 0; pin < 40; ++pin )
		{
			pinMode(pin, INPUT);
		}

		// enable deep sleep wake on m_RTC interrupt
		esp_sleep_enable_ext0_wakeup(RTC_PIN, 0);

		// enable deep sleep wake on button press
		esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);

		esp_deep_sleep_start();
	}

	void MinimalWatchy::drawWatchFace()
	{
		m_Display.setFont(&DSEG7_Classic_Bold_53);
		m_Display.setCursor(5, 53+60);

		if ( m_CurrentTime.Hour < 10 )
		{
			m_Display.print("0");
		}

		m_Display.print(m_CurrentTime.Hour);
		m_Display.print(":");

		if ( m_CurrentTime.Minute < 10 )
		{
			m_Display.print("0");
		}

		m_Display.println(m_CurrentTime.Minute);
	}

	void MinimalWatchy::showWatchFace(bool partialRefresh)
	{
		m_Display.setFullWindow();
		drawWatchFace();
		m_Display.display(partialRefresh);
	}

	void MinimalWatchy::bmaConfig()
	{
		if ( !g_Sensor.begin(readRegister, writeRegister, delay) )
		{
			//fail to init BMA
			return;
		}

		// Accel parameter structure
		Acfg cfg;
		/*!
			Output data rate in Hz, Optional parameters:
				- BMA4_OUTPUT_DATA_RATE_0_78HZ
				- BMA4_OUTPUT_DATA_RATE_1_56HZ
				- BMA4_OUTPUT_DATA_RATE_3_12HZ
				- BMA4_OUTPUT_DATA_RATE_6_25HZ
				- BMA4_OUTPUT_DATA_RATE_12_5HZ
				- BMA4_OUTPUT_DATA_RATE_25HZ
				- BMA4_OUTPUT_DATA_RATE_50HZ
				- BMA4_OUTPUT_DATA_RATE_100HZ
				- BMA4_OUTPUT_DATA_RATE_200HZ
				- BMA4_OUTPUT_DATA_RATE_400HZ
				- BMA4_OUTPUT_DATA_RATE_800HZ
				- BMA4_OUTPUT_DATA_RATE_1600HZ
		*/
		cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
		/*!
			G-range, Optional parameters:
				- BMA4_ACCEL_RANGE_2G
				- BMA4_ACCEL_RANGE_4G
				- BMA4_ACCEL_RANGE_8G
				- BMA4_ACCEL_RANGE_16G
		*/
		cfg.range = BMA4_ACCEL_RANGE_2G;
		/*!
			Bandwidth parameter, determines filter configuration, Optional parameters:
				- BMA4_ACCEL_OSR4_AVG1
				- BMA4_ACCEL_OSR2_AVG2
				- BMA4_ACCEL_NORMAL_AVG4
				- BMA4_ACCEL_CIC_AVG8
				- BMA4_ACCEL_RES_AVG16
				- BMA4_ACCEL_RES_AVG32
				- BMA4_ACCEL_RES_AVG64
				- BMA4_ACCEL_RES_AVG128
		*/
		cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

		/*! Filter performance mode , Optional parameters:
			- BMA4_CIC_AVG_MODE
			- BMA4_CONTINUOUS_MODE
		*/
		cfg.perf_mode = BMA4_CONTINUOUS_MODE;

		// Configure the BMA423 accelerometer
		g_Sensor.setAccelConfig(cfg);

		// Enable BMA423 accelerometer
		// Warning : Need to use feature, you must first enable the accelerometer
		// Warning : Need to use feature, you must first enable the accelerometer
		g_Sensor.enableAccel();

		struct bma4_int_pin_config config ;
		config.edge_ctrl = BMA4_LEVEL_TRIGGER;
		config.lvl = BMA4_ACTIVE_HIGH;
		config.od = BMA4_PUSH_PULL;
		config.output_en = BMA4_OUTPUT_ENABLE;
		config.input_en = BMA4_INPUT_DISABLE;
		// The correct trigger interrupt needs to be configured as needed
		g_Sensor.setINTPinConfig(config, BMA4_INTR1_MAP);

		struct bma423_axes_remap remap_data;
		remap_data.x_axis = 1;
		remap_data.x_axis_sign = 0xFF;
		remap_data.y_axis = 0;
		remap_data.y_axis_sign = 0xFF;
		remap_data.z_axis = 2;
		remap_data.z_axis_sign = 0xFF;
		// Need to raise the wrist function, need to set the correct axis
		g_Sensor.setRemapAxes(&remap_data);

		// Enable BMA423 isStepCounter feature
		g_Sensor.enableFeature(BMA423_STEP_CNTR, true);
		// Enable BMA423 isTilt feature
		g_Sensor.enableFeature(BMA423_TILT, true);
		// Enable BMA423 isDoubleClick feature
		g_Sensor.enableFeature(BMA423_WAKEUP, true);

		// Reset steps
		g_Sensor.resetStepCounter();

		// Turn on feature interrupt
		g_Sensor.enableStepCountInterrupt();
		g_Sensor.enableTiltInterrupt();
		// It corresponds to isDoubleClick interrupt
		g_Sensor.enableWakeupInterrupt();
	}
}
