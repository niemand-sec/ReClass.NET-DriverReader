all: debug release

clean: clean_debug clean_release

debug:
	cd Managed && make debug
	cd Native && make debug

clean_debug:
	cd Managed && make clean_debug
	cd Native && make clean_debug

release:
	cd Managed && make release
	cd Native && make release

clean_release:
	cd Managed && make clean_release
	cd Native && make clean_release
