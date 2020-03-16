#include <X11/Xlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

#include "CLIParser/CLIParser.h"

#include "vroot.h"
#include "FTree.h"

unsigned long int CreateColor(int red, int green, int blue) {
    return (red << 16) + (green << 8) + blue;
}

CLIParser::OPTIONS InitOptions() {
    CLIParser::OPTIONS options;
    options["root"] = {
        "-root",
        "-r",
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["minAngle"] = {
        "-minAngle",
        "-a",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["maxAngle"] = {
        "-maxAngle",
        "-b",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["minScale"] = {
        "-minScale",
        "-v",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["maxScale"] = {
        "-maxScale",
        "-w",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["minDeltaAngle"] = {
        "-minDeltaAngle",
        "-d",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["maxDeltaAngle"] = {
        "-maxDeltaAngle",
        "-e",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["minDeltaScale"] = {
        "-minDeltaScale",
        "-y",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["maxDeltaScale"] = {
        "-maxDeltaScale",
        "-z",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["animationSpeed"] = {
        "-speed",
        "-s",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["pauseTime"] = {
        "-pause",
        "-p",
        CLIParser::ARG_TYPE::REQUIRED_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
        
    return options;
}

void Swap(double& val1, double& val2) {
    double tmp = val1;
    val1 = val2;
    val2 = tmp;
}

double RandDouble() {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

int main(int argc, char* argv[]) {
    CLIParser argparse;
    CLIParser::OPTIONS options = InitOptions();
    argparse.AddParser("", &options);
    argparse.Parse(argc, argv);
    
    double minAngle = 35.0;
    if (options["minAngle"].flag) {
        try {
            minAngle = std::stod(options["minAngle"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "minAngle must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "minAngle out of range" << std::endl;
        }
    }
    double maxAngle = 45.0;
    if (options["maxAngle"].flag) {
        try {
            maxAngle = std::stod(options["maxAngle"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "maxAngle must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "maxAngle out of range" << std::endl;
        }
    }
    double minScale = 0.7;
    if (options["minScale"].flag) {
        try {
            minScale = std::stod(options["minScale"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "minScale must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "minScale out of range" << std::endl;
        }
    }
    double maxScale = 0.9;
    if (options["maxScale"].flag) {
        try {
            maxScale = std::stod(options["maxScale"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "maxScale must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "maxScale out of range" << std::endl;
        }
    }
    double minDeltaAngle = -7.0;
    if (options["minDeltaAngle"].flag) {
        try {
            minDeltaAngle = std::stod(options["minDeltaAngle"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "minDeltaAngle must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "minDeltaAngle out of range" << std::endl;
        }
    }
    double maxDeltaAngle = 5.0;
    if (options["maxDeltaAngle"].flag) {
        try {
            maxDeltaAngle = std::stod(options["maxDeltaAngle"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "maxDeltaAngle must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "maxDeltaAngle out of range" << std::endl;
        }
    }
    double minDeltaScale = -0.01;
    if (options["minDeltaScale"].flag) {
        try {
            minDeltaScale = std::stod(options["minDeltaScale"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "minDeltaScale must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "minDeltaScale out of range" << std::endl;
        }
    }
    double maxDeltaScale = 0.01;
    if (options["maxDeltaScale"].flag) {
        try {
            minAngle = std::stod(options["maxDeltaScale"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "maxDeltaScale must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "maxDeltaScale out of range" << std::endl;
        }
    }
    double speed = 1.0;
    if (options["animationSpeed"].flag) {
        try {
            speed = std::stod(options["animationSpeed"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "speed must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "speed out of range" << std::endl;
        }
    }
    double pauseTime = 10.0;
    if (options["pauseTime"].flag) {
        try {
            pauseTime = std::stod(options["pauseTime"].result);
        } catch (std::invalid_argument const& e) {
            std::cerr << "pause time must be a double" << std::endl;
        } catch (std::out_of_range const& e) {
            std::cerr << "pause time out of range" << std::endl;
        }
    }


    if (minAngle > maxAngle) {
        Swap(minAngle, maxAngle);
    }

    if (minScale > maxScale) {
        Swap(minScale, maxScale);
    }

    if (minDeltaAngle > maxDeltaAngle) {
        Swap(minDeltaAngle, maxDeltaAngle);
    }

    if (minDeltaScale > maxDeltaScale) {
        Swap(minDeltaScale, maxDeltaScale);
    }

    double fps = 144.0;
    double framePeriod = 1.0 / fps;

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

    XMapWindow(pDisplay, root);

    Window windowReturn;
    int x;
    int y;
    unsigned int border;
    unsigned int depth;
    XGetGeometry(pDisplay, root, &windowReturn, &x, &y, &width, 
                      &height, &border, &depth);

    width = width - border - x;
    height = height - border - y;

    Pixmap redBuffer = XCreatePixmap(pDisplay, root, width, height, depth);
    Pixmap blueBuffer = XCreatePixmap(pDisplay, root, width, height, depth); 
    Pixmap* frontBuffer = &redBuffer;
    Pixmap* backBuffer = &blueBuffer; 
    while (true) {
        Color start(rand() % 256, rand() % 256, rand() % 256);
        Color end(rand() % 256, rand() % 256, rand() % 256);      
        double angle = RandDouble() * (maxAngle - minAngle) + minAngle;
        angle = angle * PI / 180.0;
        double scale = RandDouble() * (maxScale - minScale) + minScale;
        double deltaAngle = RandDouble() * (maxDeltaAngle - minDeltaAngle) + minDeltaAngle;
        deltaAngle = deltaAngle * PI / 180.0;
        double deltaScale = RandDouble() * (maxDeltaScale - minDeltaScale) + minDeltaScale; 
        FTree fTree(width, height, deltaAngle, deltaScale);
        fTree.SetStartColor(start);
        fTree.SetEndColor(end);
        fTree.Grow(9, angle, scale);
        fTree.StartAnimation(speed);
        clock_t prevTime = clock();
        while (!fTree.AnimationFinished()) {           
            clock_t currTime = clock(); 
            double timeElapsed = static_cast<double>(currTime - prevTime) / CLOCKS_PER_SEC; 

            if (timeElapsed > framePeriod) {
                prevTime = currTime;


                // Push front buffer to screen
                XCopyArea(pDisplay, *frontBuffer, root, gc, 0, 0, width, height, 0, 0);

                // Clear buffer
                XSetForeground(pDisplay, gc, 0x000000);
                XFillRectangle(pDisplay, *backBuffer, gc, x, y, width, height);

                // Draw animation
                fTree.DrawAnimationStep(pDisplay, backBuffer, &gc);   
                
                // Present 
                XSync(pDisplay, False);
               
                // Swap buffers 
                Pixmap* tmpBuffer = frontBuffer;
                frontBuffer = backBuffer;
                backBuffer = tmpBuffer;
            }
        }
        prevTime = clock();
        while (static_cast<double>(clock() - prevTime) / CLOCKS_PER_SEC < pauseTime) {
            (void)0;
        }
    }

    XCloseDisplay(pDisplay);
 
    return 0;
}
