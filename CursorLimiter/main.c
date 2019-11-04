//
//  main.c
//  CursorLimiter
//
//  Created by Chileung Law on 2019/11/4.
//  Copyright © 2019 Chileung Law. All rights reserved.
//

#include <ApplicationServices/ApplicationServices.h>

// Specify switch hot key here, see CGEventFlags.
const CGEventFlags SwitchKey = kCGEventFlagMaskCommand;

CGRect DisplayRect;
CGDirectDisplayID DisplayID = kCGNullDirectDisplay;

CGEventRef EventCallback(CGEventTapProxy proxy,
    CGEventType type,
    CGEventRef event,
    void* userInfo)
{
    CGPoint Point = CGEventGetLocation(event);

    if (((CGEventGetFlags(event) & SwitchKey) == SwitchKey) || DisplayID == kCGNullDirectDisplay) {
        CGDirectDisplayID CurrentDisplayID;
        CGGetDisplaysWithPoint(Point, 1, &CurrentDisplayID, NULL);
        if (CurrentDisplayID != DisplayID) {
            DisplayRect = CGDisplayBounds(CurrentDisplayID);
            printf(
                "Updated Display Rect: ID=%X, Origin=(%.f, %.f), Size=(%.f, %.f)\n",
                CurrentDisplayID,
                DisplayRect.origin.x,
                DisplayRect.origin.y,
                DisplayRect.size.width,
                DisplayRect.size.height);
            DisplayID = CurrentDisplayID;
        }
    } else {
        CGFloat MinX = DisplayRect.origin.x;
        CGFloat MaxX = (DisplayRect.origin.x + DisplayRect.size.width);
        CGFloat MinY = DisplayRect.origin.y;
        CGFloat MaxY = (DisplayRect.size.height + DisplayRect.origin.y);
        if (!((Point.x < MaxX && Point.x >= MinX) && (Point.y < MaxY && Point.y >= MinY))) {
            Point.y = (Point.y >= MaxY) ? (MaxY - 1) : MinY;
            Point.x = (Point.x >= MaxX) ? (MaxX - 1) : MinX;
            printf("Move Cursor: To=(%.f, %.f)\n", Point.x, Point.y);
            CGAssociateMouseAndMouseCursorPosition(false);
            CGWarpMouseCursorPosition(Point);
            CGAssociateMouseAndMouseCursorPosition(true);
        }
    }
    return event;
}

int main()
{
    CFMachPortRef EventTap = CGEventTapCreate(kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionListenOnly,
        CGEventMaskBit(kCGEventMouseMoved) | CGEventMaskBit(kCGEventLeftMouseDragged) | CGEventMaskBit(kCGEventRightMouseDragged),
        EventCallback,
        NULL);
    assert(EventTap != NULL);
    CFRunLoopAddSource(
        CFRunLoopGetCurrent(),
        CFMachPortCreateRunLoopSource(kCFAllocatorDefault, EventTap, 0),
        kCFRunLoopCommonModes);
    CGEventTapEnable(EventTap, true);
    CFRunLoopRun();
    return 0;
}
