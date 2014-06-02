VERSION=0.3.15p1
COMPONENT=tachyon-meter
SUFFIX:=$(shell echo $(SUFFIX))
BASENAME:=$(COMPONENT)-$(VERSION)$(SUFFIX)
.PHONY: package clean

package:
	cp install.sh $(BASENAME)
	cd ../dist && tar -cf ../pkg/$(BASENAME).tar $(COMPONENT)
	uuencode $(BASENAME).tar $(BASENAME).tar >> $(BASENAME)
	-rm $(BASENAME).gz $(BASENAME).tar
	gzip -9 $(BASENAME)

clean:
	-rm *.gz
