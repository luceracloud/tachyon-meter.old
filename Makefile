.PHONY: dist test deps version_header

all: tachyon-meter version

dist: all
	-rm -rf rel/pkg/tachyon-meter
	mkdir -p rel/pkg/tachyon-meter/{bin,lib,etc}
	mkdir -p rel/pkg/tachyon-meter/share/smf/methods
	cp tachyon.xml rel/pkg/tachyon-meter/share/smf/methods
	cp kstat/tachyon-meter{,.sh} rel/pkg/tachyon-meter/bin
	cp kstat/kstat.conf rel/pkg/tachyon-meter/etc/kstat.conf.example
	cp tachyon.conf rel/pkg/tachyon-meter/etc/tachyon.conf.example
	cp tachyon.version rel/pkg/tachyon-meter/etc/tachyon.version
	ldd rel/pkg/tachyon-meter/bin/tachyon-meter | grep 'libcrypto\|libcurl\|libiconv\|libidn\|libintl\|liblber\|libldap\|libsasl2\|libssh2\|libss' | awk '{print $$3}' | xargs -I{} cp {} rel/pkg/tachyon-meter/lib

tachyon-meter:
	make -C kstat

clean:
	make -C kstat clean
	make -C curl clean
	-rm -rf dist

gz: dist
	(cd rel/pkg; tar zcf ../tachyon-meter.tgz tachyon-meter)

package: gz
	make -C rel/pkg

version:
	echo "$(shell git symbolic-ref HEAD 2> /dev/null | cut -b 12-)-$(shell git log --pretty=format:'%h, %ad' -1)" > tachyon.version

# Dummy targets
test:
	true

deps:
	true

version_header:
	true
