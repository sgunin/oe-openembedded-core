DESCRIPTION = "The X.Org X server"
HOMEPAGE = "http://www.x.org"
SECTION = "x11/base"
LICENSE = "MIT-X"

LIC_FILES_CHKSUM = "file://COPYING;md5=3dd2bbe3563837f80ed8926b06c1c353"

# xf86-*-* packages depend on an X server built with the xfree86 DDX
# so we have a virtual to represent that:
PROVIDES = "virtual/xserver-xf86"

# Other packages tend to just care that there is *an* X server:
PROVIDES += "virtual/xserver"

XORG_PN = "xorg-server"
SRC_URI = "${XORG_MIRROR}/individual/xserver/${XORG_PN}-${PV}.tar.bz2"

S = "${WORKDIR}/${XORG_PN}-${PV}"

inherit autotools pkgconfig

PACKAGES =+ "${PN}-utils"

RDEPENDS_${PN} += "xserver-xf86-config xkeyboard-config"

FILES_${PN}-utils = "${bindir}/scanpci ${bindir}/pcitweak ${bindir}/ioport ${bindir}/in[bwl] ${bindir}/out[bwl] ${bindir}/mmap[rw] ${bindir}/gtf ${bindir}/getconfig ${bindir}/getconfig.pl"
FILES_${PN} = "${libdir}/X11/Options ${libdir}/X11/Cards ${libdir}/X11/getconfig ${libdir}/X11/etc ${libdir}/xserver/SecurityPolicy ${libdir}/xorg/modules/*.so ${libdir}/xorg/modules/*/*.so ${libdir}/xorg/modules/*/*/*.so /etc/X11 "
FILES_${PN}-doc += "${libdir}/X11/doc ${datadir}/X11/xkb/compiled/README.compiled"

FILES_${PN}-dbg += "${libdir}/xorg/modules/.debug \
                    ${libdir}/xorg/modules/*/.debug \
		    ${libdir}/xorg/modules/*/*/.debug \
		    "

SRC_URI += "file://macro_tweak.patch"

EXTRA_OECONF = "--with-fop=no"
