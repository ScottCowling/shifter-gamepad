# shifter-gamepad
 Convert a truck shifter (or other device) into a usb gamepad for games like Euro Truck Simulator 2.

# Why?
 Simulation games revolve around realism and experience. Euro Truck Simulator 2 and American Truck Simulator are even more fun with manual shifting, and having the real shifter switches for split (L/H) and range (1-3 > 4-6 | 1-4 > 5-8) for shifting makes the experience more immersive and fun.

# How?
 This project is using Arduino's HID library (and NicoHood's HID Project) alongisde an Arduino compatible microcontroller with Native USB support e.g. 32u4 or samd21 chip based controllers. For more details on controllers, see NicoHood's GitHub Repository and its amazing Wiki. https://github.com/NicoHood/HID

# Requirements
- A microcontroller with Native USB supported by the NicoHood HID Project and Arduino HID. See the link to NicoHood's HID Project Wiki for more information about compatible devices. I used and recommend an Adafruit Trinket M0 for use in a truck shifter project like this one. Its very small but plenty fast, making it the perfect choice for a clean finished product. (Yours will look better than mine).
- A shifter or controller to use with the controller. Searches for MAN TGX Shifter 6 pin will generally help you find cheap options like this: 
![Man TGX Shifter](/guide/example_shifter_tgx.png)
- Soldering iron, solder, and some patience and a willingness to fail and try again (especially for anyone new to soldering, like I am).

# Guide
TODO: check back soon!.