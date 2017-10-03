#import <opengl.h>
#import <Cocoa/Cocoa.h>
#import <window/cocoa.h>
#import <keyboard.h>
#import <std/array.h>

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
    [(NSOpenGLContext*)_context release];
    [(NSWindow*)_window release];
}

static NSWindow* create_cocoa_window()
{
    NSWindow* win;
    NSRect frame;

    [NSApplication sharedApplication];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"ApplePersistenceIgnoreState"];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    [[NSApplication sharedApplication] finishLaunching];

    frame = [[NSScreen mainScreen] frame];
    win = [[NSWindow alloc] initWithContentRect:frame
        styleMask:NSWindowStyleMaskBorderless
        backing:NSBackingStoreBuffered
        defer:NO];
    [win setOpaque:YES];
    [win setCollectionBehavior:[win collectionBehavior] | NSWindowCollectionBehaviorFullScreenPrimary];
    [win setTitle:[NSString stringWithCString:"Supremacy" encoding:NSASCIIStringEncoding]];
    [win setFrame:frame display:YES animate:YES];
    [win toggleFullScreen:nil];
    return win;
}

static NSOpenGLContext* create_cocoa_opengl(NSWindow* win, int major, int minor)
{
    Array<NSOpenGLPixelFormatAttribute> gl_attr;
    NSRect frame;

    (void)minor;

    gl_attr.push_back(NSOpenGLPFANoRecovery);
    gl_attr.push_back(NSOpenGLPFAAccelerated);
    gl_attr.push_back(NSOpenGLPFAColorSize);
    gl_attr.push_back(32);
    gl_attr.push_back(NSOpenGLPFAAlphaSize);
    gl_attr.push_back(8);
    gl_attr.push_back(NSOpenGLPFADepthSize);
    gl_attr.push_back(24);
    gl_attr.push_back(NSOpenGLPFAStencilSize);
    gl_attr.push_back(8);
    gl_attr.push_back(NSOpenGLPFADoubleBuffer);
    gl_attr.push_back(NSOpenGLPFAMinimumPolicy);
    gl_attr.push_back(kCGLPFAOpenGLProfile);

    if (major >= 3)
        gl_attr.push_back(kCGLOGLPVersion_3_2_Core);
    else
        gl_attr.push_back(kCGLOGLPVersion_Legacy);
    gl_attr.push_back(0);

    frame = [[NSScreen mainScreen] frame];
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:gl_attr.data()];
    NSOpenGLView* view = [[NSOpenGLView alloc] initWithFrame:frame pixelFormat:pixelFormat];
    NSOpenGLContext* context = [[NSOpenGLContext alloc]
        initWithFormat:pixelFormat
        shareContext:nil];
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
    NSWindow*           win;
    NSOpenGLContext*    context;

    if (api != WindowRenderApi::OpenGL)
        return nullptr;
    win = create_cocoa_window();
    if (!win)
        return nullptr;
    context = create_cocoa_opengl(win, major, minor);
    if (!context)
    {
        [win release];
        return nullptr;
    }

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
        else
        {
            [NSApp sendEvent:event];
        }
    }
}
