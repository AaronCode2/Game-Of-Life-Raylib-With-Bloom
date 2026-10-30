#include "Utils.hpp"

int Utils::selectedtileTypeIndex = 0;
Color const Utils::testColor = {255, 0, 0, 100};
float Utils::deltaTimeIt = 0.0f;
bool Utils::hackerMode = false;
int Utils::shaderIndex = 3;

float Utils::bloomAdjust = 1.0f;

Font Utils::font;
int Utils::fontSize;

// Try to not use asserts that often

void Utils::exitApp(const std::string errorInfo) {

    CloseWindow();
    std::cerr << "\033[31m" << errorInfo << "\033[37m" << '\n';

    std::ofstream file("errors.log");

    file << errorInfo;

    file.close();

#if DEBUG_ENABLED

    __debugbreak;

    std::cout << "Press ENTER to exit the Application as a Error has occured [PLAYER]: ";
    std::cin.get();

    exit(EXIT_FAILURE);
#endif
} 

bool Utils::isSameXY(const Rectangle aObject, const Rectangle bObject) {

    return (
        aObject.x == bObject.x &&
        aObject.y == bObject.y
    );
}

bool Utils::isSameXY(const Vector2 aObject, const Vector2 bObject) {

    return (
        aObject.x == bObject.x &&
        aObject.y == bObject.y
    );
}

bool Utils::isSameXY(const Vector2 aObject, const Vector2 bObject, const Vector2 bObject2) {

    return (
        (
            aObject.x == bObject.x &&
            aObject.y == bObject.y
        ) 
        ||
        (
            aObject.x == bObject2.x &&
            aObject.y == bObject2.y
        )
    );
}

// These funcs are not used LOL, don't use them, I burrowed code thinking I will use these.

void Utils::collisionActionX(Rectangle &aObject, const Rectangle ahitBox, const Rectangle bHitBox, Vector2 &aVelocity, bool resetVelocity) {

    // Double-check Collision #BUG-FIX

    if(!CheckCollisionRecs(ahitBox, bHitBox)) 
        return;
            
#if DEBUG_ENABLED

    DrawText("COLLISON X!!!", 400, 400, 30, RED);

#endif
    if(aVelocity.x > 0) {
                
        const float offset = ahitBox.x - aObject.x + ahitBox.width + 0.01;

        aObject.x = bHitBox.x - offset;
    }
        
    if(aVelocity.x < 0) {
                
        const float offset = ahitBox.x - aObject.x - 0.01;

        aObject.x = bHitBox.x + bHitBox.width - offset;
    }

    if(resetVelocity) aVelocity.x = 0;
}

void Utils::collisionActionY(Rectangle &aObject, const Rectangle ahitBox, const Rectangle bHitBox, Vector2 &aVelocity, bool resetVelocity) {

    // Double-check Collision #BUG-FIX

    if(!CheckCollisionRecs(ahitBox, bHitBox)) 
        return;

#if DEBUG_ENABLED

    DrawText("COLLISON Y!!!", 400, 400, 30, RED);

#endif

    if(aVelocity.y > 0) {
                
        const float offset = ahitBox.y - aObject.y + ahitBox.height;

        aObject.y = bHitBox.y - offset;
    }
        
    if(aVelocity.y < 0) {
                
        const float offset = ahitBox.y - aObject.y;

        aObject.y = bHitBox.y + bHitBox.height - offset;
    }

    if(resetVelocity) aVelocity.y = 0;
}

Rectangle Utils::getScreenRect() {

    return {0, 0, (float) GetScreenWidth(), (float) GetScreenHeight()};
}

Vector2 Utils::getScreenSizeV() {

    return {(float) GetScreenWidth(), (float) GetScreenHeight()};
}

Rectangle Utils::THE_DEBUG_RECT = {0, 0, 0, 0};
Vector2 Utils::THE_DEBUG_VECTOR = {0, 0};

void Utils::debugRect(Rectangle object) {

    DrawRectangleRec(object, {255, 0, 0, 100});
}

bool Mouse::isHovering(const Rectangle object) {

    return CheckCollisionPointRec(GetMousePosition(), object);
}

bool Mouse::isClickedL(const Rectangle object) {

    return (CheckCollisionPointRec(GetMousePosition(), object) && IsMouseButtonDown(MOUSE_BUTTON_LEFT));
}

bool Mouse::isClickedR(const Rectangle object) {

    return (CheckCollisionPointRec(GetMousePosition(), object) && IsMouseButtonDown(MOUSE_BUTTON_RIGHT));
}

bool Mouse::isClickedOnceL(const Rectangle object) {


    return (CheckCollisionPointRec(GetMousePosition(), object) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}

bool Mouse::isClickedOnceR(const Rectangle object) {


    return (CheckCollisionPointRec(GetMousePosition(), object) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT));
}

std::string Utils::formatZeros(int num, int width) {

    // This func can fail when overflow happens, don't know if it works in release?

    std::string numText;
    int leadingZero;

    try {

        numText = std::to_string(num);
        leadingZero = width - numText.length();
    } catch(const std::exception& e) {

#if DEBUG_ENABLED

        std::cerr << e.what() << '\n';
#endif

        Utils::exitApp(
            "Overflow Money, The variable `NumText` value (Integar): " + numText + " To fix simply go to"
            " the Data/Money.txt and edit this to become a number below 4 digits"
        );
    }
    
    if(num >= 0)
        numText = 'x' + std::string(leadingZero, '0') + numText;
    else numText = "x" + numText;

    return numText;
}
