#!/usr/bin/env python3

from PIL import Image
from os import listdir, system
from pathlib import Path

def convert_pixel(pixel):
    int32_value = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16) + (pixel[3] << 24)
    return int32_value

def convert(file_name):
    print("Converting: " + file_name)
    include_file = open(Path(file_name).stem + ".h", "w")
    include_file.write("const uint32_t " + Path(file_name).stem + "_image[] = {")
    image_source = Image.open(file_name) # Can be many different formats.
    pix = image_source.load()
    image_size = image_source.size
    print("Width: " + str(image_size[0]) + " height: " + str(image_size[1]))
    print(image_source.size)  # Get the width and hight of the image for iterating over
    for x in range(image_size[0]):
        for y in range(image_size[1]):
            if (y % 8 == 0):
                include_file.write("\n")
            value = convert_pixel(pix[y,x])
            if (x == (image_size[0] -1)  and y == (image_size[1]- 1)):
                include_file.write(" 0x{:08x}".format(value))
            else:
                include_file.write(" 0x{:08x},".format(value))
    include_file.write("\n};\n")
    include_file.write("const uint32_t " + Path(file_name).stem + "_xsize = " +
                       str(image_size[0]) + ";\n")
    include_file.write("const uint32_t " + Path(file_name).stem + "_ysize = " +
                       str(image_size[1]) + ";\n")
if __name__ == "__main__":
    print("Converting files")
    filelist = listdir()
    for file in filelist:
        if file.endswith(".png"):
            convert(file)
