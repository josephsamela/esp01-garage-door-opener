# esp01-garage-door-opener

esp01 IoT device to open/close your garage door with Apple HomeKit

![](photos/photo-final-build.JPG)

## Software

This device can be controlled completely independent of any smart-home infastructure. It's simply an IoT "thing" you send HTTP GET requests to open/close your garage door.

That said, I personally am integrating this with Apple HomeKit. In order to do this you'll need a local instance of [Homebridge](https://homebridge.io/). You'll also need to install the [homebridge-http-garage](https://github.com/phenotypic/homebridge-http-garage) plugin for homebridge.

Once connected to your network, set a DHCP reservation so the device retains it's IP address. Then you can control it by sending HTTP requests to the routes below:

### Routes

The `/` endpoint is purely informational. It's helpful when setting up the garage door the first time.
```
/
```

The `/toggle` endpoint toggles the garage door. It's equivelant to you pushing the garage door button.

```
/toggle
```

The `status` endpoint will return json indicating the state of the garage door. `{"currentState":0}`
```
/status
```

The `setState` route accepts a query param called `value`. You can set state to `0` (open) or `1` (closed).
```
/setState?value=INT_VALUE
```

### Homebridge Configuration

I recommend following the default homebridge configuration suggested by the [homebridge-http-garage](https://github.com/phenotypic/homebridge-http-garage) plugin readme. Here's what I use:

```json
{
    "accessory": "GarageDoorOpener",
    "name": "Garage Door",
    "apiroute": "http://192.168.1.35",
    "openTime": 15,
    "closeTime": 15,
    "autoLock": false,
    "autoLockDelay": 20,
    "pollInterval": 15,
    "timeout": 5000,
    "http_method": "GET"
}
```

## Hardware

The hardware is an ESP-01 microcontroller with a relay module. The rear of the garage door unit has three screw terminals connected to the wall switch. The relay is connected between the two screw terminals connected to the garage door opener push-button. When the relay is closed the garage door unit recognizes this as a "button press" and opens/closes the door as if you pushed the button on the wall.

- [ESP-01 and Relay Board](https://www.aliexpress.us/item/2251832647269116.html) ~$2
- [Wired door sensor](aliexpress.us/item/2255800949489160.html) ~$1
- 10k Ohm Resistor. <$0.01
- 5V USB Charger (I used a spare phone charger)

## Installation

First wire the switch to the relay board with pull-down resistor. Then Wire the relay to garage door terminals as shown.

![](photos/screenshot1.jpg)

| ESP01 | Relay | Garage Door | Switch |
|:-----:|:-----:|:-----------:|:------:|
|  3.3V |  VCC  |             |        |
|  GND  |  GND  |             |  GND   |
|  D0   |   IN  |             |        |
|       |   NC  |   Switch 1  |        |
|       |   C   |   Switch 2  |        |
|  D3   |       |             |  POS   |

The GPIO pin `D3` is used for the wired door switch. The resistor is a pull-down resistor that you need to wire between `D3` to `GND`. I chose to solder the resistor to the bottom of the relay board.

## Hardware Mounting

To mount switch and to garage door components, I designed several parts in the `./switch-mount` folder. Both sides of the switch are screwed to the brackets. Then the brackets are pressed onto edge of the garage door track.

For the switch relay I printed [this case](https://www.thingiverse.com/thing:3976187) which has an easy to remove top.
