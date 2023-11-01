#!/bin/bash

if [ ! "$PalmPDK" ];then
PalmPDK=/opt/PalmPDK
fi

export BUILDDIR="Build_Host"
if [ -e "${BUILDDIR}" ]; then
	rm -rf "${BUILDDIR}"
fi
mkdir -p ${BUILDDIR}

CC="g++"

INCLUDEDIR="${PalmPDK}/include"
LIBDIR="${PalmPDK}/host/lib"

CPPFLAGS="-I${INCLUDEDIR} -I${INCLUDEDIR}/SDL"
LDFLAGS="-L${LIBDIR}"
LIBS="-lSDL -lGLESv2 -lpdl"
SRCDIR="../src/"

$CC $CPPFLAGS $LDFLAGS $LIBS -o ${BUILDDIR}/simple $SRCDIR/simple.cpp
