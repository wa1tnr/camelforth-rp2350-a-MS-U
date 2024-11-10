# CamelForth in C
## by Dr. Brad Rodriguez

#### camelforth-rp2350-a-MS-U   UNSTABLE   0.2.0-pre-alpha   Sun Nov 10 20:09:35 UTC 2024

#### commit 04380a6a5de988ccbba09b00001c2ef085ad9265
#### Date:   Sun Nov 10 20:09:35 UTC 2024

#### for rp2350 - working version 10 Nov 20:09z

##### Please note: release tags prior to 0.2.0-pre-alpha are an artifact of the earlier work, and were ignored when making the 0.2.0-pre-alpha release.  See the (entirely intact) commit history, from February of 2021, forward, when the cloned repository began as an effort for the RP2040 (which was new at the time).

## CamelForth in C, by Dr. Brad Rodriguez

# RP2350 version

This version of CamelForth in C continues what was done for
the Raspberry Pi Pico RP2040 board (as well as one or two
other variants of board based on the RP2040 chip).

Now for RP2350.

A few minor USB CDC/ACM (tty) input/output issues resolved
for a smooth end-user experience.

Stores forth code in resident QSPI flashROM at 0x10040000.

` ok    hex 10040000 10 dump`

to see it, when resident.

The ` reading ` word expects keyboard input and was designed for
ascii-xfr (from minicom) upload of a plaintext forth source code
file (virtual typing into the forth interpreter).

Press ESC to terminate .. or flood the input buffer (TIB) with
space bar presses, to trip the counter of the maximum number of
bytes to accept.

`buf2flash` should be used next (immediately).  The program
assumes this was done with no intervening use of the forth
interpreter for side-queries.

Next cold boot, the newly uploaded forth source (now resident
in QSPI flashROM at 0x10040000) is read in (`'flaccept'` during
the boot) and thereby, the forth dictionary (held in SRAM)
 is expanded to include the new forth words.

In other words, the `'accept'` word (for normal interactive use of
the forth interpreter) was used as a model for reading forth source
code from the QSPI flashROM address space.  This became the (threaded)
`'flaccept'` word.


Backspace behaves correctly now - it backspaces, prints a space
character (to obliterate the existing character in the text
terminal interface) and then backspaces a second time (to
reposition the cursor).

The context makes sense; `5522` followed by two backspace keypresses,
followed by `11` produces `5511`.  Like that.

No provision tested for what to do if backspace is pressed such
that the left margin is 'exceeded' (flush left to expected location,
and one or more additional keypresses of the backspace key beyond it).

If it was handled prior (earlier iterations of this port/program)
it is not presently recollected. ;)

### big bug was
The big bug was:

`ch_read = getchar_timeout_us(18);`

.. producing a new (and unexpected) output of `0xfe` stored (possibly)
in `ch_read`.  This new behavior required a response; a suppression
strategy (or similar) was chosen.

Until this bug was found and fixed, the interpreter would not process
input (even though it was echo'd to the console fairly accurately).

#### the big bug fix (Nov 2024)

Literally: another `'OR'` clause was added to existing filtering
(of `0x00` and of `0xFF`).  Now it also filters `0xFE`.  It is
deemed likely that further testing would show that `0xFF` no longer
needs a filter there.  Nothing was examined to find where in the
pico-sdk this is determined. ;)

##### Future: filter all in one place

At the moment there are two likely places (in two different source files, likely).

Where `ch_read = getchar_timeout_us(18);` was employed.

Considering a common wrapper for any use case as the intent seems
at least superficially identical (to gain valid keyboard input).



## Yes this got too long.  tldr ftw.


#### Earlier text read:

UNSTABLE version - with mass storage support QSPI flashROM

NEOPIX support added (pio) 11 May.
Feather target configured for that. Edit source for others.

Stores forth source code in QSPI flash, and plays it
back during COLD (player piano).

# NEWS
## 4 November 2022 - inquiry received


> Begin forwarded message:

&gt; Date: Mon, 14 Nov 2022 15:03:24 +0000 

&gt; From: D.V. 

