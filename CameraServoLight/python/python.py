#from __future__ import division

import logging
import socket


import struct
import sys
import os
os.chdir(sys.argv[1])

sys.path.append('../base')

import remotemeMessages
import remoteme
import remotemeStruct
import remotemeUtils


import Adafruit_PCA9685
import RPi.GPIO as GPIO
LED_PIN=19



logger=None
remoteMe=None


pwm=None;



def onUserSyncMessage(senderDeviceId,data):
    logger.info("on user SYNC message got from {} of length {}".format(senderDeviceId,len(data)))




def onUserMessage(senderDeviceId,data):
    global pwm
    data=struct.unpack('>Bhh', data)
    if data[0]==1:
        pwm.set_pwm(0, 0,data[1] )
        pwm.set_pwm(1, 0,data[2] )
    elif data[0]==2:
        GPIO.output(LED_PIN, GPIO.HIGH if data[1] == 1 else GPIO.LOW)


def setupServo():
    global pwm
    pwm= Adafruit_PCA9685.PCA9685()
    pwm.set_pwm_freq(80)


def setupPins():
     GPIO.setmode(GPIO.BCM)
     GPIO.setup(LED_PIN, GPIO.OUT) 
     GPIO.output(LED_PIN, GPIO.LOW)


try:

    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%d.%m %H:%M',
                        filename="logs.log")

    logger = logging.getLogger('application')

    logger.info(">>> My Python application")

    setupServo()
    setupPins() #here
    
    remoteMe = remoteme.RemoteMe()
    remoteMe.startRemoteMe(sys.argv)


    remoteMe.setUserMessageListener(onUserMessage)
    remoteMe.setUserSyncMessageListener(onUserSyncMessage)


    remoteMe.wait()


finally:


    print("PYTHON finished")



