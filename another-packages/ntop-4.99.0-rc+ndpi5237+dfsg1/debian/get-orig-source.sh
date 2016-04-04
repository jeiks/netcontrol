#!/bin/sh

DEB_UPSTREAM_VERSION=$1
DEB_SOURCE_VERSION=$2
NDPI_REV=$3

UPSTREAM_DIR=$(echo -n $DEB_UPSTREAM_VERSION | sed -e 's/+rc[0-9]*//')

if [ ! -f ../ntop_$DEB_SOURCE_VERSION.orig.tar.gz ]; then
    uscan --noconf --force-download --rename --download-current-version --destdir=.
    rm -rf ntop-$DEB_UPSTREAM_VERSION
    rm -rf ntop-$UPSTREAM_DIR
    rm -rf ntop-$DEB_SOURCE_VERSION
    tar zxf ntop_$DEB_UPSTREAM_VERSION.orig.tar.gz
    rm ntop_$DEB_UPSTREAM_VERSION.orig.tar.gz
    # remove files not DFSG-compliant
    rm ntop-$UPSTREAM_DIR/ntop_darwin.*
    rm ntop-$UPSTREAM_DIR/ntop_win32.*
    rm ntop-$UPSTREAM_DIR/packages/Win32/ntop-VC6-project.zip

    # remove binary-onyl files from MaxMind
    rm ntop-$UPSTREAM_DIR/3rd_party/Geo*.dat.gz

    # Remove non-free Highcharts JS library.
    rm ntop-$UPSTREAM_DIR/html/exporting.js
    rm ntop-$UPSTREAM_DIR/html/highcharts.js

    # remove non-DFSG-compliant part of ntop.h
    ed ntop-$UPSTREAM_DIR/ntop.h > /dev/null <<EOF
/Declaration of POSIX directory browsing functions and types for Win32.
?/\* \** \*/?,/\/\* \** \*\//d
wq
EOF

    mv ntop-$UPSTREAM_DIR ntop-$DEB_SOURCE_VERSION

    GZIP=--best tar -cz --owner root --group root --mode a+rX \
        -f ntop_$DEB_SOURCE_VERSION.orig.tar.gz ntop-$DEB_SOURCE_VERSION

    mv ntop_$DEB_SOURCE_VERSION.orig.tar.gz ..

    rm -fr ntop-$DEB_SOURCE_VERSION
fi

if [ ! -f ../ntop_$DEB_SOURCE_VERSION.orig-nDPI.tar.gz ]; then
    rm -fr nDPI
    svn export -r $NDPI_REV https://svn.ntop.org/svn/ntop/trunk/nDPI/
    GZIP=--best tar -cz --owner root --group root --mode a+rX \
        -f ../ntop_$DEB_SOURCE_VERSION.orig-nDPI.tar.gz nDPI
fi
