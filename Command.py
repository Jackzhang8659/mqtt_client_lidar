import paho.mqtt.client as mqtt
import datetime
import json
import keyboard
import time


client = mqtt.Client()

client.connect("192.168.31.146", 1883,60)

count = 1

while 1:
    if keyboard.is_pressed('z'):
        x = {
        "sequence_number": count,
        "command": 0
        }
        count += 1
        y = json.dumps(x)
        client.publish("/Navigation/command0",y)
        time.sleep(5)
    elif keyboard.is_pressed('x'):
        x = {
        "sequence_number": count,
        "command": 1
        }
        count += 1
        y = json.dumps(x)
        client.publish("/Navigation/command1",y)
        time.sleep(5)