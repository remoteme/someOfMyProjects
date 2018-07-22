#from __future__ import division

import logging
import socket

import struct
import sys
import os


os.chdir(sys.argv[1])
sys.path.append('../base');

import remotemeMessages
import remoteme
import remotemeStruct
import remotemeUtils

import RPi.GPIO as GPIO
LED_PIN=19
BUTTON_PIN=26
WEB_DEVICEID=1001

light = False
logger=None
remoteMe=None


def onUserSyncMessage(senderDeviceId,data):
    global light
    logger.info("on user SYNC message got from {} of length {}".format(senderDeviceId,len(data)))

    dataTemp=[0];
    if light:
        dataTemp[0]=1
    return dataTemp

def changeButtonState(channel):
    global light,remoteMe
    dataTemp=[0]
    light = not light
    if light:
        dataTemp[0]=1
    GPIO.output(LED_PIN, GPIO.HIGH if light else GPIO.LOW)
    remoteMe.sendUserMessage(1001,dataTemp)
    
def onUserMessage(senderDeviceId,data):
   changeButtonState(1)
	
def setupPins():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(LED_PIN, GPIO.OUT) 
    GPIO.output(LED_PIN, GPIO.LOW)
    GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) 
    GPIO.add_event_detect(BUTTON_PIN,GPIO.RISING,callback=changeButtonState,bouncetime=200) 
try:

    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%d.%m %H:%M',
                        filename="logs.log")

    logger = logging.getLogger('application')

    setupPins()
  
    logger.info(">>> My Python application")

    remoteMe = remoteme.RemoteMe()

    remoteMe.setUserMessageListener(onUserMessage)
    remoteMe.setUserSyncMessageListener(onUserSyncMessage)

    remoteMe.startRemoteMe(sys.argv)

    remoteMe.wait()


finally:
    print("PYTHON finished")



