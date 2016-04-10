# ardupilot-navio2
Watchdog Timer implementation for Navio 2

The fix is not perfect and a proper fix will be to modify the MCU to add a fail safe if the ArduPilot program dies. There is currently a fail safe implemented in the MCU co-processor if the raspberry pi dies (powers off) or if the Kernel panics, but not if the ArduPilot program dies.

The temporary fix essentially enables the watchdog timer in the raspberry pi and the ArduPilot application "pats the dog" or resets the watchdog timer from timing out and re-booting the raspberry pi. This will reboot the raspberry pi in the event that the ArduPilot application dies. I have included a paramter that can be enabled or disabled from Mission Planner in the event you need to frequently stop or start the applicaiton for testing purposes. The default paramter is set to disable the watchdog timer (ie. disabled = 0)

First you will load the watchdog kernel module by issuing the following command:

$ sudo modprobe bcm2708_wdog

This loads the watchdog kernel module, but doesn't start the timer... I'll explain how that happens in a bit.

$ lsmod

This should list all the kernel modules that are loaded, you should see the following:

bcm2708_wdog 3537 0

The watchdog kernel module should now be loaded, however you will have to make this permanent if a re-boot happens. To do this you will have to modify the /etc/modules file (using nano or vim) and include the following:

bcm2708_wdog

Close and save the file. Next you will be modifying the following six files in the ArudPilot source tree, which can be downloaded below. Add these into the ArduPilot/Copter or ArduPilot/Plane directories.

The watchdog timer will only start when you open the /dev/watchdog file. When it's opened for read/write, the timer is started automatically and begins its countdown. At this point, if you fail to write any character (except the special "V" character) to the device file, the timer will timeout and reboot the Rpi. Writing a value to the file will reset the counter, this is known as "patting the dog" as you are constantly resetting the timer to stop it from timing out and resetting the Rpi. Writing the special character "V" will disable the watchdog timer.
