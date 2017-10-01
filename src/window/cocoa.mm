#import <opengl.h>
#import <Cocoa/Cocoa.h>
#import <window/cocoa.h>
#import <keyboard.h>

static const uint8_t kc_lookup_table[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x4f, 0x50, 0x51, 0x52, 0x00,
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
    this->_width = 800;
    this->_height = 600;
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

    frame = NSMakeRect(0, 0, 800, 600);
    win = [[NativeWindow alloc] initWithContentRect:frame
        styleMask:NSTitledWindowMask
        backing:NSBackingStoreBuffered
        defer:NO];
    [win setCollectionBehavior:[win collectionBehavior] | NSWindowCollectionBehaviorFullScreenPrimary];
    [win setTitle:[NSString stringWithCString:"Supremacy" encoding:NSASCIIStringEncoding]];
    [win setFrame:frame display:YES animate:YES];
    return win;
}

static NSOpenGLContext* create_cocoa_opengl(NativeWindow* win, int major, int minor)
{
    NSRect frame = NSMakeRect(0, 0, 800, 600);
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

    context = static_cast<NSOpenGLContext*>(_context);
    [context flushBuffer];
}

#import <log.h>

void WindowCocoa::poll(Keyboard& keyboard)
{
    NSEvent* event;
    uint32_t kc;
    bool down;

    while ((event = [NSApp nextEventMatchingMask:NSAnyEventMask
                     untilDate:[NSDate distantPast]
                     inMode:NSDefaultRunLoopMode
                     dequeue:YES]))
    {
        NSEventType type = [event type];
        if (type == kCGEventKeyDown || type == kCGEventKeyUp)
        {
            down = (type == kCGEventKeyDown);
            kc = [event keyCode];
            if (kc >= 128)
                continue;
            kc = kc_lookup_table[kc];
            keyboard.set_scancode(kc, down);
        }
    }
}
