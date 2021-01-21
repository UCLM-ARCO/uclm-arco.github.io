---
title: "Google Assistant on the Voice Bonnet"
date: 2021-01-20T10:13:30+01:00
draft: true
tags:
- Google 
- voice assistant

image: ""
description: "A recipe to learn how to create a Google Assistant with a Raspberry Pi 4 and Adafruit Voice Bonnet."
---

## Overview
You can set up your own Google Assistant with just a Raspberry Pi and an Adafruit Voice Bonnet. You through setting up the Google Assistant API you can install a few library, enable permissions and get the Google Assistant running on the RPi. Now you can ask Google what you want with the simple push of a button.

## Ingredients

In order to follow this recipe you will need:
* A Raspberry Pi 4 or **RPi4**
* A Adafruit Voice Bonnet 
* Two Mono Enclosed Speaker or headphones
* A Google acount

## Raspberry Pi Setup
The first step is perform an update/upgrade and install the cross-platform package manager `pip`:

{{<shell>}}
$ sudo apt-get update
$ sudo apt-get -y upgrade

$ sudo apt-get install -y python3-pip
$ sudo pip3 install --upgrade setuptools
{{</shell>}}

## Blinka Setup

Blinka is a CirciutPython library compatibility layer. It requires just a few commands to run:

{{<shell>}}
$ cd ~
$ sudo pip3 install --upgrade adafruit-python-shell
$ wget hhtps://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/raspi-blinka.py
$ sudo python3 raspi-blinka.py
{{</shell>}}

When it finishes, it will ask you if you would like to reboot. Choose **yes**

You install DotStart library to controlling the 3 on-board DotStar LEDs:

{{<shell>}}
$ pip3 install --upgrade adafruit-circuitpython-dotstar adafruit-circuitpython-bmp280
{{</shell>}}

### Check I2C and SPI

To verify that the script had enable I2C and SPI, run the following command: 

{{<shell>}}
$ ls /dev/i2c* /dev/spi*
{{</shell>}}

You sould see the next response: 

If you need another hardware SPI port, you can enable it by adding the line `dtoverlay=spi1-3cs` to the bottom of `boot/config.txt` and rebooting. 

## Audio Setup

### Install Voicecard software
You run `sudo i2cdetect -y 1` and you should see an entry under 1a. If you already installed software the number appear as UU, as show in the image. 

At the commands like run: 
{{<shell>}}
$ sudo apt-get install -y git
$ git clone https://github.com/HinTak/seeed-voicecard
$ cd seeed-voicecard
$ git checkout v5.5
$ sudo ./install.sh
{{</shell>}}

When it finishes reboot and on reboot run `sudo aplay -l` to list all sound cards. You should see it at the bottom:

You can user `alsamixer` to adjust the volume. Select the card with **F6**
 
### Headphone/Speaker Test
Make sure you ahce the audio on/off switch is set to on. Run `speaker-test -c2` and you will hear white noise. 

### Microphone Test
Run `sudo arecord -f cd -Dhw:2 | aplay -Dhw:2`. Then speak to hear yourself echoed. 

## Google Setup
### Project Creation

### Device Registration

### Google Assistant API Setup

### Enabling Permissions

## Device Setup 
Upload the **client_secret.json** credentials file and place it in your home directory. Next, you are going to check that you have the required packages and setup a virtual environment

{{<shell>}}
$ cd ~
$ sudo apt-get update
$ sudo apt-get install python3-dev python3-venv
$ python3 -m venv env
$ env/bin/python -m pip install --upgrade pip setuptools wheel
$ source env/bin/activate
{{</shell>}}

Install the Authorization library:
{{<shell>}}
$ sudo apt-get install portaudio19-dev libffi-dev libssl-dev
$ python -m pip install --upgrade google-assistant-sdk[samples]
$ python -m pip install --upgrade google-auth-oauthlib[tool]
{{</shell>}}

### Generating an OAuth Token
Run the following command: 

{{<shell>}}
$ google-oauthlib-tool --scope https://www.googleapis.com/auth/assistant-sdk-prototype --save --headless --client-secrets ~/client_secret.json
{{</shell>}}

The script should provide a URL to visit to generate the token. Go ahead and click on the **Advanced** link. After that, it will come up with a confirmation dialog, go ahead and click on **Allow**. Finally, you will be given an Authorization Code; click on the **Copy** icon and paste the authorization on your terminal. Code back into the script and the token will be generated and saved. 

## Usage
First make sure you're in the virtual environment. If you aren't run `source env/bin/activate`.

Next, run the following command

{{<shell>}}
$ wget https://raw.githubusercontent.com/adafruit/Adafruit_Learning_System_Guides/master/BrainCraft_Google_Assistant/gv_buttontotalk.py
{{</shell>}}

And run the script, type `python3 gv_buttontotalk.py`. The leds should turn green, meaning it's waiting for you to give it ask a question. 