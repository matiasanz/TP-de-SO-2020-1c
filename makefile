all: 
	-cd CrenitoCommons && $(MAKE) all
	-cd Broker && $(MAKE) all
	-cd GameBoy && $(MAKE) all
	-cd GameCard && $(MAKE) all
	-cd Team && $(MAKE) all
	-cd CrenitoCommons && $(MAKE) test

    #descomentar esto para poder debuggear localmente
	#-cd CrenitoCommons && $(MAKE) debug
	#-cd Broker && $(MAKE) debug
	#-cd GameBoy && $(MAKE) debug
	#-cd GameCard && $(MAKE) debug
	#-cd Team && $(MAKE) debug

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
	-cd CrenitoCommons && $(MAKE) test

uninstall:
	-cd CrenitoCommons && $(MAKE) install