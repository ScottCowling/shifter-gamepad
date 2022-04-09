# shifter-gamepad
 This project is for converting a truck shifter into a usb gamepad for games like Euro Truck Simulator 2 and American Truck Simulator.

# Why?
 Simulation games revolve around realism and experience. Having a shifter knob with the switches for H-shifting trucks on your shifter controller (e.g. Thrustmaster TH8A) in American Truck Simulator and Euro Truck Simulator 2 like on real manual trucks makes the experience even more immersive. In simulation, immersion and realism is king, so being able to use range, split, and perhaps a transmission brake in an authentic way via the shifter knob tickles the realism and immersion senses.

# How?
 This project is using Arduino's HID library and NicoHood's HID Project alongisde an Arduino compatible microcontroller with Native USB support e.g. 32u4 or samd21 chip based controllers. This project was built with the Adafruit Trinket M0, a samd21 based controller, which is what I will focus on. For more details on controllers, see NicoHood's GitHub Repository and its excellent Wiki with lots of detail and examples on coding for this kind of project. https://github.com/NicoHood/HID

# Requirements
To use this project, you will need:

- A supported microcontroller with Arduino support, and native USB compatibility. See NicoHood's HID Project Wiki for more information on compatible microcontrollers. A good choice is the Adafruit Trinket M0 that I will be using. Its fast, and extremely small for fitting inside the shifter, but you can always put a larger controller in a box outside the shifter if necessary, but nothing beats a clean look and convenient single cable approach.

- A long USB cable compatible with your microcontrollers port.

- A shifter to use as a controller. Searches for MAN TGX Shifter 6 pin will generally help you find cheap options like this: 
![Man TGX Shifter](/guide/example_shifter_man_tgx.jpg)

- Soldering tools e.g. wire or paste, iron, wire cutters, and a multimeter as a nice to have for debugging, especially if you are new to soldering.
- Some patience and a willingness to fail and try again (especially for anyone new to soldering, like I am).

- A 3d printer or 3d printing service for creating a mount converter because truck shifter knobs are larger diameter than gamepad shifter mounts. This isn't the only way to achieve this though, a drill and a bolt to hold it tight is another successful method I have seen, I just really didn't want to drill into a rounded metal object. The converter I used is this one, printed by a 3d printing service: https://www.thingiverse.com/thing:4827444

# Guide
1. Preparing The Shifter
   - Remove the plastic cover from the shifter. It will unclip quite easily in most cases.
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
- Remove dependency on nicohood's HID library and write a dedicated gamepad class & HID. (Purely for personal learning if not for any optimisation reasons).
- Set DotStar colour based on the report, not on the raw digitalread value so constant switches dont block