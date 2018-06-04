## SVisual

The SVisual software is designed to monitor the operation of the MK devices, debug the program, alert the user to the events that have occurred.

## Features

* connection to the MK via COM port (usb for arduino), over Ethernet or Wi-Fi protocol TCP;

* Interrogation of values of signals in real time with frequency from 100 Hz, the number of devices and signals is selected by the user;

* output of the values of the selected signals to the monitor screen in real time;

* record the archive of signals to the PC hard disk;

* allowable number of signals 2048, number of modules 8

* viewing the archive using additional SVViewer software;

* ability to set alerts for an event (triggers) that have occurred, start a custom process when the trigger is triggered;

* adds a signal to write only the client, that is, no additional instructions are required

## Demo

https://www.youtube.com/watch?v=LR0Rb964DLA&feature=youtu.be

https://www.youtube.com/watch?v=wlvArf7h6lE


## Arduino example

```cpp
#include <SVClient.h>
void setup() {

	bool ok = svisual::connectOfCOM("test");

}
int cnt = 0;
void loop() {

	svisual::addValue("dfv", cnt);

	++cnt;
	if (cnt > 10) cnt = 0;

	if (cnt % 2)  svisual::addValue("bFW", true);
	else  svisual::addValue("bFW ", false);

	if (!(cnt % 2))  svisual::addValue("bBW", true);
	else  svisual::addValue("bBW ", false);

	delay(200);
}
```
[As a result](docs/example.png)

## License
Licensed under an [MIT-2.0]-(https://github.com/Tyill/SVisual/LICENSE) license.




