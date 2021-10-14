## PixelGrid 

![PixelGrid](https://user-images.githubusercontent.com/15841740/136886043-a6b512bb-102d-4967-9a35-6d7e460ffeda.jpg)

Pixelgrid is an arduino based game console that is extremely easy to program!  For doccumentation, make sure you visit the [WIKI](https://github.com/desertgreg/PixelGrid/wiki)

# IDE Install:
Install the Arduino IDE from this website https://www.arduino.cc/en/software. These instructions were written from the 1.8.16 version of the IDE. On windows, just leave most settings at their defaults but I disable the desktop icon:

![image](https://user-images.githubusercontent.com/15841740/136675209-1501f271-a506-40a6-b618-d8b1b8bcb54b.png)

Install to the default location (or wherever you want if you know what you're doing):

![image](https://user-images.githubusercontent.com/15841740/136675218-0ab381de-f25a-4cc3-a78a-628fea1adead.png)

The installer may ask you to install one or more drivers.  Is important to install these so your computer will be able to communicate with your arduino devices such as PixelGrid.  Select 'Install' for all of them.

![DriveInstall](https://user-images.githubusercontent.com/15841740/136675320-b98791a7-cadb-4994-85cb-e782f72ef18a.png)

# Board Package
The next step is to launch the arduino IDE and install a board package to allow it to program your PixelGrid.  First navigate to the board manager as shown below:

![image](https://user-images.githubusercontent.com/15841740/137393280-f8be4661-19ba-4803-bf9d-8927306a4662.png)

In the board manager, enter **Zero** in the search bar and then install the **Arduino SAMD Boards** package.  The process of installing the board package may ask you to install driers as well, select 'install' for all of these.  Once the Arduino SAMD Boards package is installed, you can close the board manager.

![BoardManager2](https://user-images.githubusercontent.com/15841740/136675772-e53d57a6-f5d5-4b3f-bbd1-50c9cc7343b5.png)

# Library Install

Soon I will publish the PixelGrid library into the official arduino system.  For now, just put the contents of this git repo in your Documents/Arduino/Libraries folder.  It should look like this:

![image](https://user-images.githubusercontent.com/15841740/137391627-fa150864-d88a-4e33-95bb-6107377daf4b.png)

Pixelgrid currently depends on one other library from Adafruit.  This will soon be automatic but for now install it as well by using the library manager:

![AdruinoLibraries1](https://user-images.githubusercontent.com/15841740/136675936-6338c2c6-0733-4280-8097-97f6c33c8cd2.png)

Enter **Adafruit SAMD21** into the search bar to narrow down the list of libraries.  Then you need to install the **Arduino Zero DMA Library**.  

![AdruinoLibraries4](https://user-images.githubusercontent.com/15841740/136676548-e609f8fa-fa98-4c17-b7ff-7289e6327784.png)

# Examples

The PixelGrid API is very simple.  Take a look at the example sketches under the libraries/PixelGrid/examples folder to get started!.  Make sure you select your board type as shown below:

![image](https://user-images.githubusercontent.com/15841740/137393024-654d152c-acab-4d2d-8575-d360f2f990c7.png)




