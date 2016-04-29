
#include "alert_message.h"
#import<Cocoa/Cocoa.h>

void alert_message() {
    @autoreleasepool {
            NSAlert *alert = [[NSAlert alloc] init];
            alert.messageText = @"Could not connect to webcam is it plugged in ?";
            alert.informativeText = @"Check and see if your webcam is connected.?";
            [alert runModal];
            [alert release];
    }
}

void ask_Filename(std::string &text) {
    @autoreleasepool {
        NSAlert *alert = [[NSAlert alloc] init];
        alert.messageText = @"Would you like to use a Webcam?";
        alert.informativeText = @"Ok for Webcam";
        [alert addButtonWithTitle:@"Use Video File"];
        [alert addButtonWithTitle:@"Use Webcam"];
        NSInteger return_value = [alert runModal];
        if(return_value == NSAlertFirstButtonReturn) {
            NSOpenPanel *panel = [NSOpenPanel openPanel];
            [panel setCanChooseFiles:YES];
            [panel setCanChooseDirectories:NO];
            [panel setAllowsMultipleSelection: NO];
            NSArray* videoTypes = [NSArray arrayWithObjects: @"mp4", @"mov", @"m4v", nil];
            [panel setAllowedFileTypes:videoTypes];
            if([panel runModal]) {
                NSURL *url = [panel URL];
                text = [[url path] UTF8String];
            }
            NSLog(@"Using Video File\n");
        } else if(return_value == NSAlertSecondButtonReturn) {
            NSLog(@"Using Webcam\n");
        }
        [alert release];
    }
}