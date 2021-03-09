<!-- PROJECT LOGO -->
<p align="center">
  <a href="https://github.com/LiveSparks/ESP_Power_Button">
    <img src="https://i.imgur.com/VdzvRhn.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">ESP PC Power Button</h3>

  <p align="center">
    A Wi-Fi enabled replacement for the broken Power Button of my PC.
    <a href="https://github.com/LiveSparks/ESP_Power_Button">Watch The Video</a>
  </p>
</p>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#schematics">Schematics</a></li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#autohotkey">AutoHotKey</a>
        <ul>
            <li><a href="#some-examples">Some Examples</a></li>
        </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
<div align="center">
  <a href="https://www.youtube.com/watch?v=C6GKLckhQSw"><img src="https://img.youtube.com/vi/C6GKLckhQSw/0.jpg" alt="YouTube"></a>
</div>

Replace/Enhance the Power button of your PC with the ESP8266. No need to lift your arms to reach for the power button button of your PC when you can press it from the comfort of your phone. You can control more than just power with the included macro pad.

The project was made when the power button of my computer broke. I also wanted to add a few physical marco buttons and wanted to monitor some of the usage statistics remotely. This project is all that rolled into one.

> ⚠ Requires the [WinSerial](https://github.com/LiveSparks/WinSerial) program for full functionality. Read [Macro Pad & System Stats](#macro-pad-&-system-stats) for more info.

### Built With
* [Arduino](https://www.arduino.cc/)
* [PlatformIO](https://platformio.org/)
* [MinGW](http://mingw-w64.org/)



<!-- GETTING STARTED -->
## Getting Started

If you wish to replicate the project then you can either just grab the binary from the [releases page](https://github.com/LiveSparks/ESP_Power_Button/releases) and flash to your board with the [flash tool](https://www.espressif.com/en/support/download/other-tools).
**OR**
if you wish to make some changes to the code then follow the following steps:

### Prerequisites

I would recommend using PlatformIO. Instructions on how to install it are [here](https://platformio.org/platformio-ide). If you are indeed using PlatformIO, then skip this step.
* Arduino IDE
	* Make sure that the ESP8266 board configs are already installed. [Instructions](https://github.com/esp8266/Arduino#installing-with-boards-manager)
	* Make sure that the following libraries are installed. [How to install libraries in Arduino IDE](https://www.arduino.cc/en/guide/libraries).
		* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)
		* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
		* [ESPUI](https://github.com/s00500/ESPUI#manual-install-arduino-ide)
		* [ESPAsyncWiFiManager](https://github.com/alanswx/ESPAsyncWiFiManager#quick-start)

### Installation

1. Clone/Download the repo
   ```sh
   git clone https://github.com/LiveSparks/ESP_Power_Button
   ```
2. Compile & Upload
	* PlatformIO
		1. Open the repository folder in VS Code and upload after PlatformIO has loaded.
	* Arduino IDE
		1. In the source folder, rename `main.cpp` to `main.ino`.
		2. Open `main.ino` file in arduino IDE.
        3. Comment out `#include <Arduino.h>`.
        4. Connect the ESP Board and Upload.


## Schematic

![Schematic](https://i.imgur.com/PU3bBpa.png)

The Above diagram is made with the Wemos D1 mini in mind. Check to make sure that you use the correct GPIOs is you you are using a different board.

The USB headers on motherboards have the following layout:

![USB 2.0 Header on the motherboard.](https://i.imgur.com/NKY97qF.jpg)

You can also just connect to a external USB port directly if you want.

Regardless of whether you are connecting to the USB or not, you need to make sure that the ground of your PC and the ESP are connected together.


<!-- USAGE EXAMPLES -->
## Usage

The program works as is and there is no need to enter any WiFi Credentials in the code itself. 
1. On first bootup, a WiFi access point would be created by the name of ESP Power Button. Join it. 
2. Go to `192.168.4.1`. There you can enter your WiFi credentials.
3. Make sure to have a Serial Monitor open to check the connection status.
4. The ESP will reboot and connect to your home WiFi.
5. The new IP address will be written in serial monitor. You can also find the IP using the [Fing App](https://www.fing.com/products/fing-app). It would show up as a smart device.
6. (Optional) Assign the ESP a static IP from your router.
7. Go to it's IP address from a device that is on the same network.

You should be able to control the PC Power and and LED brightness from this page but the system stats would remain blank. 


### Macro Pad & System Stats

To see the system stats on the Web UI and make use of the macro pad follow the instructions on [this](https://github.com/LiveSparks/WinSerial) page.

The buttons on the keypad are F13 through F18 so there is never any possibility of conflict with a real keyboard. You can also modify the project to include 6 more keys so you have F13-F24 for 12 total keys. No modifications of the PC sided software are required for this.


## AutoHotKey

[AutoHotKey](https://www.autohotkey.com/) is a very powerful piece of automation software that allows you to define custom desktop wide hotkeys that can do complex tasks based on context.

### Some Examples

* Map one key to a another keybind. F13 -> Ctrl+C
    ```ahk
    F13::^c
    ```
* Open a web page
    ```ahk
    F17::Run chrome.exe "http://192.168.29.3/" " --new-window "
    ```
* Context Specific actions
    ```ahk
    #IfWinActive Untitled - Notepad
    !q::
    MsgBox, You pressed ALT+Q in Notepad.
    return

    ; Any window that isn't Untitled - Notepad
    #IfWinActive
    !q::
    MsgBox, You pressed ALT+Q in any window.
    return
    ```
* Combine two keys together
    ```ahk
    F13 & F17::Media_Play_Pause
    ```

> ⚠ You can only combine two keys together.

> ⚠ For some reason using keys of the same coloum together doesn't work. If you can figure out the reason, please let me know.

For more examples and in depth guides regarding AutoHotKey, visit their [documentations](https://www.autohotkey.com/docs/AutoHotkey.htm).

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License.



<!-- CONTACT -->
## Contact

Your Name - [@LiveSparksYT](https://twitter.com/LiveSparksYT) - livesparking@gmail.com

Project Link: [https://github.com/LiveSparks/ESP_Power_Button](https://github.com/LiveSparks/ESP_Power_Button)

<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements

* Logo icon made by [photo3idea-studio](https://www.flaticon.com/authors/photo3idea-studio)