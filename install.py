import os
import shutil

# This file installs the program in a directory with the assets

os.mkdir("minecraft")

if os.name == "nt":
    os.rename("build/minecraft.exe", "minecraft/minecraft.exe")
else:
    os.rename("build/minecraft", "minecraft/minecraft")

shutil.copytree("assets", "minecraft/assets")
