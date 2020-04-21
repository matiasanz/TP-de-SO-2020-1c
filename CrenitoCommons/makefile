all: 
	-cd src && $(MAKE) all
#-cd src && $(MAKE) debug

clean:
	-cd src && $(MAKE) clean

d-install: uninstall debug install clean

debug:
	-cd src && $(MAKE) debug

install:
	-cd src && $(MAKE) install

uninstall:
	-cd src && $(MAKE) uninstall
	
test: all
	-cd tests && $(MAKE) test