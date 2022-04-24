#Remove the block comment below to allow the USB PID-VID & Product code to run to change the identity of your device. 
#Note that the default values are pid.codes testing values and NOT recommended for production use. 
#See pid.codes or the USB-IF for guidance on attaining a product id so you can give your device a production name.
#You can also put any custom advanced build process code here where platformio.ini does not allow.

"""
Import("env")

board_config = env.BoardConfig()

board_config.update("build.hwids", [
    ["0x1209", "0001"],
    ["0x1209", "0001"]
])
board_config.update("build.usb_product", "JoyShift HGV")
"""