#include <stdbool.h>

#include <Arduino.h>

static volatile bool oneSecEvent;

/*
 *
 */
static void setupTimer1()
{
    // Clear registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // 1 Hz (16000000/((15624+1)*1024))
    OCR1A = 15624;
    // CTC
    TCCR1B |= (1 << WGM12);
    // Prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // Output Compare Match A Interrupt Enable
    TIMSK1 |= (1 << OCIE1A);
}

/*
 *
 */
ISR(TIMER1_COMPA_vect)
{
    oneSecEvent = true;
}

/*
 *
 */
void RtcHal_Init(void)
{
    setupTimer1();
    oneSecEvent = false;
}

/*
 *
 */
bool RtcHal_IsOneSecondEvent(void)
{
    bool result = false;

    if(oneSecEvent)
    {
        oneSecEvent = false;
        result = true;
    }

    return result;
}
