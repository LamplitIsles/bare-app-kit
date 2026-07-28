#import <assert.h>
#import <AppKit/AppKit.h>

#import "../lib/status-item.h"

int
main(void) {
  @autoreleasepool {
    [NSApplication sharedApplication];
    BareStatusItem *owner = [[BareStatusItem alloc] init];
    owner->status_item = [[[NSStatusBar systemStatusBar]
      statusItemWithLength:NSSquareStatusItemLength] retain];
    owner->menu = [[NSMenu alloc] init];
    owner->items = [[NSMutableDictionary alloc] init];
    owner->menu.autoenablesItems = NO;
    owner->status_item.menu = owner->menu;

    NSMenuItem *item = [[[NSMenuItem alloc] initWithTitle:@"Starting"
                                                   action:@selector(selected:)
                                            keyEquivalent:@""] autorelease];
    item.target = owner;
    item.representedObject = @"status";
    item.enabled = NO;
    [owner->menu addItem:item];
    [owner->items setObject:item forKey:@"status"];
    assert(!owner->menu.autoenablesItems);
    assert(!item.enabled);
    bare_app_kit_status_item_update_native(owner, @"status", @"Online", YES, YES, YES);
    assert([item.title isEqualToString:@"Online"]);
    assert(item.enabled);

    [owner prepareForBareTermination];
    assert(item.target == nil);
    [owner destroy];
    [owner destroy];
    assert(owner->removed_from_status_bar);
    assert(owner->status_item == nil);
    assert(owner->menu == nil);
    [owner release];
  }
  return 0;
}
