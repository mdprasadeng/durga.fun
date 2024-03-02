#define SUPPORT_FILEFORMAT_SVG      1

#include "raylib.h"
#include "stdio.h"

char hex[] = "0123456789abcdef";

void printHex(unsigned char byte) {
    printf("%c%c", hex[byte >> 4], hex[byte & 0xf]);
}

int main(void) {
    int screenX = 9 * 50;
    int screenY = 16 * 50;


    int minScreenXY = (screenX < screenY) ? screenX : screenY;
    int cardsGap = 16;
    int cardIconGap = 8;
    int cardSizePlusGap = minScreenXY / 5;
    int cardSize = cardSizePlusGap - cardsGap;
    int iconSize = cardSize - cardIconGap;
    int gridSize = 4;

    int order[] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0};
    int show[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lastSelected = -1;
    int currentSelected = -1;
    int displayTimer;
    int DISPLAY_FOR_FRAMES = 60;

    InitWindow(screenX, screenY, "raylib [core] example - basic window");

    char *levelFile = "./animals";
    int levelFileSize = 0;
    unsigned char *data = LoadFileData(levelFile, &levelFileSize);
    int totalPointsCount = GetCodepointCount(data);
    int dataLines[totalPointsCount];
    Texture2D textures[totalPointsCount];
    Image images[totalPointsCount];
    for (int i = 0; i < totalPointsCount; ++i) {
        int codePointSize = 0;
        dataLines[i] = GetCodepointNext(data, &codePointSize);
        {
            char fileName[] = "./openmoji/1F600.png";
            sprintf(fileName, "./openmoji/%X.png", dataLines[i]);
            images[i] = LoadImage(fileName);
            ImageResize(&images[i], iconSize, iconSize);
            textures[i] = LoadTextureFromImage(images[i]);
        }
        data += codePointSize;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        //Timers
        if (displayTimer >= 0) {
            displayTimer -= 1;
        }

        Vector2  mousePosition = GetMousePosition();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        int offX = screenX / 2 - cardSizePlusGap - cardSize - cardsGap / 2;
        int offY = screenY / 2 - cardSizePlusGap - cardSize - cardsGap / 2;
        if (displayTimer >= 0 ) {
            
        }
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                Rectangle cardRect = {offX + cardSizePlusGap * i,
                                      offY + cardSizePlusGap * j,
                                      cardSize,
                                      cardSize};

                int offsetPosition = i * gridSize + j;
                if (displayTimer < 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (mousePosition.x > cardRect.x && mousePosition.x < (cardSize + cardRect.x)) {
                        if (mousePosition.y > cardRect.y && mousePosition.y < (cardSize + cardRect.y)) {

                            if (lastSelected < 0) {
                                lastSelected = offsetPosition;
                            } else if(lastSelected != offsetPosition){
                                currentSelected = offsetPosition;
                                displayTimer = DISPLAY_FOR_FRAMES;
                                if (order[currentSelected] == order[lastSelected]) {

                                } else {

                                }
                            }
                            if (show[offsetPosition]) {
                                show[offsetPosition] = 0;
                            } else {
                                show[offsetPosition] = 1;
                            }

                        }
                    }
                }

                if (show[offsetPosition]) {
                    DrawRectangleRounded(cardRect,
                                         0.15f,
                                         8,
                                         BEIGE);

                    DrawTexture(textures[order[offsetPosition]],
                                offX + cardSizePlusGap * i + cardIconGap / 2,
                                offY + cardSizePlusGap * j + cardIconGap / 2, WHITE);
                } else {
                    DrawRectangleRounded(cardRect,
                                         0.15f,
                                         8,
                                         BROWN);

                }
                DrawRectangleRoundedLines(cardRect,
                                          0.15f,
                                          8,
                                          2.5f,
                                          BLACK);

            }
        }
        EndDrawing();
    }
    for (int i = 0; i < totalPointsCount; ++i) {
        UnloadTexture(textures[i]);
        UnloadImage(images[i]);
    }
    CloseWindow();

    return 0;
}