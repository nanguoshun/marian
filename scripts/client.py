#!/usr/bin/env python

from websocket import create_connection
import time
import sys

filePath = sys.argv[1]
print filePath

with open(filePath) as f:
  for line in f:
    ws = create_connection("ws://localhost:8080/translate")
    ws.send(line)
    result=ws.recv()
    print(result)
    ws.close()
    #time.sleep(5)

