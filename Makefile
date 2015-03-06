.PHONY: dist

all: tachyon-meter

dist: all
	-rm -rf dist
	mkdir -p dist/tachyon-meter/{bin,lib,etc}
	mkdir -p dist/tachyon-meter/share/smf/methods
	cp tachyon.xml dist/tachyon-meter/share/smf/methods
	cp kstat/tachyon-meter{,.sh} dist/tachyon-meter/bin
	cp kstat/kstat.conf dist/tachyon-meter/etc
	cp kstat/tachyon-config.sample dist/tachyon-meter/etc
	ldd dist/tachyon-meter/bin/tachyon-meter | grep 'libcrypto\|libcurl\|libiconv\|libidn\|libintl\|liblber\|libldap\|libsasl2\|libssh2\|libss' | awk '{print $$3}' | xargs -I{} cp {} dist/tachyon-meter/lib

tachyon-meter:
	make -C kstat

clean:
	make -C kstat clean
	make -C curl clean
	-rm -rf dist

gz: dist
	(cd dist; tar zcf ../tachyon-meter.tgz tachyon-meter)
