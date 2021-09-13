#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include <stdlib.h>
#include <unistd.h>

// from: https://gist.github.com/atr000/387590
#define RSHIFT_DOWN 0x20104ull
#define MODIFIER_UP 0x100ull

// The following callback method is invoked on every keypress.
CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
	CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
	CGEventFlags modifiers = CGEventGetFlags(event);

	(void)keyCode;

	if (modifiers == RSHIFT_DOWN) {
		system("curl http://10.10.10.10:4444/toggle-usb-switch --max-time 3 -o /dev/null &> /dev/null &");
	}

	return event;
}

int main(int argc, const char *argv[]) {
	fprintf(stderr, "I am: %d : %d\n", getuid(), geteuid());

	if (argc > 1) {
		fprintf(stderr, "Version: 1.0.0\n");
		exit(0);
	}

	// Create an event tap to retrieve keypresses.
	CGEventMask eventMask = /*CGEventMaskBit(kCGEventKeyDown) |*/ CGEventMaskBit(kCGEventFlagsChanged);
	CFMachPortRef eventTap = CGEventTapCreate(
		kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, NULL
	);

	// Exit the program if unable to create the event tap.
	if (!eventTap) {
		fprintf(stderr, "ERROR: Unable to create event tap.\n");
		exit(1);
	}

	// Create a run loop source and add enable the event tap.
	CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
	CGEventTapEnable(eventTap, true);

	CFRunLoopRun();

	return 0;
}
