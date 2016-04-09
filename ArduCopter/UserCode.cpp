/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "Copter.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <unistd.h>

static void wdtDevOpen(int *fd);      // local file open wrapper
static void wdtDevClose(int *fd);     // local file close wrapper

static int deviceHandle;              // Device file handler
static int watchdogTimeout = 15;
static bool watchdogPreRun = false;   // First run, previously run


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

         wdtDevOpen(&deviceHandle);

         if (watchdogPreRun == false){
             ioctl(deviceHandle, WDIOC_SETTIMEOUT, &watchdogTimeout); //Set watchdot default timeout
             watchdogPreRun = true;            // Watchdog timer first run
             }

         if (ioctl(deviceHandle, WDIOC_KEEPALIVE, 0) < 0)     // Reset the watchdog timer
             hal.console->println("Failed to reset Watchdog Timer!");

         wdtDevClose(&deviceHandle);

     }else if (watchdogPreRun){         // Has the watchdog timer previously been enabled && WDT_ENABLED parameter been disabled

         wdtDevOpen(&deviceHandle);

         if (write(deviceHandle,"V",1) < 0)       // Disable Watchdog timer
             hal.console->println("Failed to stop watchdog Device");

         watchdogPreRun = false;                // Watchdog timer has been disabled so it has not been previously run
         wdtDevClose(&deviceHandle);
     }
 }
#endif

 static void wdtDevOpen(int *fd)
 {
     if ((*fd = open("/dev/watchdog",O_RDWR | O_NOCTTY)) < 0) //watchdog is automatically started when opened
         hal.console->println("Failed to open watchdog Device");

 }

 static void wdtDevClose(int *fd)
 {
     if (close(*fd) < 0)                // Close the handle
         hal.console->println("Failed to close watchdog device");
 }
