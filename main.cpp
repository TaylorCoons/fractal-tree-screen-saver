#include <X11/Xlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

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
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["maxAngle"] = {
        "-maxAngle",
        "-b",
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["minScale"] = {
        "-minScale",
        "-v",
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["maxScale"] = {
        "-maxScale",
        "-w",
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["minDeltaAngle"] = {
        "-minDeltaAngle",
        "-d",
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["maxDeltaAngle"] = {
        "-maxDeltaAngle",
        "-e",
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["minDeltaScale"] = {
        "-minDeltaScale",
        "-y",
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
        CLIParser::OPT_TYPE::OPTIONAL_OPT
    };
    options["maxDeltaScale"] = {
        "-maxDeltaScale",
        "-z",
        CLIParser::ARG_TYPE::OPTIONAL_ARG,
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
        fTree.StartAnimation(1);
        while (!fTree.AnimationFinished()) {       
            // Clear window
            XClearWindow(pDisplay, root);       

            fTree.DrawAnimationStep(pDisplay, &root, &gc);   

            // Swap buffers
            XFlush(pDisplay);

            // Pause
            usleep(10000); 
        }
        usleep(1000000);
        /*
        // Clear window
        XClearWindow(pDisplay, root);       
 
        FTree fTree(width, height, 0.001, 0.01);
        fTree.SetStartColor(rand() % 256, rand() % 256, rand() % 256);
        fTree.SetEndColor(rand() % 256, rand() % 256, rand() % 256);
        fTree.Grow(10, PI / 6.0, 0.8);
        fTree.Draw(pDisplay, &root, &gc);

        // Swap buffers
        XFlush(pDisplay);

        // Pause
        usleep(1000000); 
        */
    }

    XCloseDisplay(pDisplay);
 
    return 0;
}
