Import("env")

board_config = env.BoardConfig()

board_config.update("build.hwids", [
    ["0x1209", "0001"],
    ["0x1209", "0001"]
])
board_config.update("build.usb_product", "JoyShift HGV")