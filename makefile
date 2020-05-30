all: 
	# descomentar la siguiente linea para poder debuggear localmente
	#-cd CrenitoCommons/src && $(MAKE) debug
	
	# comentar la siguiente linea para poder debuggear localmente
	-cd CrenitoCommons/src && $(MAKE) all
	-cd Broker && $(MAKE) all
	-cd GameBoy && $(MAKE) all
	-cd GameCard && $(MAKE) all
	-cd Team && $(MAKE) all
	-cd CrenitoCommons && $(MAKE) test  

clean:
	-cd CrenitoCommons && $(MAKE) clean
	-cd Broker && $(MAKE) clean
	-cd GameBoy && $(MAKE) clean
	-cd GameCard && $(MAKE) clean
	-cd Team && $(MAKE) clean

debug:
	-cd CrenitoCommons && $(MAKE) debug
	-cd Broker && $(MAKE) debug
	-cd GameBoy && $(MAKE) debug
	-cd GameCard && $(MAKE) debug
	-cd Team && $(MAKE) debug

install:
	-cd CrenitoCommons && $(MAKE) install
	-cd Broker && $(MAKE) all
	-cd GameBoy && $(MAKE) all
	-cd GameCard && $(MAKE) all
	-cd Team && $(MAKE) all
	-cd CrenitoCommons && $(MAKE) test

uninstall:
	-cd CrenitoCommons && $(MAKE) uninstall