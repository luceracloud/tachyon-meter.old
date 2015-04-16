VERSION=0.3.15p1
COMPONENT=tachyon-meter
SUFFIX:=$(shell echo $(SUFFIX))
BASENAME:=$(COMPONENT)-$(VERSION)$(SUFFIX)
.PHONY: package clean

package:
	cp install.sh $(BASENAME)
	tar -cf $(BASENAME).tar $(COMPONENT)
	uuencode $(BASENAME).tar $(BASENAME).tar >> $(BASENAME)
	-rm $(BASENAME).tar
	gzip -9 $(BASENAME)

clean:
	-rm *.gz
