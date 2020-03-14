#include <X11/Xlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

#include "vroot.h"
#include "FTree.h"

unsigned long int CreateColor(int red, int green, int blue) {
    return (red << 16) + (green << 8) + blue;
}

int main(int argc, char* argv[]) {
    // Seed random
    srand(time(0));
    
    unsigned int width = 800;
    unsigned int height = 800;
       
    // Open the display
    Display* pDisplay = XOpenDisplay(getenv("DISPLAY"));

    // Get a handle to the root window
    Window root = DefaultRootWindow(pDisplay);

    // Create graphics context
    GC gc = XCreateGC(pDisplay, root, 0, NULL);

    Window windowReturn;
    int x;
    int y;
    unsigned int border;
    unsigned int depth;
    XGetGeometry(pDisplay, root, &windowReturn, &x, &y, &width, 
                      &height, &border, &depth);

    width = width - border - x - 2;
    height = height - border - y - 2;

    while (true) {
       
        // Clear window
        XClearWindow(pDisplay, root);       
 
        // Swap buffers
        XFlush(pDisplay);

        // Pause
        usleep(100000); 
    }

    XCloseDisplay(pDisplay);
 
    return 0;
}
