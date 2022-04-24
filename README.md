# shifter-gamepad
 Convert a HGV shifter knob into a usb gamepad for games like Euro Truck Simulator 2 and American Truck Simulator. Hoorah for manual shifting!


# Why?
 Simulation games revolve around realism and experience. Having a shifter knob with the switches for H-shifting trucks on your shifter controller (e.g. Thrustmaster TH8A) in American Truck Simulator and Euro Truck Simulator 2 like on real manual trucks makes the experience even more immersive. In simulation immersion and realism is king, so being able to use range, split and perhaps a transmission brake in an authentic way via the shifter knob tickles the realism and immersion senses.


# How?
 This project is using Arduino's HID library and NicoHood's HID Project alongisde an Arduino compatible microcontroller with Native USB support e.g. 32u4 or samd21 chip based controllers. This project was built with the Adafruit Trinket M0, a samd21 based controller, which is what I will focus on. For more details on controllers, see NicoHood's GitHub Repository and its excellent Wiki with lots of detail and examples on coding for this kind of project. https://github.com/NicoHood/HID


# Features
//TODO: List project features.


# Requirements
To use this project, you will need:

- A supported microcontroller with Arduino support, and native USB compatibility. 
	- See NicoHood's HID Project Wiki for more information on compatible microcontrollers. 
	- A good choice is the Adafruit Trinket M0 that I will be using. Its fast and extremely small for fitting inside the shifter if you would like me, prefer to hide it rather than have it in some kind of external dongle box.

- USB power/data cabling for the microcontroller.

- A shifter to use as a controller. Searches for MAN TGX Shifter 6 pin will generally help you find cheap options like this: 

![Man TGX Shifter](/guide/example_shifter_man_tgx.jpg)

- Soldering tools e.g. wire or paste, iron, wire cutters, and a multimeter as a nice to have for debugging, especially if you are new to soldering.

- A 3d printer or 3d printing service for creating a mount converter because truck shifter knobs are larger diameter than gamepad shifter mounts. This isn't the only way to achieve this though, a drill and a bolt to hold it tight is another successful method I have seen, I just really didn't want to drill into a rounded metal object. The converter I used is this one, printed by a 3d printing service: https://www.thingiverse.com/thing:4827444

- A willingness to fail and try again (especially for anyone new to soldering, like I am), and a bit of patience for the finicky bits like connecting wires.


# Guide
1. Preparing The Shifter
	- Remove the plastic cover from the shifter. It will usually just pop right off with 2 clips at the bottom holding it in.
   	- Remove the 6 pin connector off the end of the cable. You can do this using tweezers if you want to keep the plastic connector, or just break it off. Slide the plastic flex tube off the cables. This will expose the 6 individual wires connecting the button and 2 switches. Cut the wires shorter, leaving enough spare for maneuvering them around and stripping them ready solder them to your microcontroller. You can always cut them shorter if you left them too long or need to try again. Keep the spare wire for later projects if you wish to.

![Prepared Man TGX Shifter](/guide/prepared_shifter_man_tgx.jpg)

2. Soldering
	- Once you've checked your microcontroller will fit comfortably inside the spare space in the shifter knob and your cables are cut to a length you're happy with, you can get started soldering the switches to digital pins and ground on the controller.  The "momentary" switch if your shifter has one is likely to be 3 pin and have an inverted output as well as a regular output. For a gamepad, that inverted output can be ignored.
	- Start by stripping back and pre-soldering the ends of each cable when you've test fitted them with the microcontroller and found a comfortable length, and the pre-solder the pads of your controller. Dont strip back too much or too little, or you will risk a short, or worse, snapping the connector off the solder pad when pressing it into the shifter with the plastic cover. (I definitely didn't break a controller this way... hehe. Oops!)
   	- Take care to solder them on angles that wont make them flex or get stuck and risk breaking when you push the controller deeper into the shifter housing so you can close it.

![Soldered Man TGX Shifter](/guide/soldered_shifter_man_tgx.jpg)

3. Software
	- Simply clone this repo, or download and open it in PlatformIO. 
	- Use the PlatformIO tools to make sure you have the libraries required and they're up to date.
	- Tweak platformio.ini to your liking e.g. board/platform used.
	- Adjust the options in src/main.cpp e.g. pins, dotstar rgb, debugging.
	- build it, and upload it to your microcontroller.

![Tested Man TGX Shifter](/guide/tested_shifter_man_tgx.gif)

4. Mounting
   - Those who are confident with drilling curved materials can (if there isn't already one) drill and thread a hole into the metal at the bottom of their shifter knob so they can tension it onto the shifter stem with a screw.
   - I opted to use a 3d model someone made to have it 3d printed using Graphite-SLS, a strong 3d printing technology that produces robust parts. 
   - The model I used is here. If it helps you out, dont forget to consider supporting the creator! https://www.thingiverse.com/thing:4827444

![Adapter for Man TGX Shifter](/guide/adapter_shifter_man_tgx.jpg)
![Inserted Adapter for Man TGX Shifter](/guide/adapter_shifter_man_tgx_inserted.jpg)

5. Extra Steps
	- You can use rubber tape or zip ties to help hold the USB cable inside the shifter, so if the cable is pulled on it wont cause damage to the microcontroller or solder joints as easily.
	- You can modify platform_config.py to change the USB device vendor id, product id and product name if you wish. *see platform_config.py comments for some caveats/warnings.
	- You can use a short USB A Male to USB A Female cable prior to your main cable run down to your computer. This acts as a tension quick release, and makes it easier to unscrew your shifter if you want to easily swap back to your usual shifter knob.

6. All done
   - Enjoy!


# Suggestions?
 Do you have ideas on how to improve the project or have a sister project like this one based around custom simulator controls? Do let me know, I love hearing about what people are making, and think it'd be cool to create a list of links at the bottom here to other projects like this one.


# ROADMAP
- Inversion mapping for buttons, to invert their digitalRead state e.g. pin 1 HIGH = unpressed, LOW = pressed instead of LOW = unpressed, HIGH = pressed.
- ~~Button types (perhaps as part of a "button" class for ease of use). Push to toggle, toggle to push, maybe fancier stuff such as "blinking".~~ (This seems out of scope. I may implement this for a future project e.g. dashboard/buttonbox?)
- Remove dependency on nicohood's HID library and write a dedicated gamepad class & HID. (Purely for personal learning if not for any optimisation reasons).