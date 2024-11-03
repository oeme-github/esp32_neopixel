#include <MyNeoPixelHandler.h>


MyNeoPixelHandler::MyNeoPixelHandler( uint16_t n, int16_t pin = 6,neoPixelType type = NEO_GRB + NEO_KHZ800 )
    : Adafruit_NeoPixel( n, pin, type)
{
    LightState::init<LightOff>(*this, lightState);
}

MyNeoPixelHandler::~MyNeoPixelHandler()
{
}


void MyNeoPixelHandler::lightOn()
{  
    lightState->lightOn();
    this->show();
}
void MyNeoPixelHandler::lightOff()
{ 
    lightState->lightOff(); 
    this->show();
}


void MyNeoPixelHandler::startSunRise()
{
    this->lightState->sunRise();
}

uint32_t MyNeoPixelHandler::calcColorValues(uint16_t i_Brightness)
{
    return this->Color(
        map(this->i_maxBrightness, 0, this->i_maxSunPhase, 0, this->iRed),
        map(this->i_maxBrightness, 0, this->i_maxSunPhase, 0, this->iGreen),
        map(this->i_maxBrightness, 0, this->i_maxSunPhase, 0, this->iBlue),
        map(this->i_maxBrightness, 0, this->i_maxSunPhase, 0, this->iWhite)
    );
}

void MyNeoPixelHandler::letSunRise()
{
    this->i_actualSunPhase++;

    // long x, long in_min, long in_max, long out_min, long out_max
    uint16_t i_Brightness = map(this->i_actualSunPhase, 0, this->i_maxSunPhase, 0, this->i_maxBrightness); 
    this->setBrightness( i_Brightness );

    this->fill(this->calcColorValues(i_Brightness), 0, this->numPixels());
    this->show();

    Serial.printf( "i_actualSunPhase[%i] i_maxSunPhase[%i] i_Brightness [%i]\n", this->i_actualSunPhase, this->i_maxSunPhase, i_Brightness);
    if(this->i_actualSunPhase>=this->i_maxSunPhase)
    {
        this->deleteTimer(this->iTime);
        this->lightState->lightOn();
    }
}

void MyNeoPixelHandler::setTimerCB( timer_callback ptrTimerCB_ )
{
    this->ptrTimerCB = ptrTimerCB_;
}

void MyNeoPixelHandler::createTimer()
{
    /* -------------------------------------------------- */
    /*  1 Sekunde entspricht 1.000 Millisekunden          */
    unsigned long ul_delay = this->i_wakeDelayInMinutes*60*(1000-this->i_loopDealy)/this->i_maxSunPhase;

    Serial.printf("ul_delay [%f]\n", ul_delay);

    /* -------------------------------------------------- */
    /* create the timer                                   */
    this->iTime = this->setInterval(ul_delay, this->ptrTimerCB);

}





