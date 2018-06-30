import logging
import socket
import struct
import time
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

motorAIn1 = 8  # GPIO25
motorAIn2 = 25  # GPIO8

motorBIn1 = 24  # 24
motorBIn2 = 23  # 23

motors = [[motorAIn1, motorAIn2], [motorBIn1, motorBIn2]]

motorsPWM = [14,15]
turnPWM = 3

def onUserSyncMessage(senderDeviceId, data):
    logger.info("on user SYNC message got from {} of length {}".format(senderDeviceId, len(data)))


def setMotor( mode, speed):
    motorSoftStop()

    time.sleep(0.02)

    if mode == 2:
        motorForward()
    elif mode == 3:
        motorBackward()

    pwm.set_pwm(motorsPWM[0], 0, speed)
    pwm.set_pwm(motorsPWM[1], 0, speed)

def onUserMessage(senderDeviceId, data):
    global pwm
    data = struct.unpack('>Bhhh', data)


    if data[0] == 1:
        motorMode = data[1]
        motorSpeed = data[2] * 16;
        turn=data[3]

        setMotor( motorMode, motorSpeed)

        pwm.set_pwm(turnPWM, 0, turn)


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


    for motor in motors:
        for pinId in motor:
            GPIO.setup(pinId, GPIO.OUT)


def motorForward():
    GPIO.output(motors[0][0], GPIO.LOW)
    GPIO.output(motors[0][1], GPIO.HIGH)
    GPIO.output(motors[1][0], GPIO.LOW)
    GPIO.output(motors[1][1], GPIO.HIGH)

def motorBackward():
    GPIO.output(motors[0][0], GPIO.HIGH)
    GPIO.output(motors[0][1], GPIO.LOW)
    GPIO.output(motors[1][0], GPIO.HIGH)
    GPIO.output(motors[1][1], GPIO.LOW)

def motorSoftStop():
    GPIO.output(motors[0][0], GPIO.LOW)
    GPIO.output(motors[0][1], GPIO.LOW)
    GPIO.output(motors[1][0], GPIO.LOW)
    GPIO.output(motors[1][1], GPIO.LOW)

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



