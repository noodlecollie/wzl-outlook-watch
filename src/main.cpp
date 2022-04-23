#include "Watchy_7_SEG.h"

static watchySettings settings
{
	"5128581", // New York City https://openweathermap.org/current#cityid
	"f058fe1cad2afe8e2ddc5d063a64cecb", // use your own API key
	"http://api.openweathermap.org/data/2.5/weather?id=",
	"metric",
	"en",
	30,
	"pool.ntp.org",
	0,
	3600
};

Watchy7SEG watchy(settings);

void setup()
{
	watchy.init();
}

void loop()
{
}
