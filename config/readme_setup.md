# Internet over USB
To enable internet after each boot 'startUSBnetwork' has been added to /etc/rc.local

# Set current date
Need to run setLocalTime to set the local time
Only after internet has been enabled can the current time be set. Basically, no time availble if no internet.
'setTime' has been added to /etc/rc.local to set the time at boot. 
