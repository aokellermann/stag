## Command line image steganographic injector and extractor
#### How to install:
Before installation, make sure all dependencies are installed:
* libpng
```bash
$ git clone https://github.com/aokellermann/stag.git
$ cd stag
$ make
$ sudo make install
```
If you are an Arch user, you can install using the provided PKGBUILD.
```bash
$ git clone https://github.com/aokellermann/stag.git
$ cd stag
$ makepkg
$ sudo pacman -U *.xz
```
#### Usage
This program currently only works for PNG and PPM images. In the future, more
formats will be supported.

To inject an image with another image, run
```bash
$ tick inject super_image sub_image output_image
```
where super_image is the image to have sub_image hidden inside it. The
final product will be output to output_image.

To extract an image from another previously injected image, run
```bash
$ tick extract super_image output_image
```
where super_image has previously had an image injected in it. The extracted
image will be output to output_image.

#### License
MIT License