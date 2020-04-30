all: 
	-cd src && $(MAKE) all
#	-cd tests && $(MAKE) all
	
#descomentar esto para poder debuggear localmente
#-cd src && $(MAKE) debug
#-cd tests && $(MAKE) debug

clean:
	-cd src && $(MAKE) clean
	-cd tests && $(MAKE) clean

debug:
	-cd src && $(MAKE) debug
	-cd tests && $(MAKE) debug

install:
	-cd src && $(MAKE) install

d-install: uninstall debug install clean

uninstall:
	-cd src && $(MAKE) uninstall
	
#compila y corre los tests
#test: all
#	-cd tests && $(MAKE) all
#	-./tests/unit-tests