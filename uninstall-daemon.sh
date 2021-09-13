#!/bin/bash -euf

source vars.sh

if [ -f "$PLIST_DEST_PATH" ]; then
	printf "Found .plist file.\n"

	printf "Stopping service ..."
	launchctl stop "$SERVICE_NAME"
	printf "done\n"

	printf "Unloading from launchctl ... "
	launchctl unload "$PLIST_DEST_PATH"
	printf "done\n"

	sudo rm -f "$PLIST_DEST_PATH"
fi

if [ -f "$BIN_DEST_PATH" ]; then
	sudo rm -f "$BIN_DEST_PATH"
fi

printf "SUCCESSFULLY UNINSTALLED\n"
