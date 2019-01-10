# Arduino IR LED Strip Controller

This Arduino project permits to control LED strips with the RC5 infrared protocol.
CAUTION: This controller is only a receiver. If you want to use it you must respect its protocol explained below.
I've made this project to be linked with a raspberry pi 3 that commands it; I will upload the code soon.

## Getting Started
### Prerequisites

You need an Arduino of course, an infrared sensor and a LED strip working with a data wire.
I use two libraries that are the [FastLED library](http://fastled.io/) and [this RC5 decoder library](https://github.com/guyc/RC5). You need do add them to your Arduino IDE.

### Installing
You need to include the two libraries in the project and just upload it in your Arduino and it works!
Of course you can change the pin numbers at the beginning of the code.

## What you need to know
Each controller has an id variable, so you can control several LED strips with several Arduinos.

### Send what ?
The RC5 protocol have frames of 14 bits. To change the color, this controller needs to receive 6 frames:
* One frame for each color value. Red first, green after and blue at the end
* Each frame must be sent twice to check errors

### The strucuture of a frame
Each frame needs to contain the controller destination ID of 4 bits (so there's 15 possible destinations) and a value of 8 bits. Here is the structure of a frame:
```
2 usless bits | destination | value = 14 bits
```

### Example
Here we'll send the RGB color (254, 126, 187) to the controller of destination ID number 0.
```
254 in binary: 11111110
126 in binary: 01111110
187 in binary: 10111011

Frame 1: 00000011111110
Frame 2: 00000011111110
Frame 3: 00000001111110
Frame 4: 00000001111110
Frame 5: 00000010111011
Frame 6: 00000010111011
```