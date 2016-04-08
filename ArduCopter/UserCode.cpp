/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "Copter.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <unistd.h>

static void _wdtOpen(int *fd);      // local file open wrapper
static void _wdtClose(int *fd);     // local file close wrapper

int _deviceHandle;              // Device file handler
int _watchdogTimeout = 15;
bool _watchdogPreRun = false;   // First run, previously run


#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here
}
#endif

#ifdef USERHOOK_WATCHDOGLOOP
void Copter::userhook_WatchDogLoop()
{

    // WatchDog Timer parameter Enabled
     if (g.wdt_enabled){

         _wdtOpen(&_deviceHandle);

         if (_watchdogPreRun == false){
             ioctl(_deviceHandle, WDIOC_SETTIMEOUT, &_watchdogTimeout); //Set watchdot default timeout
             _watchdogPreRun = true;            // Watchdog timer first run
             }

         if (ioctl(_deviceHandle, WDIOC_KEEPALIVE, 0) < 0)     // Reset the watchdog timer
             hal.console->println("Failed to reset Watchdog Timer!");

         _wdtClose(&_deviceHandle);

     }else if (_watchdogPreRun){         // Has the watchdog timer previously been enabled && WDT_ENABLED parameter been disabled

         _wdtOpen(&_deviceHandle);

         if (write(_deviceHandle,"V",1) < 0)       // Disable Watchdog timer
             hal.console->println("Failed to stop watchdog Device");

         _watchdogPreRun = false;                // Watchdog timer has been disabled so it has not been previously run
         _wdtClose(&_deviceHandle);
     }
 }
#endif

 // Static file open, pass device handler as pointer
 static void _wdtOpen(int *fd)
 {
     if ((*fd = open("/dev/watchdog",O_RDWR | O_NOCTTY)) < 0) //watchdog is automatically started when opened
         hal.console->println("Failed to open watchdog Device");

 }

 // Static file close, pass device handler as pointer
 static void _wdtClose(int *fd)
 {
     if (close(*fd) < 0)                // Close the handle
         hal.console->println("Failed to close watchdog device");
 }
