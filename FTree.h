#ifndef FTree_h
#define FTree_h

#include <cmath>
#define PI 3.14159265359
#include <X11/Xlib.h>

struct Point {
    double x;
    double y;

    Point() {
        this->x = 0.0;
        this->y = 0.0;
    }

    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }

    Point(unsigned int x, unsigned int y) {
        this->x = static_cast<double>(x);
        this->y = static_cast<double>(y);
    }

    friend Point operator+(const Point& p1, const Point& p2) {
        return Point(p1.x + p2.x, p1.y + p2.y);
    }
    
    friend Point operator-(const Point& p1, const Point& p2) {
        return Point(p1.x - p2.x, p1.y - p2.y);
    }
    
    friend Point operator*(const Point& p, double m) {
        return Point(p.x * m, p.y * m);
    }
};

struct Branch {
    Point start;
    Point end;
    unsigned long int color;
    unsigned int width;
    Branch** pBranches;
};

struct Color {
    short red;
    short green;
    short blue;
 
    Color() {
        this->red = red;
        this->blue = blue;
        this->green = green;
    }
   
    Color(short red, short green, short blue) {
        this->red = red;
        this->blue = blue;
        this->green = green;
    }
    
    unsigned long int GetLong() {
        return (red << 16) + (green << 8) + blue;
    }
};

class FTree {
    /* Variables */
    private:
    Branch* pTrunk;
    unsigned int width;
    unsigned int height; 
    unsigned int numBranches = 2;
    double deltaAngle = 0.0;
    double deltaScale = 0.0;
    unsigned int numLevels = 0;
    Color startColor;
    Color endColor;
    int startThickness = 10;
    bool animationFinished = false;
    unsigned int animationLevel = 0;
    bool branchFinished = false;
    double stepDist = 0;
    double stepTotalDist = 0;

    /* Functions */
    public:
    FTree(unsigned int width, unsigned int height, double deltaAngle, double deltaScale) {
        this->width = width;
        this->height = height;
        this->deltaAngle = deltaAngle;  
        this->deltaScale = deltaScale;
        pTrunk = new Branch;
        pTrunk->start = Point(width / 2, height);
        pTrunk->end = Point(width / 2, height - 200);
    }

    Color MapColor(unsigned int levels) {
        Color color;
        if (numLevels == 0) {
            return endColor;
        }
        double perc = static_cast<double>(levels) / static_cast<double>(numLevels);
        color.red = static_cast<double>(endColor.red - startColor.red) * perc;
        color.red += startColor.red;
        color.green = static_cast<double>(endColor.green - startColor.green) * perc;
        color.green += startColor.green;
        color.blue = static_cast<double>(endColor.blue - startColor.blue) * perc;
        color.blue += startColor.blue;
        return color;
    }

    void SetStartColor(Color start) {
        startColor = start;
    }

    void SetEndColor(Color end) {
        endColor = end;
    }

    void Grow(unsigned int numLevels, double startAngle, double startScale) {
        this->numLevels = numLevels;
        pTrunk->color = MapColor(numLevels).GetLong();
        pTrunk->width = startThickness;
        BranchRec(pTrunk, numLevels, startAngle, startScale); 
    }

    void StartAnimation(double stepDist) {
        this->stepDist = stepDist;
        stepTotalDist = 0;
        animationLevel = 0;
        animationFinished = false;
    }

    void DrawAnimationStep(Display* pDisplay, Window* pWindow, GC* pGC) {
        stepTotalDist += stepDist;
        AnimateLevel(pTrunk, animationLevel, pDisplay, pWindow, pGC);
        if (branchFinished) {
            stepTotalDist = 0;
            animationLevel++;
            branchFinished = false;
        } 
        DrawLevels(pTrunk, animationLevel, pDisplay, pWindow, pGC);
    }

    bool AnimationFinished() {
        return animationFinished;
    }

    void AnimateLevel(Branch* pBranch, unsigned int level, Display* pDisplay,
                      Window* pWindow, GC* pGC) {
        if (level == 0) {
            Point vec = pBranch->end - pBranch->start;
            double dist = sqrt(pow(vec.x, 2.0) + pow(vec.y, 2.0));
            double desiredDist = stepTotalDist;
            if (stepTotalDist > dist) {
                branchFinished = true;
                desiredDist = dist;
                if (animationLevel == numLevels) {
                    animationFinished = true;
                }
            }
            vec = vec * (desiredDist / dist);
            
            XSetLineAttributes(pDisplay, *pGC, pBranch->width, LineSolid, CapButt, JoinBevel);
            XSetForeground(pDisplay, *pGC, pBranch->color); 
            XDrawLine(
                pDisplay, 
                *pWindow, 
                *pGC, 
                pBranch->start.x, 
                pBranch->start.y, 
                pBranch->start.x + vec.x, 
                pBranch->start.y + vec.y
            );    
        } else {
            level--;
            if (pBranch->pBranches != nullptr) {
                if (pBranch->pBranches[0] != nullptr) {
                    AnimateLevel(pBranch->pBranches[0], level, pDisplay, pWindow, pGC);
                }
                if (pBranch->pBranches[1] != nullptr) {
                    AnimateLevel(pBranch->pBranches[1], level, pDisplay, pWindow, pGC);
                }
            }
        }
    }

