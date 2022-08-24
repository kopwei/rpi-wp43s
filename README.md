# rpi-wp43s

The WP43s calculator using RPi

## Hardware preparation

We are using following component:

* RaspberryPi 4 (4GB)

* 5 inch DSI Display (IPS panel) (Optional)

* Waveshare 3.7inch E-Paper Hat

* SmartYao LY092 mini keyboard (48 keys with Cherry red switch)

## Install OpenCV4

We need to install OpenCV4 lib. Followed instruction below:

* [RPi OpenCV4 install](https://solarianprogrammer.com/2018/12/18/cross-compile-opencv-raspberry-pi-raspbian/)

* [OpenCV C++ examples on Raspberry Pi 4](https://qengineering.eu/opencv-c-examples-on-raspberry-pi.html)

* [install-opencv-4-on-raspberry-pi-4-and-raspbian-buster](https://www.pyimagesearch.com/2019/09/16/install-opencv-4-on-raspberry-pi-4-and-raspbian-buster/)

## Install necessary libraries

```bash
# Before this step, you need to install all necessary software, e.g. wiringPi and bcm2835 lib which are necessary for Waveshare Eink display library
apt-get install libxi-dev
```

## Clone & compile WP43s on RPi

```bash
mkdir -p ~/Developer/gitlab.com/Over_score
cd ~/Developer/gitlab.com/Over_score && git clone https://gitlab.com/Over_score/wp43s.git
cd wp43s && make
```

## Build & run

```bash
make clean && make
./output/bin/epd
```
