require apt.inc
inherit native

DEPENDS += "dpkg-native gettext-native"
PACKAGES = ""
USE_NLS = "yes"

SRC_URI += "file://db_linking_hack.patch"

python do_install () {
	bb.build.exec_func('do_install_base', d)
	bb.build.exec_func('do_install_config', d)
}

python do_install_config () {
	indir = os.path.dirname(bb.data.getVar('FILE',d,1))
	infile = file(os.path.join(indir, 'files', 'apt.conf'), 'r')
	data = infile.read()
	infile.close()

	data = bb.data.expand(data, d)

	outdir = os.path.join(bb.data.getVar('D', d, 1), bb.data.getVar('sysconfdir', d, 1), 'apt')
	if not os.path.exists(outdir):
		os.makedirs(outdir)
	outpath = os.path.join(outdir, 'apt.conf.sample')

	outfile = file(outpath, 'w')
	outfile.write(data)
	outfile.close()
}

do_install_base () {
	install -d ${D}${bindir}
	install -m 0755 bin/apt-cdrom ${D}${bindir}/
	install -m 0755 bin/apt-get ${D}${bindir}/
	install -m 0755 bin/apt-config ${D}${bindir}/
	install -m 0755 bin/apt-cache ${D}${bindir}/
	install -m 0755 bin/apt-sortpkgs ${D}${bindir}/
	install -m 0755 bin/apt-extracttemplates ${D}${bindir}/

	eval `cat environment.mak | grep ^GLIBC_VER | sed -e's, = ,=,'`
	eval `cat environment.mak | grep ^LIBSTDCPP_VER | sed -e's, = ,=,'`
	oe_libinstall -so -C bin libapt-pkg$GLIBC_VER$LIBSTDCPP_VER ${D}${libdir}/
	ln -sf libapt-pkg$GLIBC_VER$LIBSTDCPP_VER.so ${D}${libdir}/libapt-pkg.so
	oe_libinstall -so -C bin libapt-inst$GLIBC_VER$LIBSTDCPP_VER ${D}${libdir}/
	ln -sf libapt-inst$GLIBC_VER$LIBSTDCPP_VER.so ${D}${libdir}/libapt-inst.so

	install -d ${D}${libdir}/apt/methods
	install -m 0755 bin/methods/* ${D}${libdir}/apt/methods/

	install -d ${D}${libdir}/dpkg/methods/apt
	install -m 0644 dselect/desc.apt ${D}${libdir}/dpkg/methods/apt/ 
	install -m 0644 dselect/names ${D}${libdir}/dpkg/methods/apt/ 
	install -m 0755 dselect/install ${D}${libdir}/dpkg/methods/apt/ 
	install -m 0755 dselect/setup ${D}${libdir}/dpkg/methods/apt/ 
	install -m 0755 dselect/update ${D}${libdir}/dpkg/methods/apt/ 

	install -d ${D}${sysconfdir}/apt
	install -d ${D}${localstatedir}/lib/apt/lists/partial
	install -d ${D}${localstatedir}/cache/apt/archives/partial

	install -d ${D}${localstatedir}/log/apt/
}
