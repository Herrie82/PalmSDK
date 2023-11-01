@echo off
call packageit.cmd %1 %2

@rem install the package
call palm-install com.palmdts.app.badfont_0.4.0_all.ipk

@rem and finally, launch it
call palm-launch com.palmdts.app.badfont

