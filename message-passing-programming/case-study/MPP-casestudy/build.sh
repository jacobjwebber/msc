#!/bin/bash

gcc edge-serial.c pgmio.c -o edge-ser -lm && ./edge-ser f14.ascii.pgm && display output.pgm
