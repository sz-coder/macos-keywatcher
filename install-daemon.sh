#!/bin/bash -euf

./uninstall-daemon.sh &> /dev/null

source ./vars.sh

printf "Compiling ... "
rm -f keywatcher
gcc -framework ApplicationServices -framework Carbon -Wall -Wpedantic main.c -o keywatcher
sudo mv keywatcher "$BIN_DEST_PATH"
printf "done\n"

sudo cp daemon.plist "$PLIST_DEST_PATH"

launchctl load -w "$PLIST_DEST_PATH"
launchctl start "$SERVICE_NAME"

printf "SUCCESSFULLY INSTALLED\n"
