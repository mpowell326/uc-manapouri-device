# Internet over USB
To enable internet after each boot 'startUSBnetwork' has been added to /etc/rc.local

# Set current date
Need to run setLocalTime to set the local time
Only after internet has been enabled can the current time be set. Basically, no time availble if no internet.
'setTime' has been added to /etc/rc.local to set the time at boot. 


# Wifi Dongle
lsusb shows:
    Bus 001 Device 002: ID 0bda:8179 Realtek Semiconductor Corp. 

After some googling this appears to be a "USB: 0BDA:8179 Realtek Semiconductor Corp. RTL8188EUS 802.11n Wireless Network Adapter"   
    https://wiki.debian.org/rtl819x#supported-r8188eu



Need to install the linux-headers for current kernel 'uname -a'
    sudo apt-get install build-essential linux-headers-3.8.13-bone79


    git clone git://github.com/lwfinger/rtl8188eu.git
    cd rtl8188eu
    CONFIG_RTL8188EU=m make -C /usr/src/linux-headers-3.8.13-bone30 M=`pwd`
    sudo cp 8188eu.ko /lib/modules/`uname -r`/kernel/drivers/net/wireless/
    sudo depmod -a

    sudo insmod 8188eu.ko
    lsmod
    iwconfig
    sudo ifconfig wlan0 up
    ifconfig

Scan for availabel wifi networks
    sudo iwlist wlan0 scanning


Update ssid and password in sudo nano /etc/network/interfaces



maybe useful:
    modinfo
    lsmod
    uname -a