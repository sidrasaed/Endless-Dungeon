#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILE_SIZE 20
#define GRID_WIDTH (SCREEN_WIDTH / TILE_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / TILE_SIZE)
#define MAX_ROOMS 15
#define MIN_ROOM_SIZE 4
#define MAX_ROOM_SIZE 10

// Tile types
typedef enum {
    TILE_EMPTY = 0,
    TILE_WALL,
    TILE_FLOOR,
    TILE_CORRIDOR,
    TILE_START,
    TILE_END
} TileType;

// Room structure
typedef struct {
    int x;
    int y;
    int width;
    int height;
    bool connected;
} Room;

// Game grid
int grid[GRID_WIDTH][GRID_HEIGHT];
Room rooms[MAX_ROOMS];
int roomCount = 0;
Vector2 startPos;
Vector2 endPos;

// Function prototypes
void InitGrid(void);
void GenerateDungeon(void);
bool CreateRoom(int x, int y, int width, int height);
void ConnectRooms(void);
void CreateCorridor(int x1, int y1, int x2, int y2);
void PlaceStartEnd(void);
void DrawDungeon(void);

int main(void) {
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Endless Dungeon");
    SetTargetFPS(60);
    
    // Generate initial dungeon
    GenerateDungeon();
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        if (IsKeyPressed(KEY_SPACE)) {
            GenerateDungeon();
        }
        
        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            DrawDungeon();
            
            // Draw instructions
            DrawText("Press SPACE to generate a new dungeon", 20, SCREEN_HEIGHT - 30, 20, LIGHTGRAY);
        EndDrawing();
    }
    
    // Close window
    CloseWindow();
    
    return 0;
}

// Initialize grid with empty tiles
void InitGrid(void) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y] = TILE_WALL;
        }
    }
    roomCount = 0;
}

// Generate a new dungeon
void GenerateDungeon(void) {
    InitGrid();
    
    // Create rooms
    for (int i = 0; i < MAX_ROOMS; i++) {
        int width = MIN_ROOM_SIZE + rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 1);
        int height = MIN_ROOM_SIZE + rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 1);
        int x = rand() % (GRID_WIDTH - width - 2) + 1;
        int y = rand() % (GRID_HEIGHT - height - 2) + 1;
        
        if (CreateRoom(x, y, width, height)) {
            rooms[roomCount].x = x;
            rooms[roomCount].y = y;
            rooms[roomCount].width = width;
            rooms[roomCount].height = height;
            rooms[roomCount].connected = false;
            roomCount++;
        }
    }
    
    // Connect rooms
    ConnectRooms();
    
    // Place start and end positions
    PlaceStartEnd();
}

// Create a room
bool CreateRoom(int x, int y, int width, int height) {
    // Check if the room overlaps with existing rooms
    for (int i = 0; i < roomCount; i++) {
        if (x + width + 1 > rooms[i].x - 1 && x - 1 < rooms[i].x + rooms[i].width + 1 &&
            y + height + 1 > rooms[i].y - 1 && y - 1 < rooms[i].y + rooms[i].height + 1) {
            return false;
        }
    }
    
    // Create the room
    for (int rx = x; rx < x + width; rx++) {
        for (int ry = y; ry < y + height; ry++) {
            if (rx >= 0 && rx < GRID_WIDTH && ry >= 0 && ry < GRID_HEIGHT) {
                grid[rx][ry] = TILE_FLOOR;
            }
        }
    }
    
    return true;
}

// Connect rooms with corridors
void ConnectRooms(void) {
    if (roomCount < 2) return;
    
    // Mark the first room as connected
    rooms[0].connected = true;
    
    // Connect all rooms
    while (true) {
        bool allConnected = true;
        for (int i = 0; i < roomCount; i++) {
            if (!rooms[i].connected) {
                allConnected = false;
                break;
            }
        }
        
        if (allConnected) break;
        
        // Find a connected room
        int connectedRoom = -1;
        for (int i = 0; i < roomCount; i++) {
            if (rooms[i].connected) {
                connectedRoom = i;
                break;
            }
        }
        
        // Find the closest unconnected room
        int closestRoom = -1;
        int minDistance = GRID_WIDTH * GRID_HEIGHT;
        
        for (int i = 0; i < roomCount; i++) {
            if (!rooms[i].connected) {
                int centerX1 = rooms[connectedRoom].x + rooms[connectedRoom].width / 2;
                int centerY1 = rooms[connectedRoom].y + rooms[connectedRoom].height / 2;
                int centerX2 = rooms[i].x + rooms[i].width / 2;
                int centerY2 = rooms[i].y + rooms[i].height / 2;
                
                int distance = abs(centerX1 - centerX2) + abs(centerY1 - centerY2);
                
                if (distance < minDistance) {
                    minDistance = distance;
                    closestRoom = i;
                }
            }
        }
        
        if (closestRoom != -1) {
            // Connect the rooms
            int centerX1 = rooms[connectedRoom].x + rooms[connectedRoom].width / 2;
            int centerY1 = rooms[connectedRoom].y + rooms[connectedRoom].height / 2;
            int centerX2 = rooms[closestRoom].x + rooms[closestRoom].width / 2;
            int centerY2 = rooms[closestRoom].y + rooms[closestRoom].height / 2;
            
            CreateCorridor(centerX1, centerY1, centerX2, centerY2);
            rooms[closestRoom].connected = true;
        }
    }
}

// Create a corridor between two points
void CreateCorridor(int x1, int y1, int x2, int y2) {
    // First go horizontally, then vertically
    int x = x1;
    int y = y1;
    
    while (x != x2) {
        if (x < x2) x++;
        else x--;
        
        if (grid[x][y] == TILE_WALL) {
            grid[x][y] = TILE_CORRIDOR;
        }
    }
    
    while (y != y2) {
        if (y < y2) y++;
        else y--;
        
        if (grid[x][y] == TILE_WALL) {
            grid[x][y] = TILE_CORRIDOR;
        }
    }
}

// Place start and end positions
void PlaceStartEnd(void) {
    if (roomCount < 2) return;
    
    // Place start in the first room
    int startRoomIdx = 0;
    int startX = rooms[startRoomIdx].x + rooms[startRoomIdx].width / 2;
    int startY = rooms[startRoomIdx].y + rooms[startRoomIdx].height / 2;
    grid[startX][startY] = TILE_START;
    startPos = (Vector2){ startX, startY };
    
    // Place end in the last room
    int endRoomIdx = roomCount - 1;
    int endX = rooms[endRoomIdx].x + rooms[endRoomIdx].width / 2;
    int endY = rooms[endRoomIdx].y + rooms[endRoomIdx].height / 2;
    grid[endX][endY] = TILE_END;
    endPos = (Vector2){ endX, endY };
}

// Draw the dungeon
void DrawDungeon(void) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            Rectangle rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            
            switch (grid[x][y]) {
                case TILE_WALL:
                    DrawRectangleRec(rect, DARKGRAY);
                    break;
                case TILE_FLOOR:
                    DrawRectangleRec(rect, GRAY);
                    break;
                case TILE_CORRIDOR:
                    DrawRectangleRec(rect, DARKBLUE);
                    break;
                case TILE_START:
                    DrawRectangleRec(rect, GREEN);
                    break;
                case TILE_END:
                    DrawRectangleRec(rect, RED);
                    break;
                default:
                    break;
            }
            
            // Draw grid lines
            DrawRectangleLinesEx(rect, 1, BLACK);
        }
    }
}