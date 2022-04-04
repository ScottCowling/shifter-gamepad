# shifter-gamepad
 Convert a truck shifter (or other device) into a usb gamepad for games like Euro Truck Simulator 2.

# Why?
 Simulation games revolve around realism and experience. Euro Truck Simulator 2 and American Truck Simulator are even more fun with manual shifting, and having the real shifter switches for split (L/H) and range (1-3 > 4-6 | 1-4 > 5-8) for shifting makes the experience more immersive and fun.

# How?
 This project is using Arduino's HID library (and NicoHood's HID Project) alongisde an Arduino compatible microcontroller with Native USB support e.g. 32u4 or samd21 chip based controllers. For more details on controllers, see NicoHood's GitHub Repository and its excellent Wiki with lots of detail and examples on coding for this kind of project. https://github.com/NicoHood/HID

# Requirements
- A microcontroller with Native USB supported by the NicoHood HID Project and Arduino HID. See the link to NicoHood's HID Project Wiki for more information about compatible devices. I used and recommend an Adafruit Trinket M0 for use in a truck shifter project like this one. Its very small but plenty fast, making it the perfect choice for a clean finished product. (Yours will look better than mine). If you use a larger controller, it will still work, but will have to be in a box outside the shifter, whereas I wanted mine to be concealed within the shifter itself.
- A long USB cable compatible with your microcontrollers port.
- A shifter to use as a controller. Searches for MAN TGX Shifter 6 pin will generally help you find cheap options like this: 
![Man TGX Shifter](/guide/example_shifter_man_tgx.jpg)
- Soldering iron, soldering wire, wire cutters, basic electronics tooling e.g. multimeter.
- Some patience and a willingness to fail and try again (especially for anyone new to soldering, like I am).
- A 3d printer or a 3d printing service for creating a mount converter, as many truck shifter knobs are much larger threading than gamepad shifters like the Logitech G Shifter or Thrustmaster TH8A.

# Guide
1. Preparing The Shifter
   - Remove the plastic cover by gripping or prying the plastic from the top of the shifter where the shifting legend is. It will unclip quite easily in most cases.
   - Remove the 6 pin connector off the end of the cable. Doing so without breaking it if you want to keep it for re-use on another project. Slide the plastic flex tube off the cabling if your shifter has one like the one in the example images. This will expose the 6 individual wires connecting the button and 2 switches. Cut the wires much shorter, leaving enough spare for maneuvering them to solder them to your microcontroller e.g. 10cm extra out of the bottom of the shifter if you would like room for retries if it goes wrong, then keep the spare wire for later projects if you wish to. You should end up with something like this.
![Prepared Man TGX Shifter](/guide/prepared_shifter_man_tgx.jpg)

2. Soldering
   - Once you've checked your microcontroller will fit comfortably inside the spare space in the shifter knob and your cables are cut to a length you're happy with, you can get started soldering the switches to digital pins and ground on the controller. Make sure you check your cables with a multimeter to check which cables control which switch if you're unsure to save extra difficulty later. The "momentary" switch, if your shifter has one, is likely to be 3 pin, and have an inverted output. Start by stripping back and pre-soldering the ends of each cable when you've test fitted them with the microcontroller and found a comfortable length, and the pre-solder the pads of your controller. Dont strip back too much or too little, or you will risk a short, or worse, snapping the connector off the solder pad when pressing it into the shifter with the plastic cover. (I definitely didn't break a controller this way... hehe. Oops!)
   - Solder the wires to the pads, taking care to fit them on angles that wont make them flex or get stuck and risk breaking when you push the controller deeper into the controller so you can close it.
![Soldered Man TGX Shifter](/guide/soldered_shifter_man_tgx.jpg)

3. Software
   - TODO: Work in progress. Check back soon!

4. Mounting
   - TODO: Work in progress. Check back soon!

5. All done
   - Enjoy!

# Extra
 Do you have ideas on how to improve the project or have a sister project like this one based around custom simulator controls. Do let me know, I love hearing about what people are making, and think it'd be cool to create a list of links at the bottom here to other projects like this one.


# ROADMAP
- Inversion mapping for buttons, to invert their digitalRead state e.g. pin 1 HIGH = unpressed, LOW = pressed instead of LOW = unpressed, HIGH = pressed.
- Button id mapping for mapping a pin number to a different "button" on the gamepad e.g. pin 4 presses button 1.
- Button types (perhaps as part of a "button" class for ease of use). Push to toggle, toggle to push, maybe fancier stuff such as "blinking".
- Calculate and execute delays for timekeeping via micros instead of millis for higher accuracy. (If there is a benefit to doing so?).