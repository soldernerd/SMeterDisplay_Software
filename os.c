
#include <stdint.h>
#include <xc.h>
#include "os.h"
#include "lcd.h"
#include "i2c.h"
//#include "adc.h"


//12ms for normal load, 1ms for short load
#define TIMER0_LOAD_HIGH_48MHZ 0xB9
#define TIMER0_LOAD_LOW_48MHZ 0xB0
#define TIMER0_LOAD_SHORT_HIGH_48MHZ 0xFA
#define TIMER0_LOAD_SHORT_LOW_48MHZ 0x24

void tmr_isr(void)
{ 
    //Timer 0
    if(INTCONbits.T0IF)
    {
        if(os.done) 
        {
            //8ms until overflow
            TMR0H = TIMER0_LOAD_HIGH_48MHZ;
            TMR0L = TIMER0_LOAD_LOW_48MHZ;
            ++os.timeSlot;
            if(os.timeSlot==NUMBER_OF_TIMESLOTS)
            {
                os.timeSlot = 0;
            }
            os.done = 0;
        }
        else //Clock stretching
        {
            //1ms until overflow
            TMR0H = TIMER0_LOAD_SHORT_HIGH_48MHZ;
            TMR0L = TIMER0_LOAD_SHORT_LOW_48MHZ;
        }
        INTCONbits.T0IF = 0;
    }
}


static void _system_timer0_init(void)
{
    //Clock source = Fosc/4
    T0CONbits.T0CS = 0;
    //Operate in 16bit mode
    T0CONbits.T08BIT = 0;
    //Prescaler=8
    T0CONbits.T0PS2 = 0;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 0;
    //Use prescaler
    T0CONbits.PSA = 0;
    //8ms until overflow
    TMR0H = TIMER0_LOAD_HIGH_48MHZ;
    TMR0L = TIMER0_LOAD_LOW_48MHZ;
    //Turn timer0 on
    T0CONbits.TMR0ON = 1;
            
    //Enable timer0 interrupts
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.GIE = 1;
    
    //Initialize timeSlot
    os.timeSlot = 0;
}

void system_restore_default_calibration(void)
{
    os.calibration[0] = 4800;
    os.calibration[1] = 14400;
    os.calibration[2] = 24000;
    os.calibration[3] = 33600;
    os.calibration[4] = 43200;
    os.calibration[5] = 52800;
    os.calibration[6] = 62400;
    os.calibration[7] = 72000;
    os.calibration[8] = 81600;
    os.calibration[9] = 91200;
    os.calibration[10] = 100800;
    os.calibration[11] = 110400;
    os.calibration[12] = 120000;
    os.calibration[13] = 129600;
}

void system_load_calibration(void)
{
    uint8_t cntr;
    for(cntr=0; cntr<14; ++cntr)
    {
        os.calibration[cntr] = i2c_eeprom_calibration_read(cntr);
    }
}

void system_save_calibration(void)
{
    int8_t cntr;
    int32_t tmp;
    for(cntr=0; cntr<14; ++cntr)
    {
        tmp = i2c_eeprom_calibration_read(cntr);
        if(tmp != os.calibration[cntr]);
        {
            i2c_eeprom_calibration_write(os.calibration[cntr], cntr);
        }
    }
}

void system_init(void)
{
    //Set up timer0 for timeSlots
    _system_timer0_init();
    
    //Set up I2C
    i2c_init();

    //Set up LCD and display startup screen
    lcd_setup();
    lcd_init_4bit();
    lcd_refresh_all(); 
    
    //Load calibration data
    system_load_calibration();
}