&gt; Subject: [CamelForth] Camelforth for Pico 

>&gt; Wondering whether the implementor will continue to develop the Pico implementation and/or 'entertain' questions about the implementation.

>&gt; I didn't see any contact information on this site or the 'github' material.  Thank you!

The one open [issue](https://github.com/wa1tnr/camelforth-rp2040-b-MS-U/issues/2) already invites contact through the issue mechanism.

Work is only done on this repository when a specific operational
difficulty arises, such as an update to pico-sdk that changes
how (or if) the program continues to compile correctly.

Not noticed ordinarily, as the project is essentially mothballed.

In its current state it should be able to read and write to
on-board (external, QSPI) flashROM 'raw'.  This amounts to a
player piano situation; it plays back the Forth source code
(in ascii representation, as with any ascii plaintext file)
.. upon a cold boot.

Which, in turn, amounts to an extended Forth dictionary.

## 14 April 2021 - three targets supported.

RPi Pico RP2040, Adafruit Feather and ItsyBitsy RP2040
(three targets) all supported.

Use a different shell script to build each.

see pico-examples/build for those scripts.

Added brief documentation (see immediately, below!)

# BRIEF DOCO

The program will stores your Forth source code, in
QSPI flash.

It will then play it back, during COLD (player piano).

There is a new 'flaccept' (flash accept) word/thread
that does the play-back during COLD; 'rewind' is used
to reset the pointer to the flashROM address where
flaccept (& friends) reads in the simulated keystrokes.

'rewind' word controls what happens the next time COLD
is called.  'rewind' allows the source to be read back
into the interpreter; failure to use 'rewind' leaves
the generic camelforth kernel, only, without the update
to the forth dictionary provided by this mechanism.

'reflash' simply starts the target such that the USB
mass storage is visible to the host PC.

'reading' accepts ascii upload of forth source code;
terminate with ESC (press it after the upload, the
interpreter will see your keypress of ESC and stop
the upload, and mark the buffer termination).

Max size supported is 4096 bytes (leave off a few
bytes for proper termination of the buffer).

Short uploads are not detected - press ESC as
detailed, above.

'reading' puts out its own address onto TOS.

'buf2flash' takes that address and uses it to find
what you've just ascii-uploaded (into SRAM).

It writes that block of SRAM into QSPI flashROM.

Nominally, at 0x10040000 per the SDK example program.

'erase' takes an address and erases the QSPI flashROM
at that address.  Should be a multiple of 0x1000 as
that's the size erased.

# OLDER

 new feature: no-error getKey()

     improved: rp2040_pico_getkey_usb.inc


Reasonably well debugged - functional Forth interpreter.

There was one bug where it crashes unexpectedly, after
a decently long runtime.  'getchar()' in the upstream
pico-sdk now has a bug report filed.

Port: rp2040, Raspberry Pi Pico target board, February, 2021.

older entries in this README.md, follows (from prior to 17 Feb Wed 16:29 UTC).


# PARENT was camelforth-rp2040-a    0.1.1-pre-alpha   Tue Feb 9 05:28:31 UTC 2021

commit 6cd03e97f38784f3f7fe8baf68416657af9469a7

    CamelForth in C by Brad Rodriguez

# About

CamelForth

A Forth in C by Dr Brad Rodriguez

Forth interpreter for the
RP2040 and Raspberry Pi Pico.

UNSTABLE version.  Look for changes and improvements here.

New feature: the blink word.  Use once per blink.

Requires pico-sdk and is modeled on pico-examples.

The /dev/ttyACM0 is the primary interface (USB).

(Reference host PC is Linux Debian amd64)

Using CP2104 USB-to-USART bridge, the secondary
interface (in Linux host PC) is /dev/ttyUSB0, but
that is arbitrary (you can bridge using other
methods and chips).

Connected to UART0 on the pico.

# Topics

# camelforth
# rp2040
# raspberry-pi-pico

# rpi-pico-pio

# forth
# c


## old branches:

BUILD ENVIRONMENT - bare bones text interpreter

no Forth functionality at all - not even base code.

Just keyboard echo loop.

END.
