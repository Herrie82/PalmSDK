BadFont - PDK sample app for use in palm-emulator
-------------------------------------------------

This PDK example is related to the following articles from the
HP Palm Developer Center blog:

http://developer.palm.com/blog/2010/04/a-walk-through-the-plug-in-development-kit-part-1/

http://developer.palm.com/blog/2010/04/a-walk-through-the-plug-in-development-kit-part-2/

http://developer.palm.com/blog/2010/09/a-walk-through-the-plug-in-development-kit-part-3/

The primary difference between this version of the example source
and the original BadFont-Part3.zip is that this version can be
compiled for the emulator (x86) as well as the device (ARM).

Note that in order to build PDK code for the emulator, you must have
the latest preview version of the HP webOS SDK/PDK installed which
includes x86 support.  This means there will be directories named
"emulator" in /opt/PalmPDK as well as "i686-gcc" (on the Mac).

Windows scripts of note:
------------------------

buildit.cmd - builds for all devices by default, invoke as "builtit 
emulator" to build emulator version

packageit.cmd - runs buildit, then packages files into IPK.  Use as 
"packageit emulator" to build IPK for emulator

runit.cmd - runs packagit, then installs and starts app. Use as
"runit emulator" to install into emulator.

To build, package, install, and then launch BadFont on the emulator,
type "runit emulator".  Of course, the Palm Emulator must already be running.

Mac OS X scripts of note:
-------------------------

runit.sh - builds, packages, installs, then launches app on emulator

buildit_for_device.sh   - Mac build script, defaults to ARM build
buildit_for_emulator.sh - Mac build script, generates emulator build

Thus, to build BadFont for the emulator, type "./buildit_for_emulator.sh".

To build, package, install, and then launch BadFont on the emulator,
type "./runit.sh".  Of course, the Palm Emulator must already be running.

