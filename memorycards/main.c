#define SUPPORT_FILEFORMAT_SVG      1

#include <stdlib.h>
#include "raylib.h"
#include "stdio.h"

char hex[] = "0123456789abcdef";

void printHex(unsigned char byte) {
    printf("%c%c", hex[byte >> 4], hex[byte & 0xf]);
}

typedef struct {
    int screenX;
    int screenY;
    int drawWidth;
    int drawHeight;
    int startAtX;
    int startAtY;
} Dimensions;

int main(void) {

    //DATA
    Texture *emojiTextures;
    int emojiTexturesLength;

    int cardsPerSide = 4;
    int cards[cardsPerSide*cardsPerSide]; //should be CARDS_PER_SIDE * CARDS_PER_SIDE;
    float showCards[cardsPerSide*cardsPerSide]; //should be CARDS_PER_SIDE * CARDS_PER_SIDE;
    int lastSelected = -1;
    int currentSelected = -1;
    bool isMouseDown;
    bool wasMousePressed;


    //SCREEN LAYOUT
    int GAP_BETWEEN_CARDS = 16;
    int GAP_BETWEEN_CARD_EMOJI = 4;
    int CARD_SIZE = 72 + GAP_BETWEEN_CARD_EMOJI;

    int TIMER_BAR_HEIGHT = CARD_SIZE / 3;
    int TIMER_BAR_WIDTH = cardsPerSide * (CARD_SIZE + GAP_BETWEEN_CARDS) - GAP_BETWEEN_CARDS;


    int CARD_LINE_THICKNESS = 3;
    float CARD_ROUNDEDNESS = 0.25f;
    float CARD_ROUND_SEGMENTS = 4;


    //ANIMATION STUFF
    int CARD_PREVIEW_FRAMES = 90;
    int cardPreviewTimer = -1;

    Dimensions  dimensions = {};
    //Compute dimensions
    {
        dimensions.screenX = 9 * 50;
        dimensions.screenY = 16 * 50;
        {
            dimensions.drawWidth = 0;
            dimensions.drawWidth += cardsPerSide*(CARD_SIZE + GAP_BETWEEN_CARDS) + GAP_BETWEEN_CARDS;

            dimensions.drawHeight = 0;
            dimensions.drawHeight += GAP_BETWEEN_CARDS + TIMER_BAR_HEIGHT;
            dimensions.drawHeight += cardsPerSide*(CARD_SIZE + GAP_BETWEEN_CARDS) + GAP_BETWEEN_CARDS;

            dimensions.startAtX = (dimensions.screenX - dimensions.drawWidth) / 2;
            dimensions.startAtY = (dimensions.screenY - dimensions.drawHeight) / 2;
        }


    }
    InitWindow(dimensions.screenX, dimensions.screenY, "raylib [core] example - basic window");

    //Init Data
    //Load Textures
    {
        char *levelFile = "./animals";
        int levelFileSize = 0;
        unsigned char *data = LoadFileData(levelFile, &levelFileSize);
        emojiTexturesLength = GetCodepointCount(data);
        emojiTextures = (Texture *)malloc(emojiTexturesLength * sizeof(Texture));;

        for (int i = 0; i < emojiTexturesLength; ++i) {
            int codePointSize = 0;
            int codePoint = GetCodepointNext(data, &codePointSize);
            {
                char fileName[] = "./openmoji/1F600.png";
                sprintf(fileName, "./openmoji/%X.png", codePoint);
                Image image = LoadImage(fileName);
                ImageResize(&image, CARD_SIZE - GAP_BETWEEN_CARD_EMOJI, CARD_SIZE - GAP_BETWEEN_CARD_EMOJI);
                emojiTextures[i] = LoadTextureFromImage(image);
                UnloadImage(image);
            }
            data += codePointSize;
        }
    }
    {
        for (int i = 0; i < cardsPerSide * cardsPerSide; ++i) {
            cards[i] = i % emojiTexturesLength;
            showCards[i] = 0;
        }
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        //Timers
        if (cardPreviewTimer > 0) {
            cardPreviewTimer -= 1;
        }
        if (cardPreviewTimer == 0) {
            cardPreviewTimer = -1;
            if (cards[currentSelected] == cards[lastSelected]) {
                showCards[currentSelected] = 1 ;
                showCards[lastSelected] = 1 ;
            } else {
                showCards[currentSelected] = 0 ;
                showCards[lastSelected] = 0 ;
            }
            lastSelected = -1;
            currentSelected = -1;
        }

        Vector2  mousePosition = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !isMouseDown) {
            isMouseDown = true;
            wasMousePressed = true;
        } else {
            isMouseDown = false;
        }


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            printf("Pressed");
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            printf("ButtonDown");
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            printf("Released");
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
        int offX = dimensions.startAtX;
        int offY = dimensions.startAtY;
        Rectangle timerRect = {offX + GAP_BETWEEN_CARDS,
                               offY + GAP_BETWEEN_CARDS,
                               TIMER_BAR_WIDTH,
                               TIMER_BAR_HEIGHT};
        DrawRectangleRoundedLines(timerRect,
                                  0.15f,
                                  8,
                                  3,
                                  BLACK);
        offY += TIMER_BAR_HEIGHT + GAP_BETWEEN_CARDS;
        if (cardPreviewTimer >= 0 ) {

            timerRect.width -= timerRect.width * (CARD_PREVIEW_FRAMES - cardPreviewTimer) / CARD_PREVIEW_FRAMES;
            DrawRectangleRounded(timerRect,
                                      0.15f,
                                      8,
                                      GREEN);
        }
        for (int i = 0; i < cardsPerSide; ++i) {
            for (int j = 0; j < cardsPerSide; ++j) {
                Rectangle cardRect = {offX + CARD_SIZE * i + GAP_BETWEEN_CARDS * (i + 1),
                                      offY + CARD_SIZE * j + GAP_BETWEEN_CARDS * (j + 1),
                                      CARD_SIZE,
                                      CARD_SIZE};

                int offsetPosition = i * cardsPerSide + j;
                if (cardPreviewTimer < 0 && wasMousePressed) {
                    if (mousePosition.x > cardRect.x && mousePosition.x < (CARD_SIZE + cardRect.x)) {
                        if (mousePosition.y > cardRect.y && mousePosition.y < (CARD_SIZE + cardRect.y)) {
                            if (lastSelected < 0) {
                                lastSelected = offsetPosition;
                            } else if(lastSelected != offsetPosition){
                                currentSelected = offsetPosition;
                                cardPreviewTimer = CARD_PREVIEW_FRAMES;
                            }
                            if (showCards[offsetPosition] == 0) {
                                showCards[offsetPosition] = 1;
                            }

                        }
                    }
                }

                if (showCards[offsetPosition]) {
                    DrawRectangleRounded(cardRect,
                                         0.15f,
                                         8,
                                         BEIGE);

                    DrawTexture(emojiTextures[cards[offsetPosition]],
                                cardRect.x + GAP_BETWEEN_CARD_EMOJI,
                                cardRect.y + GAP_BETWEEN_CARD_EMOJI, WHITE);
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
        wasMousePressed = false;
    }
    for (int i = 0; i < emojiTexturesLength; ++i) {
        UnloadTexture(emojiTextures[i]);
    }
    CloseWindow();

    return 0;
}