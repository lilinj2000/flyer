#! /bin/sh

home_libs=~/libs

home_flyer=${home_libs}/flyer

./configure --prefix=${home_flyer}

if test -d ${home_flyer}; then
    rm -rf ${home_flyer}
fi

make install

make distclean
