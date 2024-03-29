DESCRIPTION = "Utility scripts for internationalizing XML"
SECTION = "devel"
LICENSE = "GPLv2"

URLV="${@'.'.join(bb.data.getVar('PV',d,1).split('.')[0:2])}"
SRC_URI = "${GNOME_MIRROR}/intltool/${URLV}/intltool-${PV}.tar.bz2"
S = "${WORKDIR}/intltool-${PV}"

DEPENDS = "libxml-parser-perl-native"
DEPENDS_virtclass-native = "libxml-parser-perl-native"
#RDEPENDS_${PN} = "libxml-parser-perl"
RRECOMMENDS_${PN} = "perl-modules"
RRECOMMENDS_${PN}_virtclass-native = ""

inherit autotools pkgconfig perlnative

export PERL = "${bindir}/env perl"
PERL_virtclass-native = "/usr/bin/env nativeperl"

BBCLASSEXTEND = "native"
