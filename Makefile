all: build

build: clean
	mkdir -p target
	gcc \
		-Wall \
		-Wextra \
		-Werror \
		-framework CoreFoundation \
		-framework IOBluetooth \
		-framework IOKit \
		-o target/btlid src/main.c

install:
	install -m 755 target/btlid /usr/local/bin/btlid
	install -m 644 com.github.martyla.btlid.plist /Library/LaunchDaemons/com.github.martyla.btlid.plist
	launchctl unload -w com.github.martyla.btlid.plist 2>/dev/null
	launchctl load -w /Library/LaunchDaemons/com.github.martyla.btlid.plist
	launchctl start com.github.martyla.btlid.plist

uninstall:
	launchctl unload -w /Library/LaunchDaemons/com.github.martyla.btlid.plist 2>/dev/null
	rm -f /usr/local/bin/btlid
	rm -f /Library/LaunchDaemons/com.github.martyla.btlid.plist

clean:
	rm -rf target

.PHONY: all build install uninstall clean
