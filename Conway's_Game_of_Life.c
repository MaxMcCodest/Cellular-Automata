#include <raylib.h>
#include <stdio.h>

struct AutomataTypes{
    Color c;
    int number;
    int minNeighbors, maxNeighbors;
    int interactablePixel[10];
    int whatDirections[8];
};

typedef struct {
    int pixelType;
}Automata;

struct Camera{
    Vector2 position;
    float speed, zoomSpeed, zoom;
};

#define WIDTH 1280
#define HIGHT 720

struct AutomataTypes types[2];

Automata pixels[HIGHT][WIDTH];
int pixelsToShow[HIGHT][WIDTH];

int main(){
    
    srand(time(NULL));
    
    /* Define Pixels */
    types[0] = (struct AutomataTypes){.number = 0, .minNeighbors = 3, .maxNeighbors = 3, .interactablePixel = {1,11,11,11,11,11,11,11,11,11}, .c = {0,0,0, 255}, .whatDirections = {1,1,1,1,1,1,1,1}};
    types[1] = (struct AutomataTypes){.number = 1, .minNeighbors = 2, .maxNeighbors = 3, .interactablePixel = {1,11,11,11,11,11,11,11,11,11}, .c = {255,255, 0, 255}, .whatDirections = {1,1,1,1,1,1,1,1}};
    
    /* Init Camera */
    struct Camera cam = (struct Camera){.position = (Vector2){0,0}, .speed = 4, .zoomSpeed = 0.1, .zoom = 1};
    
    /* Initialize Pixel Values */
    for(int y = HIGHT; y > 0; y--){
        for(int x = WIDTH; x > 0; x--){
            pixels[y - 1][x - 1] = (Automata){.pixelType = rand() % WIDTH};
        }
    }
    
    /* Create Window */
    SetTargetFPS(60);
    InitWindow(WIDTH, HIGHT, "test");
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        /* Camera */{
        cam.position.x += HorizontalInput() * cam.speed;
        cam.position.y += -VerticalInput() * cam.speed;
        cam.zoom += cam.zoomSpeed * GetMouseWheelMove();
        if(cam.zoom < 1) cam.zoom = 1;
        if(cam.zoom > 6) cam.zoom = 6;
        }
        
        
        for(int y = HIGHT; y > 0; y--){
            for(int x = WIDTH; x > 0; x--){
                /* Rules */
                if(MooreNeighborhood(x - 1, y - 1) < types[pixels[y - 1][x - 1].pixelType].maxNeighbors + 1 && MooreNeighborhood(x - 1, y - 1) > types[pixels[y - 1][x - 1].pixelType].minNeighbors - 1){
                    pixelsToShow[y - 1][x - 1] = 1;
                }
                else{
                    pixelsToShow[y - 1][x - 1] = 0;
                }
                
            }
        }
        
        /* Draw Pixels */
        for(int y = HIGHT; y > 0; y--){
            for(int x = WIDTH; x > 0; x--){
                pixels[y - 1][x - 1].pixelType = pixelsToShow[y - 1][x - 1];
                
                if(pixels[y - 1][x - 1].pixelType != 0){
                    DrawRectangle((x - 1 - cam.position.x) * cam.zoom, (y - 1 - cam.position.y) * cam.zoom, cam.zoom, cam.zoom, types[pixels[y - 1][x - 1].pixelType].c);
                }
            }
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

MooreNeighborhood(int x, int y){
    int output = 0;
    
    /* Top Left */
    int topLeftX = x - 1;
    int topLeftY = y - 1;
    if(topLeftX < 0){
        topLeftX = WIDTH - 1;
    }
    if(topLeftY < 0){
        topLeftY = HIGHT - 1;
    }
    
    /* Top Middle */
    int topMiddleX = x;
    int topMiddleY = y - 1;
    if(topMiddleY < 0){
        topMiddleY = HIGHT - 1;
    }
    
    /* Top Right */
    int topRightX = x + 1;
    int topRightY = y - 1;
    if(topRightX > WIDTH){
        topRightX = 1;
    }
    if(topRightY < 0){
        topRightY = HIGHT - 1;
    }
    
    //-------------------------------------------------
    
    /* Middle Left */
    int middleLeftX = x - 1;
    int middleLeftY = y;
    if(middleLeftX < 1){
        middleLeftX = WIDTH - 1;
    }
    
    /* Middle Right */
    int middleRightX = x + 1;
    int middleRightY = y;
    if(middleRightX > WIDTH - 1){
        middleRightX = 0;
    }
    
    //-------------------------------------------------
    
    /* Bottom Left */
    int bottomLeftX = x - 1;
    int bottomLeftY = y + 1;
    if(bottomLeftX < 1){
        bottomLeftX = WIDTH - 1;
    }
    if(bottomLeftY > HIGHT - 1){
        bottomLeftY = 0;
    }
    
    /* Bottom Middle */
    int bottomMiddleX = x;
    int bottomMiddleY = y + 1;
    if(bottomMiddleY > HIGHT - 1){
        bottomMiddleY = 0;
    }
    
    /* Bottom Right */
    int bottomRightX = x + 1;
    int bottomRightY = y + 1;
    if(bottomRightX > WIDTH - 1){
        bottomRightX = 1;
    }
    if(bottomRightY > HIGHT){
        bottomRightY = 1;
    }
    
    //-------------------------------------------------
    
    for(int i = 10; i>0; i--){
        int t = types[pixels[y][x].pixelType].interactablePixel[i - 1];
        if(pixels[topLeftY][topLeftX].pixelType == t && types[pixels[y - 1][x - 1].pixelType].whatDirections[0] == 1) output += 1;
        if(pixels[topMiddleY][topMiddleX].pixelType == t&& types[pixels[y - 1][x - 1].pixelType].whatDirections[1] == 1) output += 1;
        if(pixels[topRightY][topRightX].pixelType == t&& types[pixels[y - 1][x - 1].pixelType].whatDirections[2] == 1) output += 1;
    
        if(pixels[middleLeftY][middleLeftX].pixelType == t&& types[pixels[y - 1][x - 1].pixelType].whatDirections[3] == 1) output += 1;
        if(pixels[middleRightY][middleRightX].pixelType == t&& types[pixels[y - 1][x - 1].pixelType].whatDirections[4] == 1) output += 1;
    
        if(pixels[bottomLeftY][bottomLeftX].pixelType == t&& types[pixels[y - 1][x - 1].pixelType].whatDirections[5] == 1) output += 1;
        if(pixels[bottomMiddleY][bottomMiddleX].pixelType == t&& types[pixels[y - 1][x - 1].pixelType].whatDirections[6] == 1) output += 1;
        if(pixels[bottomRightY][bottomRightX].pixelType == t&& types[pixels[y - 1][x - 1].pixelType].whatDirections[7] == 1) output += 1;
    }
    
    return output;
}

int HorizontalInput(){
    int input;
    
        if(IsKeyDown(KEY_D) && IsKeyDown(KEY_A)) input = 0;
        else if(IsKeyDown(KEY_D)) input = 1;
        else if(IsKeyDown(KEY_A)) input = -1;
        else input = 0;
    
    return input;
}

int VerticalInput(){
    int input;
        if(IsKeyDown(KEY_W) && IsKeyDown(KEY_S)) input = 0;
        else if(IsKeyDown(KEY_W)) input = 1;
        else if(IsKeyDown(KEY_S)) input = -1;
        else input = 0;
    
    return input;
}
