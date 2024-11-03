#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 27      // Hier wird angegeben, an welchem digitalen Pin die WS2812 LEDs bzw. NeoPixel angeschlossen sind
#define NUMPIXELS 4 // Hier wird die Anzahl der angeschlossenen WS2812 LEDs bzw. NeoPixel angegeben

#include <genericstate.h>
#include <SimpleTimer.h>


class MyNeoPixelHandler: public SimpleTimer, public Adafruit_NeoPixel
{
private:
    /* data */

    uint16_t i_actualSunPhase     = 0;
    uint16_t i_maxSunPhase        = 500;
    uint16_t i_wakeDelayInMinutes = 20; 
    uint16_t i_loopDealy          = 100;
    uint16_t i_maxBrightness      = 255;

    u_int16_t iRed   = 200;
    u_int16_t iGreen = 80;
    u_int16_t iBlue  = 50;
    u_int16_t iWhite = 10;

    uint32_t iColor;

    uint16_t iTime;
    timer_callback ptrTimerCB;

public:
    MyNeoPixelHandler( uint16_t n, int16_t p, neoPixelType t );
    ~MyNeoPixelHandler();

    void setTimerCB( timer_callback ptrTimerCB );

    void lightOn();
    void lightOff();
    void letSunRise();

    void startSunRise();

    const char *getLightState(){return lightState->getLightState();}

    uint16_t getLoopDelay(){ return this->i_loopDealy;};
    void setLoopDelay(uint16_t value){ this->i_loopDealy=value;};

    uint16_t getWakeDelayInMinutes(){ return this->i_wakeDelayInMinutes;};
    void setWakeDelayInMinutes(uint16_t value ){ this->i_wakeDelayInMinutes=value;};

    uint16_t getMaxBrightness(){ return this->i_maxBrightness;};
    void  setMaxBrightness(uint16_t value){ this->i_maxBrightness=value;};

    u_int16_t getIRed() const { return iRed; }
    void setIRed(u_int16_t value) { iRed = value;}

    u_int16_t getIGreen() const {return iGreen; }
    void setIGreen(u_int16_t value) {iGreen = value; }

    u_int16_t getIBlue() const {return iBlue;}
    void setIBlue(u_int16_t value) {iBlue = value; }

    u_int16_t getIWhite() const {return iWhite;}
    void setIWhite(u_int16_t value) {iWhite = value;}

    uint32_t calcColorValues(uint16_t i_Brightness);
    

/**
 * @brief Statemachine for alarm
 * 
 */
private:

    void createTimer();


    /**
     * @brief default unhandelt event function
     * 
     * @param str 
     */
    static void unhandledEvent(const String &str) 
    { 
        Serial.printf("unhandled event in %s\n", str.c_str()); 
    }

    /**
     * @brief generic alarm state
     * 
     */
    struct LightState : public GenericState<MyNeoPixelHandler, LightState> 
    {
        using GenericState::GenericState;
        virtual void lightOn() { unhandledEvent( "LightOn" ); }
        virtual void lightOff(){ unhandledEvent( "LightOff"); }
        virtual void sunRise() { unhandledEvent( "SunRise" ); }

        virtual const char *getLightState(){ unhandledEvent( "LightState"); return "unhandledEvent"; }
    };
    StateRef<LightState> lightState;
    /**
     * @brief LightOn
     * 
     */
    struct LightOn : public LightState 
    {
        using LightState::LightState;
        void entry() 
        { 
            Serial.println("-> entry LightOn");

            stm.fill(stm.Color(255,255,255,255), 0, stm.numPixels());
            
            return;
        
        }
        void lightOn() 
        { 
            return; 
        }
        void lightOff() 
        {
            change<LightOff>();
        }
        void sunRise()
        {
            change<SunRise>();
        }
        const char *getLightState()
        {
            return "LightOn";
        }
        void exit() 
        { 
            Serial.println("<- exit LightOn");
            return;
        }
    };
    /**
     * @brief LightOff
     * 
     */
    struct LightOff : public LightState 
    {
        using LightState::LightState;
        void entry() 
        { 
            Serial.println("-> entry LightOff");
            stm.fill(stm.Color(0,0,0,0), 0, stm.numPixels());
            return; 
        }
        void lightOff() 
        { 
            return; 
        }
        void lightOn() 
        {
            change<LightOn>();
            return;
        }
        void sunRise()
        {
            change<SunRise>();
        }
        const char *getLightState()
        {
            return "LightOff";
        }
        void exit() 
        { 
            Serial.println("<- exit LightOff");
            return;
        }
    };
    /**
     * @brief SunRise
     * 
     */
    struct SunRise : public LightState 
    {
        using LightState::LightState;
        void entry() 
        { 
            Serial.println("-> entry SunRise");
            stm.createTimer();
            return; 
        }
        void lightOff() 
        { 
            return; 
        }
        void lightOn() 
        {
            change<LightOn>();
            return;
        }
        void sunRise()
        {
            return;
        }
        const char *getLightState()
        {
            return "SunRise";
        }
        void exit() 
        { 
            Serial.println("<- exit SunRise");
            return;
        }
    };

};

