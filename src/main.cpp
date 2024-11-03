
#include <MyNeoPixelHandler.h>


MyNeoPixelHandler pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);


void SunRise()
{
  Serial.println("*** SunRise ***");
  pixels.letSunRise();
}


void setup() 
{
  Serial.begin(115200);
  Serial.println("START");
  pixels.begin();

  pixels.setBrightness(100);
  pixels.setTimerCB( SunRise );

  pixels.lightOn();
  delay(2000);
  pixels.lightOff();
  delay(2000);

  pixels.startSunRise();

}

void loop() 
{

  if( pixels.getLightState() == "SunRise" )
    pixels.run();
  delay(pixels.getLoopDelay());

}