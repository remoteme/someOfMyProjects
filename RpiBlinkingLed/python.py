import logging
import socket

import struct
import sys
import os

os.chdir(sys.argv[1])
sys.path.append('../base')

import remoteme

import RPi.GPIO as GPIO
LED_PIN=19
BUTTON_PIN=26

relay2=False;

def setRELAY_01(b):
    remoteMe.getVariables().setBoolean("RELAY_01", b)

def setRELAY_02(b):
    remoteMe.getVariables().setBoolean("RELAY_02", b)

def changeButtonState(channel):
    global relay2
    relay2=not relay2
    setRELAY_02(relay2)
    pass

def onRELAY_01Change(b):
    GPIO.output(LED_PIN, GPIO.HIGH if b else GPIO.LOW)
    
    pass

def onRELAY_02Change(b):
    global relay2
    relay2=b
    pass

def setupPins():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(LED_PIN, GPIO.OUT) 
    GPIO.output(LED_PIN, GPIO.LOW)
    GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP) 
    GPIO.add_event_detect(BUTTON_PIN,GPIO.RISING,callback=changeButtonState,bouncetime=200) 
    pass
    
try:

    logging.basicConfig(level=logging.DEBUG,
    format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
    datefmt='%d.%m %H:%M',
    filename="logs.log")

    logger = logging.getLogger('application')
    setupPins()
    remoteMe = remoteme.RemoteMe()

    remoteMe.startRemoteMe(sys.argv)
    remoteMe.getVariables().observeBoolean("RELAY_01" ,onRELAY_01Change);
    remoteMe.getVariables().observeBoolean("RELAY_02" ,onRELAY_02Change);
    
    remoteMe.wait()

finally:
    pass