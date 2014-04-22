netcat - Cycles Per Instruction
===============================

by Brandon Lucia, Andrew Olmstead, and David Balatero
Released April 2014


[http://netcat.co](http://netcat.co)
[http://netcat.bandcamp.com](http://netcat.bandcamp.com)


Cycles Per Instruction - The Kernel Module Edition
--------------------------------------------------

Welcome to the most unnecessarily complicated netcat album release format yet. 

In this repository, you will be able to compile your own kernel module, create a 
`/dev/netcat` device and pipe its output into an audio player.

This repository contains the album's track data in source files, that came from `.ogg` files that were 
encoded from `.wav` files that were created from `.mp3` files that were encoded from the mastered 
`.wav` files which were generated from ProTools final mix `.wav` files that were created from 
24-track analog tape.

Running it
----------
We've only tested this on Ubuntu Linux. Sure, it's real easy to grab another operating system ISO 
and test it out, but after a day toiling in the bitmines, who has the energy?

First, install some dependencies:

```
sudo apt-get install build-essential vorbis-tools
```

Check out the repo:

```
git clone git@github.com:usrbinnc/netcat-cpi-kernel-module.git
cd netcat-cpi-kernel-module
```

Next, build and run it:

```
make
sudo insmod netcat.ko
dmesg
```

You should see output like the following from `dmesg`:

```
[ 2606.528153] [netcat]: netcat - Cycles Per Instruction - Kernel Module Edition - 2014
[ 2606.528153] [netcat]: netcat is Brandon Lucia, Andrew Olmstead, and David Balatero
[ 2606.528153] [netcat]: Run 'sudo mknod /dev/netcat c 250 0' to setup the device.
[ 2606.528153] [netcat]: 'cat /dev/netcat | ogg123 -' to play.
```

Your `mknod` command will be slightly different, as the OS will assign you a different number every time. Run this command:

```
mknod /dev/netcat c <your number> 0
```

Finally, put on some headphones, and run:

```
cat /dev/netcat | ogg123 -
```

Track information will show up in the output of `dmesg`:

```
[  612.411529] [netcat]: Now playing track 2 - The Internet is an Apt Motherfucker
```

If you've read this far, god help us all.
