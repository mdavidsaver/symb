#!/bin/sh
set -e -x

# Build base for use with https://travis-ci.org
#
# Set environment variables
# BASE= 3.14 3.15 or 3.16  (VCS branch)
# STATIC=  static or shared
# Optional
# RTEMS= 4.9 or 4.10

die() {
  echo "$1" >&2
  exit 1
}

[ "$BASE" ] || die "Set BASE"

CDIR="$HOME/.cache/base-$BASE-$STATIC"

if [ ! -e "$CDIR/built" ]
then
    install -d "$CDIR"
    ( cd "$CDIR" && git clone --depth 50 --branch $BASE https://github.com/epics-base/epics-base.git base )

    EPICS_BASE="$CDIR/base"
    # set RTEMS to eg. "4.9" or "4.10"
    # requires qemu, bison, flex, texinfo, install-info
    ( cd "$CDIR/base" && if [ -n "$RTEMS" ]
    then
        echo "Cross RTEMS${RTEMS} for pc386"
        install -d /home/travis/.cache
        curl -L "https://github.com/mdavidsaver/rsb/releases/download/travis-20160306-2/rtems${RTEMS}-i386-trusty-20190306-2.tar.gz" \
        | tar -C /home/travis/.cache -xj

        sed -i -e '/^RTEMS_VERSION/d' -e '/^RTEMS_BASE/d' configure/os/CONFIG_SITE.Common.RTEMS
        cat << EOF >> configure/os/CONFIG_SITE.Common.RTEMS
RTEMS_VERSION=$RTEMS
RTEMS_BASE=/home/travis/.cache/rtems${RTEMS}-i386
EOF
        cat << EOF >> configure/CONFIG_SITE
CROSS_COMPILER_TARGET_ARCHS+=RTEMS-pc386
EOF

        # find local qemu-system-i386
        export PATH="$HOME/.cache/qemu/usr/bin:$PATH"
        echo -n "Using QEMU: "
        type qemu-system-i386 || echo "Missing qemu"
        EXTRA=RTEMS_QEMU_FIXUPS=YES
    fi) # RTEMS

    case "$STATIC" in
    static)
        cat << EOF >> "$EPICS_BASE/configure/CONFIG_SITE"
SHARED_LIBRARIES=NO
STATIC_BUILD=YES
EOF
        ;;
    *) ;;
    esac

    make -C "$EPICS_BASE" -j2

    touch "$CDIR/built"
fi

EPICS_HOST_ARCH=`sh $EPICS_BASE/startup/EpicsHostArch`

echo "EPICS_BASE=$EPICS_BASE" > configure/RELEASE.local
