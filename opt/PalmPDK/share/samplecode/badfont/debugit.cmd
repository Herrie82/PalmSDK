@echo off

plink -P 10022 root@localhost -pw "" "killall -9 gdbserver badfont"
plink -P 10022 root@localhost -pw "" "/usr/bin/gdbserver host:2345 /media/internal/badfont &"
