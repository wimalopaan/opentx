#!/usr/bin/python

# Taken from https://github.com/jw0k/ti83-tools
# Tool to generate two BMP files of 1-bit depth that when alternated 2 frames the dark one, and 1 frame the light one, emulate a 4 gray scale display

import sys
import argparse
import functools
from PIL import Image
parser = argparse.ArgumentParser(description="Convert indexed BMP into light and dark bmp frames")
parser.add_argument("path", help="path to the input BMP file")
parser.add_argument("-o", "--output", help="path to the output file")
args = parser.parse_args()

try:
    im = Image.open(args.path)
except e:
    print (e)
    sys.exit(2)

if (im.mode != 'P'):
    print ("not a palette image file")
    sys.exit(2)

if (im.size[0] != 128 or im.size[1] != 64):
    print ("bitmap size should be 128x64")
    sys.exit(2)

#00 - dark 0, light 0
#01 - dark 0, light 1
#10 - dark 1, light 0
#11 - dark 1, light 1

data = list(im.getdata())

darkData = [ 0x00 if int(bit/2) else 0xFF for bit in data ]
lightData = [ 0x00 if bit%2 else 0xFF for bit in data ]

darkLines = [ darkData[x:x+128] for x in range(0, len(darkData), 128) ]
lightLines = [ lightData[x:x+128] for x in range(0, len(lightData), 128) ]


dark_data = functools.reduce(lambda x,y: x+y, darkLines)
imd = Image.new('L', [128,64])
imd.putdata(dark_data)
imd.save('dark.bmp')


dark_data = functools.reduce(lambda x,y: x+y, lightLines)
iml = Image.new('L', [128,64])
iml.putdata(dark_data)
iml.save('light.bmp')

# img2lbm.py input.bmp output.lbm 128 1bit
