.PHONY: dist

all: tachyon-meter

dist: all
	-rm -rf dist
	mkdir -p dist/tachyon/{bin,lib,etc}
	cp kstat/tachyon-meter{,.sh} dist/tachyon/bin
	cp kstat/kstat.conf dist/tachyon/etc
	ldd dist/tachyon/bin/tachyon-meter | grep 'libcrypto\|libcurl\|libiconv\|libidn\|libintl\|liblber\|libldap\|libsasl2\|libssh2\|libss' | awk '{print $$3}' | xargs -I{} cp {} dist/tachyon/lib

tachyon-meter:
	make -C kstat

clean:
	make -C kstat clean
	make -C curl clean
