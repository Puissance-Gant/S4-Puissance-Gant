# The Sym-Bot Project: A Robotic Arm by Hand Control 
## Sherbrooke University, Robotics Engineering S4 Project, 66th promotion, Winter 2022
[![License: GPL-3.0](https://img.shields.io/badge/License-GPLv3-blue.svg?style=flat-square)](/LICENSE)

<div align="center">
    <img src="./Application/img/Human's%20Touch.jpg" alt="Sym-Bot Human's Touch" width="600"/>
</div>

## Table of Content
- [The Sym-Bot Project: A Robotic Arm by Hand Control](#the-sym-bot-project-a-robotic-arm-by-hand-control)
  - [Sherbrooke University, Robotics Engineering S4 Project, 66th promotion, Winter 2022](#sherbrooke-university-robotics-engineering-s4-project-66th-promotion-winter-2022)
  - [Table of Content](#table-of-content)
  - [Authors](#authors)
  - [Project context](#project-context)
  - [Abilities](#abilities)
  - [Important note](#important-note)
  - [Getting started](#getting-started)
    - [Mechanical recommandations](#mechanical-recommandations)
    - [Electrical recommandations](#electrical-recommandations)
    - [Software recommandations](#software-recommandations)
  - [Special Thanks](#special-thanks)

## Authors
- Andrei Ioan Gabor - [TerrySpooker](https://github.com/TerrySpooker)
- Julien Thériault - [JuThe72](https://github.com/JuThe72)
- Yan-Éric Jérôme - [Arasmith](https://github.com/Arasmith)
- Frank Ciric Taicas - [Chromlox](https://github.com/Chromlox)
- Frédérick Boisvert - [boif1403](https://github.com/boif1403)
- Micah-Élie Labrecque - [MicahElie](https://github.com/MicahElie)

## Project context
This project came up from the desire to help people with disabilities to take or move objects out of reach. 
More precisely, some of these people don't have enough strength in their arms to strech them out. Fortunately, most of the times, they can at least move their hands and fingers.
That's the core of the Sym-Bot Project : Using a glove with sensors to control a robotic arm in the most intuitive way possible, by working in **symbiosis** with the user.

## Abilities
Our robotic arm is a simple 3 degrees of freedom robot. The goal is to accomplish simple tasks only by using the "Glove controller". Here are four things that Sym-Bot can do:
1. Push/Pull an object on the table, placed in front of our robot
2. Turn switch On/Off, the switch being on the table
3. Take a cellphone (200g max.) and keep it in the air
4. Using AI to predict moves based on hand signs that we created

## Important note
Make sure to read all the documentation linked if you want to recreate your own Sym-Bot. 
We'll do anything we can so that the documentation is as clear as possible. First step, read the [Getting started](#getting-started) just below.

## Getting started
The first step to follow for building your own Sym-Bot application is to follow the READMEs step-by-step guide in each sections.
Our project is divided in three main sections: 
* The first one is [Application (Main Software)](Application/), that's **where all the communication and data interpretation between the Glove and the Robotic Arm is done**. In that section, you will mainly find the Python code that will go on the Raspberry Pi 4B (or any other computer of your choice) that will be linked to the OpenCR of the Robotic arm and the Bluetooth of the Glove. That's also where the [AI section](Application/AI/) is.
* The second section is [Robotic Arm](/Robotic%20Arm/), that's where **you'll find the C++ code for the motors control, all the 3D parts with the assembly models (SolidWorks) and the electrical schematic.**
* Finally, the last, but not least, part, is the [Glove User Interface](/Glove%20UI/), in this section **you'll find the C++ code used with PlatformIO on the ESP32 and the list of electrical sensors and components that are attached on the glove with the electrical schematic**.

### Mechanical recommandations
* Most of the parts of our robot arm should be 3D-printed.
* We used different kind of bolts, shafts, inserts and couplers
* We created our own bearing with metalic marbles, but you can easily modify this section
* The list of components to buy, 3D parts and the assembly of the robotic arm is in the [Material & Assembly section](/Robotic%20Arm/Material%20%26%20Assembly/)

### Electrical recommandations
* Most of the electrical part is on the implementation of sensors on the glove
* We personally sew the electrical components on the glove.
* Make sure to use long enough wires for the connection between the motors and the OpenCR
* For more information about the electrical design, look at the [Robotic Arm Electrical schematic](/Robotic%20Arm/Material%20%26%20Assembly/ElectricalSchematic_RoboticArm.png) and the [Glove Electrical schematic](/Glove%20UI/img/Electrical%20Schematic%20Glove.png)

### Software recommandations
* We mostly used Python and C/C++ with Visual Studio Code as code editor and IDE with extensions.

## Special Thanks
To [Handling GRO](https://github.com/chameau5050/Handling-Gro#handling-gro) for their robotic arm that inspired us to make our own.
