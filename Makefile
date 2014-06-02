.PHONY: dist

all: tachyon-meter version

dist: all
	-rm -rf dist
	mkdir -p dist/tachyon-meter/{bin,lib,etc}
	mkdir -p dist/tachyon-meter/share/smf/methods
	cp tachyon.xml dist/tachyon-meter/share/smf/methods
	cp kstat/tachyon-meter{,.sh} dist/tachyon-meter/bin
	cp kstat/kstat.conf dist/tachyon-meter/etc/kstat.conf.example
	cp tachyon.conf dist/tachyon-meter/etc/tachyon.conf.example
	cp tachyon.version dist/tachyon-meter/etc/tachyon.version
	ldd dist/tachyon-meter/bin/tachyon-meter | grep 'libcrypto\|libcurl\|libiconv\|libidn\|libintl\|liblber\|libldap\|libsasl2\|libssh2\|libss' | awk '{print $$3}' | xargs -I{} cp {} dist/tachyon-meter/lib

tachyon-meter:
	make -C kstat

clean:
	make -C kstat clean
	make -C curl clean
	-rm -rf dist

gz: dist
	(cd dist; tar zcf ../tachyon-meter.tgz tachyon-meter)

package: gz
	make -C pkg

version:
	echo "$(shell git symbolic-ref HEAD 2> /dev/null | cut -b 12-)-$(shell git log --pretty=format:'%h, %ad' -1)" > tachyon.version