    void DrawLevels(Branch* pBranch, unsigned int levels, Display* pDisplay, 
                    Window* pWindow, GC* pGC) {
        if (levels == 0) {
           return; 
        }
        XSetLineAttributes(pDisplay, *pGC, pBranch->width, LineSolid, CapButt, JoinBevel);
        XSetForeground(pDisplay, *pGC, pBranch->color);
        XDrawLine(
            pDisplay, 
            *pWindow, 
            *pGC, 
            pBranch->start.x, 
            pBranch->start.y, 
            pBranch->end.x, 
            pBranch->end.y
        ); 
        levels--;
        if (pBranch->pBranches != nullptr) {
            if (pBranch->pBranches[0] != nullptr) {
                DrawLevels(pBranch->pBranches[0], levels, pDisplay, pWindow, pGC);
            }
            if (pBranch->pBranches[1] != nullptr) {
                DrawLevels(pBranch->pBranches[1], levels, pDisplay, pWindow, pGC);
            }
        }

        
    }

    void BranchRec(Branch* pBranch, unsigned int levels, double angle, double scale) {
        if (levels == 0) {
            return;
        }
        Branch* pLeft = new Branch;
        Branch* pRight = new Branch;
        pBranch->pBranches = new Branch*[numBranches]; 
        
        pLeft->start = pBranch->end;
        pRight->start = pBranch->end;
        Point vec = (pBranch->end - pBranch->start) * scale;
        Point vecLeft;
        vecLeft.x = vec.x * cos(angle) - vec.y * sin(angle);
        vecLeft.y = vec.x * sin(angle) + vec.y * cos(angle);
        Point vecRight;
        vecRight.x = vec.x * cos(-angle) - vec.y * sin(-angle);
        vecRight.y = vec.x * sin(-angle) + vec.y * cos(-angle);
        pLeft->end = pBranch->end + vecLeft;
        pRight->end = pBranch->end + vecRight;
        Color color = MapColor(levels); 
        pLeft->color = pRight->color = color.GetLong();
        pLeft->width = pRight->width = startThickness - (numLevels - levels) - 1;
        pBranch->pBranches[0] = pLeft;
        pBranch->pBranches[1] = pRight;
 
        levels--;
        angle += deltaAngle;
        scale += deltaScale;
        BranchRec(pLeft, levels, angle, scale);
        BranchRec(pRight, levels, angle, scale);
    }

    void Draw(Display* pDisplay, Window* pWindow, GC* pGC) {
       DrawRec(pTrunk, pDisplay, pWindow, pGC);    
    }

    void DrawRec(Branch* pBranch, Display* pDisplay, Window* pWindow, GC* pGC) {
        XSetLineAttributes(pDisplay, *pGC, pBranch->width, LineSolid, CapButt, JoinBevel);
        XSetForeground(pDisplay, *pGC, pBranch->color);
        XDrawLine(
            pDisplay, 
            *pWindow, 
            *pGC, 
            pBranch->start.x, 
            pBranch->start.y, 
            pBranch->end.x, 
            pBranch->end.y
        );
        if (pBranch->pBranches) {
            if (pBranch->pBranches[0] != nullptr) {
                DrawRec(pBranch->pBranches[0], pDisplay, pWindow, pGC);
            }
            if (pBranch->pBranches[1] != nullptr) {
                DrawRec(pBranch->pBranches[1], pDisplay, pWindow, pGC);
            }
        }
    }

    void Delete(Branch* pBranch) {
        if (pBranch->pBranches != nullptr) {
            if (pBranch->pBranches[0] != nullptr) {
                Delete(pBranch->pBranches[0]);
                delete pBranch->pBranches[0];
                pBranch->pBranches[0] = nullptr;
            }
            if (pBranch->pBranches[1] != nullptr) {
                Delete(pBranch->pBranches[1]);
                delete pBranch->pBranches[1];
                pBranch->pBranches[1] = nullptr; 
            }
            delete[] pBranch->pBranches;
            pBranch->pBranches = nullptr; 
        }
    }

    ~FTree() {
        Delete(pTrunk);
        if (pTrunk != nullptr) {
            delete pTrunk; 
        }
    }
};

#endif
