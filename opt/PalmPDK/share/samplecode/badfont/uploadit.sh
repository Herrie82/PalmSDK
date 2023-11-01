#!/bin/sh

novacom run -- file:///usr/bin/killall -9 gdbserver badfont
novacom put file:///media/internal/badfont <Build_Emulator/badfont
novacom put file:///media/internal/logo.png <logo.png
