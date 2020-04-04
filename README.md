<div align="left">
  <a><img src="docs/SVLabel.png" width = 150 height = 50 ></a><br>
</div>

| **`Docker`** | **`License`** |
|------------------|------------------|
|[![Build Status](https://travis-ci.com/Tyill/SVisual.svg?branch=master)](https://travis-ci.com/Tyill/SVisual)|[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)|

The SVisual software is designed to monitor the operation of the MK devices, debug the program, alert the user to the events that have occurred.

<p float="left">
<img src="https://github.com/Tyill/SVisual/blob/master/docs/sv-video.gif" width="400" height="300"/>
 &emsp;
<img src="https://github.com/Tyill/SVisual/blob/master/docs/sv-web.gif" width="400" height="300"/>
</p>

## Features

* connection to the MK via COM port (usb for arduino), over Ethernet or Wi-Fi protocol TCP;

* Interrogation of values of signals in real time with frequency before 100 Hz (default 10 Hz), the number of devices and signals is selected by the user;

* the permissible number of signals for recording 2048;

* output of the values of the selected signals to the monitor screen in real time;

* record the archive of signals to the PC hard disk;

* viewing the archive using additional SVViewer software;

* ability to set alerts for an event (triggers) that have occurred, start a custom process when the trigger is triggered;

* adding a signal for viewing / recording only by the client, no additional movements are required

* export data to xlsx, json or txt

* client UART+DMA for stm32f1 https://github.com/burrbull/svisual-stm32f1

* docker image https://hub.docker.com/r/tyiler/svdocker

* viewing in a web browser

## Demo

https://www.youtube.com/watch?v=LR0Rb964DLA&feature=youtu.be

https://www.youtube.com/watch?v=wlvArf7h6lE

https://www.youtube.com/watch?v=oBf4WSAncTQ&feature=youtu.be

## Arduino example

```cpp
#include <SVClient.h>
void setup() {

	bool ok = svisual::connectOfCOM("test");

}
int cnt = 0;
void loop() {

	svisual::addIntValue("dfv", cnt);

	++cnt;
	if (cnt > 10) cnt = 0;

	bool odd_cnt = cnt % 2;

    svisual::addBoolValue("bFW", odd_cnt);

    svisual::addBoolValue("bBW", !odd_cnt);

	delay(200);
}
```
### As a result
<div align="left">
  <a><img src="docs/example.png"></a><br>
</div>

## Use in Linux
Should be installed: qt, libz 

## [Manual](https://github.com/Tyill/SVisual/tree/master/man) 

## License
Licensed under an [MIT-2.0]-[license](LICENSE).




