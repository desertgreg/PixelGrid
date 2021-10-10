# PixelGrid Setup

## IDE Install
Install the Arduino IDE from this website https://www.arduino.cc/en/software.  These instructions were written from the 1.8.16 version of the IDE.
On windows, just leave most settings at their defaults but I disable the desktop icon:

![image](https://user-images.githubusercontent.com/15841740/136675209-1501f271-a506-40a6-b618-d8b1b8bcb54b.png)

Install to the default location (or wherever you want if you know what you're doing):

![image](https://user-images.githubusercontent.com/15841740/136675218-0ab381de-f25a-4cc3-a78a-628fea1adead.png)

The installer may ask you to install one or more drivers.  Is important to install these so your computer will be able to communicate with your arduino devices such as PixelGrid.  Select 'Install' for all of them.

![DriveInstall](https://user-images.githubusercontent.com/15841740/136675320-b98791a7-cadb-4994-85cb-e782f72ef18a.png)

## Board Package
The next step is to launch the arduino IDE and install a board package to allow it to program your PixelGrid.  First navigate to the board manager as shown below:

![BoardManager](https://user-images.githubusercontent.com/15841740/136675654-8e66323e-57d0-43a6-ade7-a653de5ffdc6.png)

In the board manager, enter **Zero** in the search bar and then install the **Arduino SAMD Boards** package.  The process of installing the board package may ask you to install driers as well, select 'install' for all of these.  Once the Arduino SAMD Boards package is installed, you can close the board manager.

![BoardManager2](https://user-images.githubusercontent.com/15841740/136675772-e53d57a6-f5d5-4b3f-bbd1-50c9cc7343b5.png)

## Library Install

There are a few libraries that need to be installed before you can start programming PixelGrid.  First open the library manager:

![AdruinoLibraries1](https://user-images.githubusercontent.com/15841740/136675936-6338c2c6-0733-4280-8097-97f6c33c8cd2.png)

Enter **Adafruit SAMD21** into the search bar to narrow down the list of libraries.  Then you need to install the **Arduino DMA neopixel library**.  

![AdruinoLibraries2](https://user-images.githubusercontent.com/15841740/136676194-c4ec67cf-bcc7-441c-b273-4fe623ecbb9f.png)

The library manager may ask you to install other library dependencies, just click **Install all**.

![AdruinoLibraries3](https://user-images.githubusercontent.com/15841740/136676364-e9ce570f-9718-4608-882e-17fe43b25e06.png)

Next install the **Adafruit Zero DMA Library** 

![AdruinoLibraries4](https://user-images.githubusercontent.com/15841740/136676548-e609f8fa-fa98-4c17-b7ff-7289e6327784.png)




**TODO**  Create New project, etc

Next change the Board selection to **Arduino Zero (Native USB Port)**.  This is normally the second one in the list of Arduino SAMD boards.  Notice there is are normally two options for Arduino Zero, for PixelGrid you need to select the **Native USB Port** option.

![BoardSelection](https://user-images.githubusercontent.com/15841740/136677144-b80bd2d1-bd52-47c1-8c84-7681c8fba59c.png)





