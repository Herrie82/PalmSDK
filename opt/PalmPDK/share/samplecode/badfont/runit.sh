#!/bin/sh

# call packageit.sh to (build and) package the app
./packageit.sh

# kill any currently-running instance
novacom run -- file:///usr/bin/killall -9 gdbserver badfont

# install the package
palm-install com.palmdts.app.badfont_0.4.0_all.ipk

# and finally, launch it
palm-launch com.palmdts.app.badfont

