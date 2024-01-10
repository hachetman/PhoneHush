#!/usr/bin/env python3
# reset uc on given serial port by toggling rate to 1200
# -*- coding: utf-8 -*-

import sys
import time
import serial

com = serial.Serial(sys.argv[1], 1200, dsrdtr=True)
com.dtr = False
com.rts = False
