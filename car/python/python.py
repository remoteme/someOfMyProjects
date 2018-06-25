import logging
import socket
import struct

import sys;
import os;

os.chdir(sys.argv[1])

sys.path.append('../base');

import remotemeMessages
import remoteme
import remotemeStruct
import remotemeUtils

import threading

from time import sleep

import Adafruit_PCA9685

import time
import RPi.GPIO as GPIO

logger = None
remoteMe = None

pwm = None;

motorAIn1 = 25  # GPIO25
motorAIn2 = 8  # GPIO8



motor = [motorAIn1, motorAIn2]

motorPWM = 14
turnPWM = 3

def onUserSyncMessage(senderDeviceId, data):
    logger.info("on user SYNC message got from {} of length {}".format(senderDeviceId, len(data)))


def setMotor( mode, speed):
    if mode == 1:
        motorSoftStop()
    elif mode == 2:
        motorForward()
    elif mode == 3:
        motorBackward()

    pwm.set_pwm(motorPWM, 0, speed)


def onUserMessage(senderDeviceId, data):
    global pwm
    data = struct.unpack('>Bhhh', data)




    if data[0] == 1:
        motorMode = data[1]
        motorSpeed = data[2] * 16;
        turn=data[3]

        setMotor( motorMode, motorSpeed)

        pwm.set_pwm(turnPWM, 0, turn)

        print(turn)
    elif data[0] == 2:
        pwm.set_pwm(1, 0, data[1])
        pwm.set_pwm(0, 0, data[2])

def setupPWM():
    global pwm
    pwm = Adafruit_PCA9685.PCA9685()
    pwm.set_pwm_freq(80)


def setupPins():
    global GPIO
    GPIO.setmode(GPIO.BCM)  # Broadcom pin-numbering scheme


    for pinId in motor:
        GPIO.setup(pinId, GPIO.OUT)


def motorForward():
    GPIO.output(motor[0], GPIO.LOW)
    GPIO.output(motor[1], GPIO.HIGH)


def motorBackward():
    GPIO.output(motor[0], GPIO.HIGH)
    GPIO.output(motor[1], GPIO.LOW)


def motorSoftStop():
    GPIO.output(motor[0], GPIO.LOW)
    GPIO.output(motor[1], GPIO.LOW)


try:

    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%d.%m %H:%M',
                        filename="./logs.log")

    logger = logging.getLogger('application')

    logger.info(">>> My Python application")

    setupPWM()
    setupPins()

    remoteMe = remoteme.RemoteMe()
    remoteMe.startRemoteMe(sys.argv)

    remoteMe.setUserMessageListener(onUserMessage)
    remoteMe.setUserSyncMessageListener(onUserSyncMessage)



    remoteMe.wait()


finally:

    print("PYTHON finished")



