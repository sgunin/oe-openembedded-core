LICENSE = "LGPLv2.1 | GPLv3"
LIC_FILES_CHKSUM = "file://LICENSE.LGPL;md5=fbc093901857fcd118f065f900982c24 \
                    file://LICENSE.GPL3;md5=babc5b6b77441da277f5c06b2e547720 \
                    file://LGPL_EXCEPTION.txt;md5=411080a56ff917a5a1aa08c98acae354"

FILESPATH =. "${FILE_DIRNAME}/qt-${PV}:"

SRC_URI = "http://get.qt.nokia.com/qt/source/qt-everywhere-opensource-src-${PV}.tar.gz \
           file://0004-no-qmake.patch \
           file://hack-out-pg2-4.7.0.patch \
           file://0006-freetype-host-includes.patch \
           file://0008-qt-lib-infix.patch \
           file://0009-support-2bpp.patch \
           file://0001-Added-Openembedded-crossarch-option.patch \
           file://blacklist-diginotar-certs.diff \
           file://fix-translations.patch \
           file://g++.conf \
           file://linux.conf \
           "

SRC_URI[md5sum] = "49b96eefb1224cc529af6fe5608654fe"
SRC_URI[sha256sum] = "d02b6fd69d089c01f4a787aa18175d074ccaecf8980a5956e328c2991905937e"

S = "${WORKDIR}/qt-everywhere-opensource-src-${PV}"

FILES_${QT_BASE_NAME}-tools                += "${bindir}/qml"

do_configure_prepend() {
    for pro in $(find ${S} -name "*.pro") ; do
        sed -i \
            -e 's:$$QT_BUILD_TREE/bin/lrelease:${OE_QMAKE_LRELEASE}:g' \
            -e 's:qtPrepareTool(LRELEASE, lrelease):LRELEASE = ${OE_QMAKE_LRELEASE}:g' $pro
    done

    sed -i s:SEDME:${S}: ${WORKDIR}/linux.conf
    sed -i \
    -e /QMAKE_MOC\ /d \
    -e /QMAKE_UIC\ /d \
    -e /QMAKE_UIC3\ /d \
    -e /QMAKE_RCC\ /d \
    ${S}/configure
}

QT_GLFLAGS ?= ""
QT_CONFIG_FLAGS += " -xmlpatterns -no-rpath -qt3support -reduce-relocations -silent ${QT_GLFLAGS}"

do_compile() {
    # Fixup missing wsegl header in some SGX SDKs
    if ! [ -e ${STAGING_INCDIR}/wsegl.h ] ; then
        cp src/3rdparty/powervr/wsegl.h src/plugins/gfxdrivers/powervr/QWSWSEGL/
    fi

    unset CFLAGS CXXFLAGS
    install -m 0755 ${STAGING_BINDIR_NATIVE}/rcc4 ${S}/bin/rcc
    install -m 0755 ${STAGING_BINDIR_NATIVE}/moc4 ${S}/bin/moc
    install -m 0755 ${STAGING_BINDIR_NATIVE}/uic4 ${S}/bin/uic
    install -m 0755 ${STAGING_BINDIR_NATIVE}/lrelease4 ${S}/bin/lrelease

    oe_runmake ${EXTRA_ENV}
}
