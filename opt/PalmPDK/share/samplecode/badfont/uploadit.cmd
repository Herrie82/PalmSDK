@echo off

plink -P 10022 root@localhost -pw "" "killall -9 gdbserver badfont"
pscp -scp -P 10022 -pw "" badfont logo.png root@localhost:/media/internal
plink -P 10022 root@localhost -pw "" "chmod ugo+x /media/internal/badfont"

@echo .
@echo "badfont" is now installed in the emulator at /media/internal/badfont
@echo To launch it, type "cd /media/internal" and "./badfont" in a device shell
@echo .
