# padisco
Pike Aero Protocol (PAP) Command Line Utility

Use padisco to discover Pike Aerospace embedded ethernet devices on your network.

Use padisco to modify Pike Aerospace embedded ethernet device settings.

```
padisco --help
PADisco Version 1.2.0
Copyright (c) 2019 Pike Aerospace Research Corporation
Sales & Support <sales@pikeaero.com> | http://www.pikeaero.com
--version     -v 
--help        -h -?
--timeout     -t <milliseconds>
--find        -f 
--host        -H <hostname>
--mac         -m xx:xx:xx:xx:xx:xx
--altmac      -m yy:yy:yy:yy:yy:yy
--dhcp        -d <1|0>
--ip          -i xxx.xxx.xxx.xxx
--sub         -s xxx.xxx.xxx.xxx
--gw          -g xxx.xxx.xxx.xxx
--setmac      -S 
--apply       -a 
--reset       -r 

Examples,
padisco --find
padisco --mac 00:20:6D:69:6E:3D --dhcp 0 -ip 192.168.1.10 --apply
padisco --mac 00:20:6D:69:6E:3D --host GATEWAY1 --apply
padisco --mac 00:20:6D:69:6E:3D --reset
padisco --mac 00:20:6D:69:6E:3D --altmac FE:22:34:56:78:90 --setmac 1 --apply
padisco --mac FF:FF:FF:FF:FF:FF --dhcp 1 --sub 255.255.255.0 --apply
```
