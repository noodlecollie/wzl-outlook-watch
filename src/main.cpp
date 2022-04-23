#ifndef WATCH_CLASS_NAME
#error No watch class name defined!
#endif

#define WATCH_HEADER_NAME(name) <name.h>
#include WATCH_HEADER_NAME(WATCH_CLASS_NAME)

static WATCH_CLASS_NAME instance;

void setup()
{
	instance.init();
}

void loop()
{
}
