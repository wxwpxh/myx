// $ g++-o test test.c -lX11 -lGL -lGLU -lglut

#include <cstdlib>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <cassert>

int main() {
    // connect to X Server
    auto displayName = getenv("DISPLAY");
    Display *display = XOpenDisplay(displayName);
    assert(display);

    // get default screen
    int defaultScreen = DefaultScreen(display);

    // get frame buffer configurations
    int att[] = {GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DOUBLEBUFFER, True, GLX_DEPTH_SIZE, 16, None};
    int nelements;
    auto fbConfig = glXChooseFBConfig(display, defaultScreen, att, &nelements);
    assert(fbConfig);

    // get visual
    auto visual = glXGetVisualFromFBConfig(display, *fbConfig);
    assert(visual);

    // get root window
    Window root = RootWindow(display, defaultScreen);
    auto colorMap = XCreateColormap(display, root, visual->visual, AllocNone);

    // create window
    XSetWindowAttributes swa;
    swa.colormap = colorMap;
    swa.event_mask = ExposureMask | KeyPressMask;
    unsigned long valueMask = CWColormap | CWEventMask;
    auto window = XCreateWindow(display, root, 100, 100, 320, 240, 0, visual->depth, InputOutput, visual->visual,
                                valueMask, &swa);
    XMapWindow(display, window);
    XStoreName(display, window, "show me");
    XFlush(display);
    // XSync(display, 0);

    // create gl context
    // auto glWindow = glXCreateWindow(display, *fbConfig, window, 0);
    auto glContext = glXCreateNewContext(display, *fbConfig, GLX_RGBA_TYPE, 0, GL_TRUE);
    // glXMakeContextCurrent(display, window, window, glContext);
    glXMakeCurrent(display, window, glContext);

    // init opengl status, or create resources
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    // message loop
    for (;;) {
        // get a event
        XEvent event;
        XNextEvent(display, &event);

        // loop break
        if (event.type == KeyPress) { break; }

        switch (event.type) {
            // paint event
            case Expose:
                // render
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // swap buffer
                glXSwapBuffers(display, window);

                break;
        }
    }

    // clear
    XFreeColormap(display, colorMap);
    if (glXGetCurrentDrawable() == window){
        glXMakeCurrent(display, None, 0);
    }
    XDestroyWindow(display, window);
    XFree(visual); XFree(fbConfig);

    if (glXGetCurrentContext() == glContext){
        glXMakeCurrent(display, None, 0);
    }
    glXDestroyContext(display, glContext);

    // disconnect to X Server
    XCloseDisplay(display);

    return 0;
}
	