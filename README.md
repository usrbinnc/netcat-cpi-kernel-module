♫ netcat - Cycles Per Instruction
===============================

by [Brandon Lucia](http://brandonlucia.com/music.html), Andrew Olmstead, and David Balatero

Released April 2014

* [http://netcat.co](http://netcat.co)
* [http://netcat.bandcamp.com](http://netcat.bandcamp.com)
* [http://tableandchairsmusic.com/artists/netcat](http://tableandchairsmusic.com/artists/netcat)


Cycles Per Instruction - The Kernel Module Edition
--------------------------------------------------

Welcome to the most unnecessarily complicated netcat album release format yet. 

In this repository, you will be able to compile your own kernel module, create a 
`/dev/netcat` device and redirect its output into an audio player (tested with mplayer and play from SoX as well).

```
ogg123 - < /dev/netcat
```

This repository contains the album's track data in source files, that (for complexity's sake) came from `.ogg` files that were 
encoded from `.wav` files that were created from `.mp3` files that were encoded from the mastered 
`.wav` files which were generated from ProTools final mix `.wav` files that were created from 
24-track analog tape.

If complexity isn't your thing, you can go to our [webpage](http://netcat.co) and follow the links to a digital download or a cassette tape of our album.

Building
--------
We've only tested this on Ubuntu Linux. Sure, it's real easy to grab another operating system ISO 
and test it out, but after a day toiling in the bitmines, who has the energy?

Other people report:

* @NicolasCARPi says it works on Arch 64-bit / kernel 3.14.1-1-ARCH. @diogoff shows you [how to do it](https://github.com/usrbinnc/netcat-cpi-kernel-module/wiki/Arch-Linux-how-to)
* Intrepid explorer @jfilip [feels good about his 64-bit Fedora 20 install](https://gist.github.com/jfilip/408ee178a4379bf06c45)
* @silviuvulcan Reports that we're up and running on slackware64-current
* @alinefr did make a [howto](https://github.com/usrbinnc/netcat-cpi-kernel-module/wiki/Gentoo-Linux-HOWTO) explaining how she is enjoying netcat in her Gentoo Linux. She also [explained how](https://github.com/usrbinnc/netcat-cpi-kernel-module/wiki/Simple-http-mp3-streaming) she did a http streaming from /dev/netcat to listen from [MPD](http://www.musicpd.org) and possibily from any open source audio player.
* @pah got it running after increasing the Vmalloc limits to `vmalloc=192M` on a 32-bit i686 Debian machine (3.13-1-686-pae).
* @g0hl1n reports that it works on Debian 7.4 "wheezy" / Linux 3.2.0-4-amd64 #1 SMP Debian 3.2.54-2 x86_64 GNU/Linux
* @ciderpunx can compile and run it on Debian 8.0 "Jessie" / Linux 3.12-1-amd64 #1 SMP Debian 3.12.9-1 (2014-02-01) x86_64 GNU/Linux

First, install some dependencies:

```
sudo apt-get install build-essential vorbis-tools linux-headers-$(uname -r)
```

Check out the repo:

```
git clone https://github.com/usrbinnc/netcat-cpi-kernel-module.git
cd netcat-cpi-kernel-module
```

Next, build the module by running:

```
make
```

Building will take a long time.  Hang with it.  Building also requires several gigabytes of memory.  We're not totally sure why, but we think it is because the compiler is making lots of copies of several large, static arrays that contain track data.

Listening
---------

After you build, you need to load the module and see if it is working, so you can run:

```
sudo insmod netcat.ko
dmesg
```

You should see output like the following from `dmesg`:

```
[ 2606.528153] [netcat]: netcat - Cycles Per Instruction - Kernel Module Edition - 2014
[ 2606.528153] [netcat]: netcat is Brandon Lucia, Andrew Olmstead, and David Balatero
[ 2606.528153] [netcat]: 'ogg123 - < /dev/netcat' to play.
```


Finally, put on some headphones, and run:

```
ogg123 - < /dev/netcat 
```

Track information will show up in the output of `dmesg`:

```
[  612.411529] [netcat]: Now playing track 2 - The Internet is an Apt Motherfucker
```

If you've read this far, god help us all.

Troubleshooting
---------------

### ERROR: could not insert module netcat.ko: Cannot allocate memory

Loading the module may fail due to kernel memory allocation limits:
```
insmod: ERROR: could not insert module netcat.ko: Cannot allocate memory
```
also indicated by the errors like the following in `dmesg`:
```
[195207.149213] vmap allocation for size 37806080 failed: use vmalloc=<size> to increase size.
[195207.149216] vmalloc: allocation failure: 37798136 bytes
[195207.149217] insmod: page allocation failure: order:0, mode:0xd2
```
This can happen especially on 32-bit kernels.  The Vmalloc limit can be
increased by passing `vmalloc=<size>` to your kernel command-line
(typically 128Mb on 32-bit kernels, increasing it to `vmalloc=192M`
should be sufficient).  Some information how to overcome this problem
can be found in the
[MythTV wiki](http://www.mythtv.org/wiki/Common_Problem:_vmalloc_too_small#The_Solution).


People insane enough to contribute (thanks!!)
---------

* @mark-bi added support for auto-creating our /dev/netcat device. Thanks, Mark!
* @arfar pointed out that not everyone wants to clone public repos over SSH, and updated the clone instructions to https. Thanks!
* @jmtd simplified the command to play audio by removing the extra instance of cat. Thanks!
* @gregkh for making our module coherent with standard Linux style and cleaning up some other loose ends.
* @silviuvulcan for testing with vlc and finding that we run and build on slackware64-current. Thanks!
* @pah dropped some knowledge for anyone struggling in the streets with 32-bit kernels and vmalloc limits
