#include <raylib.h>
#include <stdio.h>
#include <time.h>

struct Camera{
    Vector2 position;
    float speed, zoomSpeed, zoom;
};

#define WIDTH 1280
#define HEIGHT 720

int pixels[HEIGHT][WIDTH];
int pixelsToShow[HEIGHT][WIDTH];

int main(){
    
    srand(time(NULL));
    
    /* Init Camera */
    struct Camera cam = (struct Camera){.position = (Vector2){0,0}, .speed = 4, .zoomSpeed = 0.1, .zoom = 1};
    
    /* Initialize Pixel Values */
    for(int y = HEIGHT; y > 0; y--){
        for(int x = WIDTH; x > 0; x--){
            pixels[y - 1][x - 1] = rand() % 2;
        }
    }
    
    /* Colors */
    Color c[3]; /* For How Many Pixels */
    
    c[0] = (Color){0,0,0,255}; /* Empty */
    c[1] = (Color){255,255,255,255}; /* Alive (White) */
    
    /* Create Window */
    SetTargetFPS(60);
    InitWindow(WIDTH, HEIGHT, "Day and Night");
    
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        /* Camera */
        cam.position.x += HorizontalInput() * cam.speed;
        cam.position.y += -VerticalInput() * cam.speed;
        cam.zoom += cam.zoomSpeed * GetMouseWheelMove();
        if(cam.zoom < 1) cam.zoom = 1;
        else if(cam.zoom > 6) cam.zoom = 6;
        
        /* Rules */
        for(int y = HEIGHT; y > 0; y--){
            for(int x = WIDTH; x > 0; x--){
                int pix = pixels[y - 1][x - 1];
                int t = MooreNeighborhood(x - 1, y - 1);
                if(pix == 0 && (t == 3 || t == 6 || t == 7 || t == 8)) pixelsToShow[y - 1][x - 1] = 1;
                else pixelsToShow[y - 1][x - 1] = 0;
                if(pix == 1 && (t == 3 || t == 4 || t == 6 || t == 7 || t == 8)) pixelsToShow[y - 1][x - 1] = 1;
            }
        }
        
        /* Draw Pixels */
        for(int y = HEIGHT; y > 0; y--){
            for(int x = WIDTH; x > 0; x--){
                pixels[y - 1][x - 1] = pixelsToShow[y - 1][x - 1];
                
                if(pixels[y - 1][x - 1] != 0)DrawRectangle((x - 1 - cam.position.x) * cam.zoom, (y - 1 - cam.position.y) * cam.zoom, cam.zoom, cam.zoom, c[pixels[y - 1][x - 1]]);
                    
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
        topLeftY = HEIGHT - 1;
    }
    
    /* Top Middle */
    int topMiddleX = x;
    int topMiddleY = y - 1;
    if(topMiddleY < 0){
        topMiddleY = HEIGHT - 1;
    }
    
    /* Top Right */
    int topRightX = x + 1;
    int topRightY = y - 1;
    if(topRightX > WIDTH){
        topRightX = 1;
    }
    if(topRightY < 0){
        topRightY = HEIGHT - 1;
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
    if(bottomLeftY > HEIGHT - 1){
        bottomLeftY = 0;
    }
    
    /* Bottom Middle */
    int bottomMiddleX = x;
    int bottomMiddleY = y + 1;
    if(bottomMiddleY > HEIGHT - 1){
        bottomMiddleY = 0;
    }
    
    /* Bottom Right */
    int bottomRightX = x + 1;
    int bottomRightY = y + 1;
    if(bottomRightX > WIDTH - 1){
        bottomRightX = 1;
    }
    if(bottomRightY > HEIGHT){
        bottomRightY = 1;
    }
    
    //-------------------------------------------------
    
        if(pixels[topLeftY][topLeftX] == 1) output += 1;
        if(pixels[topMiddleY][topMiddleX] == 1) output += 1;
        if(pixels[topRightY][topRightX] == 1) output += 1;
    
        if(pixels[middleLeftY][middleLeftX] == 1) output += 1;
        if(pixels[middleRightY][middleRightX] == 1) output += 1;
    
        if(pixels[bottomLeftY][bottomLeftX] == 1) output += 1;
        if(pixels[bottomMiddleY][bottomMiddleX] == 1) output += 1;
        if(pixels[bottomRightY][bottomRightX] == 1) output += 1;
    
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
