#import <opengl.h>
#import <Cocoa/Cocoa.h>
#import <window/cocoa.h>
#import <keyboard.h>

static const uint8_t kc_lookup_table[128] = {
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Enter,   Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Escape,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Escape,  Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Unknown,
    Keyboard::Unknown, Keyboard::Left,
    Keyboard::Right,   Keyboard::Down,
    Keyboard::Up,      Keyboard::Unknown,
};

static NSAutoreleasePool* ar_pool;

static NSOpenGLPixelFormatAttribute glAttributes[] = {
    NSOpenGLPFANoRecovery,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFAColorSize, 32,
    NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFADepthSize, 24,
    NSOpenGLPFAStencilSize, 8,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAMinimumPolicy,
    kCGLPFAOpenGLProfile,
    kCGLOGLPVersion_3_2_Core,
    0
};

@interface NativeWindow : NSWindow
@end

@implementation NativeWindow
@end

WindowCocoa::WindowCocoa(void* window, void* context)
: _window(window)
, _context(context)
{
    NSRect frame;

    frame = [[NSScreen mainScreen] frame];
    this->_width = frame.size.width;
    this->_height = frame.size.height;
}

WindowCocoa::~WindowCocoa()
{

}

static NativeWindow* create_cocoa_window()
{
    NativeWindow* win;
    NSRect frame;

    [NSApplication sharedApplication];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"ApplePersistenceIgnoreState"];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    [[NSApplication sharedApplication] finishLaunching];

    frame = [[NSScreen mainScreen] frame];
    win = [[NativeWindow alloc] initWithContentRect:frame
        styleMask:NSWindowStyleMaskBorderless
        backing:NSBackingStoreBuffered
        defer:NO];
    [win setLevel:NSMainMenuWindowLevel+1];
    [win setOpaque:YES];
    [win setCollectionBehavior:[win collectionBehavior] | NSWindowCollectionBehaviorFullScreenPrimary];
    [win setTitle:[NSString stringWithCString:"Supremacy" encoding:NSASCIIStringEncoding]];
    [win setFrame:frame display:YES animate:YES];
    [win toggleFullScreen:nil];
    return win;
}

static NSOpenGLContext* create_cocoa_opengl(NativeWindow* win, int major, int minor)
{
    NSRect frame;

    frame = [[NSScreen mainScreen] frame];
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc]
    initWithAttributes:glAttributes];
    NSOpenGLView* view = [[NSOpenGLView alloc] initWithFrame:frame pixelFormat:pixelFormat];
    NSOpenGLContext* context = [[[NSOpenGLContext alloc]
    initWithFormat:pixelFormat
    shareContext:nil] autorelease];
    [view setWantsBestResolutionOpenGLSurface:YES];
    [view setOpenGLContext:context];
    [win setContentView:view];
    [context makeCurrentContext];
    [context update];
    [win makeKeyAndOrderFront:nil];
    glewInit();
    return context;
}

WindowCocoa* WindowCocoa::create(WindowRenderApi api, int major, int minor)
{
    NativeWindow*       win;
    NSOpenGLContext*    context;

    win = create_cocoa_window();
    context = create_cocoa_opengl(win, major, minor);

    return new WindowCocoa(win, context);
}

void WindowCocoa::swap()
{
    NSOpenGLContext* context;
    GLint                       sync = 0;
    CGLContextObj               ctx = CGLGetCurrentContext();
    CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
    context = static_cast<NSOpenGLContext*>(_context);
    [context flushBuffer];
}

void WindowCocoa::poll(Input& input)
{
    InputEvent e;
    NSEvent* event;
    NSEvent* current_event;
    uint32_t kc;
    bool down;

    NSEventModifierFlags modifier_flags = 0;
    NSEventModifierFlags event_modifier_flags;
    current_event = [NSApp currentEvent];

    if (current_event)
        modifier_flags = [current_event modifierFlags];

    while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                     untilDate:[NSDate distantPast]
                     inMode:NSDefaultRunLoopMode
                     dequeue:YES]))
    {
        [NSApp sendEvent:event];
        NSEventType type = [event type];
        down = (type == NSEventTypeKeyDown);
        if (down || type == NSEventTypeKeyUp)
        {
            if ([event isARepeat])
                continue;
            kc = [event keyCode];
            if (kc >= 128)
                continue;
            kc = kc_lookup_table[kc];
            e.type = down ? InputEventType::KeyDown : InputEventType::KeyUp;
            e.key.scancode = kc;
            input.dispatch(e);
        }
        else if (type == NSEventTypeFlagsChanged)
        {
            event_modifier_flags = [event modifierFlags];
            if ((modifier_flags & NSEventModifierFlagShift) ^ (event_modifier_flags & NSEventModifierFlagShift))
            {
                down = (event_modifier_flags & NSEventModifierFlagShift);
                e.type = down ? InputEventType::KeyDown : InputEventType::KeyUp;
                e.key.scancode = Keyboard::Shift;
                input.dispatch(e);
            }
            modifier_flags = event_modifier_flags;
        }
    }
}
