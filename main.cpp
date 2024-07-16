#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <vector>

using namespace std;

Color green = Color{173, 204, 96, 255};
Color dark_green = Color{43, 51, 24, 255};

// Tạo môt kiểu dữ liệu với 6 giá trị có thể nhận được
typedef enum GameScreen
{
    MENU = 0,
    IsPlaying,
    Map
} GameScreen;

const int cellSize = 30;
const int cellCount = 25;
const int offset = 75;

double lastUpdateTime = 0;

bool eventLastTriggered(double interval) // Kích hoạt || Khoảng thời gian
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque) // Kiểm tra vị trí có nằm trên cơ thể snake?
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
            return true;
    }
    return false;
}

bool ElementInMap(Vector2 element, vector<Vector2> map) // Kiểm tra vị trí có nằm trên Map
{
    for (unsigned int i = 0; i < map.size(); i++)
    {
        if (Vector2Equals(map[i], element))
            return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool addSegment = false; // thêm bộ phận

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, dark_green);
        }
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (body[0].x == cellCount || body[0].x == -1)
        {
            body[0].x += cellCount;
            body[0].x = (int)body[0].x % cellCount;
        }
        if (body[0].y == cellCount || body[0].y == -1)
        {
            body[0].y += cellCount;
            body[0].y = (int)body[0].y % cellCount;
        }

        if (addSegment)
        {
            addSegment = false;
        }
        else
        {
            body.pop_back();
        }
    }

    void Reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class MapGame
{
public:
    vector<Vector2> map1;
    vector<Vector2> map2;
    vector<Vector2> map3;
    int Select;

    MapGame()
    {
        // Map 1
        map1 = {

        };

        // Map 2
        map2 = {
            {2, 7},
            {3, 7},
            {4, 7},
            {5, 7},
            {6, 7},
            {7, 7},
            {7, 6},
            {7, 5},
            {7, 4},
            {7, 3},
            {7, 2},

            {2, 17},
            {3, 17},
            {4, 17},
            {5, 17},
            {6, 17},
            {7, 17},
            {7, 18},
            {7, 19},
            {7, 20},
            {7, 21},
            {7, 22},

            {17, 7},
            {18, 7},
            {19, 7},
            {20, 7},
            {21, 7},
            {22, 7},
            {17, 6},
            {17, 5},
            {17, 4},
            {17, 3},
            {17, 2},

            {17, 17},
            {18, 17},
            {19, 17},
            {20, 17},
            {21, 17},
            {22, 17},
            {17, 18},
            {17, 19},
            {17, 20},
            {17, 21},
            {17, 22},
        };

        // Map 3
        map3 = {
            // Hàng ngang
            {2, 2},
            {3, 2},
            {4, 2},
            {5, 2},
            {6, 2},
            {2, 22},
            {3, 22},
            {4, 22},
            {5, 22},
            {6, 22},
            {19, 2},
            {20, 2},
            {21, 2},
            {22, 2},
            {19, 22},
            {20, 22},
            {21, 22},
            {22, 22},

            // Hàng dọc
            {2, 3},
            {2, 4},
            {2, 5},
            {2, 6},
            {22, 3},
            {22, 4},
            {22, 5},
            {22, 6},
            {2, 18},
            {2, 19},
            {2, 20},
            {2, 21},
            {22, 18},
            {22, 19},
            {22, 20},
            {22, 21},

            // Hình vuông trung tâm
            {11, 11},
            {12, 11},
            {13, 11},
            {14, 11},
            {15, 11},
            {11, 12},
            {11, 13},
            {11, 14},
            {11, 15},
            {15, 12},
            {15, 13},
            {15, 14},
            {15, 15},

            // Hình chữ nhật trên và dưới
            {7, 7},
            {8, 7},
            {9, 7},
            {10, 7},
            {11, 7},
            {12, 7},
            {7, 8},
            {7, 9},
            {7, 10},
            {7, 11},
            {7, 12},
            {16, 17},
            {17, 17},
            {18, 17},
            {19, 17},
            {20, 17},
            {16, 18},
            {16, 19},
            {16, 20},
            {16, 21},
            {16, 22},

            // Các ô rời rạc
            {1, 10},
            {1, 15},
            {23, 10},
            {23, 15},
            {10, 1},
            {15, 1},
            {10, 23},
            {15, 23},
        };
    }
    void DrawMap1()
    {
        for (unsigned int i = 0; i < map1.size(); i++)
        {
            float x = map1[i].x;
            float y = map1[i].y;
            DrawRectangle(offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize, dark_green);
        }
    }

    void DrawMap2()
    {
        for (unsigned int i = 0; i < map2.size(); i++)
        {
            float x = map2[i].x;
            float y = map2[i].y;
            DrawRectangle(offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize, dark_green);
        }
    }

    void DrawMap3()
    {
        for (unsigned int i = 0; i < map3.size(); i++)
        {
            float x = map3[i].x;
            float y = map3[i].y;
            DrawRectangle(offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize, dark_green);
        }
    }
};

class Food
{
public:
    Vector2 position;
    Texture texture;

    Food(deque<Vector2> snakeBody, MapGame mapGame)
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        if (mapGame.Select == 1)
            position = GenerateRandomPos(snakeBody, mapGame.map1);
        if (mapGame.Select == 2)
            position = GenerateRandomPos(snakeBody, mapGame.map2);
        if (mapGame.Select == 3)
            position = GenerateRandomPos(snakeBody, mapGame.map3);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody, vector<Vector2> map) // Tạo food không trùng với body Snake và Map
    {
        position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody) || ElementInMap(position, map))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game
{
public:
    Snake snake = Snake();
    MapGame mapGame = MapGame();
    Food food = Food(snake.body, mapGame);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;
    Sound soundTrack;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
        soundTrack = LoadSound("Sounds/soundtrack.mp3");
        PlaySound(soundTrack);
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        UnloadSound(soundTrack);
        CloseAudioDevice();
    }

    void Draw()
    {
        snake.Draw();
        food.Draw();
        if (mapGame.Select == 1)
            mapGame.DrawMap1();
        if (mapGame.Select == 2)
            mapGame.DrawMap2();
        if (mapGame.Select == 3)
            mapGame.DrawMap3();
    }

    void Update()
    {
        if (running)
        {
            snake.Update();
            CheckCollisionWithFood();
            // CheckCollisionWithEdges();
            CheckCollisionWithTail();
            if (mapGame.Select == 1)
                CheckCollisionWithMap(mapGame.map1);
            if (mapGame.Select == 2)
                CheckCollisionWithMap(mapGame.map2);
            if (mapGame.Select == 3)
                CheckCollisionWithMap(mapGame.map3);
        }
    }

    void GenerateRandomPosSatisfyMap() // Tạo vị trí thỏa mãn Map
    {
        if (mapGame.Select == 1)
            food.position = food.GenerateRandomPos(snake.body, mapGame.map1);
        if (mapGame.Select == 2)
            food.position = food.GenerateRandomPos(snake.body, mapGame.map2);
        if (mapGame.Select == 3)
            food.position = food.GenerateRandomPos(snake.body, mapGame.map3);
    }

    void CheckCollisionWithFood() // Kiểm tra head của Snake có trùng với food?
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            GenerateRandomPosSatisfyMap();
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    // void CheckCollisionWithEdges() // Kiểm tra Snake có trùng với cạnh của màn hình?
    // {
    //     if (snake.body[0].x == cellCount || snake.body[0].x == -1)
    //     {
    //         PlaySound(wallSound);
    //         GameOver();
    //     }
    //     if (snake.body[0].y == cellCount || snake.body[0].y == -1)
    //     {
    //         PlaySound(wallSound);
    //         GameOver();
    //     }
    // }

    void CheckCollisionWithTail() // Kiểm tra Snake có trùng với phần đuôi của Snake?
    {
        // Tạo thân không có đầu
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        // Kiểm tra phần đầu có trùng với các phần tạo hay không
        if (ElementInDeque(snake.body[0], headlessBody))
        {
            PlaySound(wallSound);
            GameOver();
        }
    }

    void CheckCollisionWithMap(vector<Vector2> map) // Kiểm tra Snake có trùng với Map
    {
        // Kiểm tra phần đầu có trùng với các phần tạo hay không
        if (ElementInMap(snake.body[0], map))
        {
            PlaySound(wallSound);
            GameOver();
        }
    }

    void GameOver()
    {
        snake.Reset();
        GenerateRandomPosSatisfyMap();
        running = false;
        score = 0;
    }
};

int main()
{

    cout << "Starting the game..." << endl;
    // Increate Window
    InitWindow(2 * offset + cellCount * cellSize, 2 * offset + cellCount * cellSize, "Retro Snake");
    // Set FPS
    SetTargetFPS(60);

    // Load Texture
    Image image = LoadImage("Graphics/Snake-Game.png");
    Texture textureMenu = LoadTextureFromImage(image);
    UnloadImage(image);

    // Increate Object
    Game game = Game();

    // Time Smake Move
    float inveral = 0.2;

    // Khởi tạo biến màn hình hiện tại
    GameScreen currentScreen = MENU;

    // Game Loop
    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        // Sound Tracking
        if (eventLastTriggered(87))
        {
            cout << "NewTrack" << endl;
            game.soundTrack = LoadSound("Sound/soundtrack.mp3");
            PlaySound(game.soundTrack);
        }

        // Switch Screen
        if (currentScreen == MENU)
        {
            ClearBackground(green);
            int posX = (2 * offset + cellCount * cellSize - textureMenu.width) / 2;
            int posY = (2 * offset + cellCount * cellSize - textureMenu.height) / 2;
            DrawTexture(textureMenu, posX, posY, green);
            // MENU GAME 1
            DrawText("SNAKE GAME", (2 * offset + cellCount * cellSize) / 2 - MeasureText("SNAKE GAME", 75) / 2, (2 * offset + cellCount * cellSize) / 2 - 250, 80, dark_green);

            // New Game
            Rectangle NewGame = {(2 * offset + cellCount * cellSize) / 2 - 100, (2 * offset + cellCount * cellSize) / 2 - 100, 200, 50};
            DrawRectangleRec(NewGame, LIGHTGRAY);
            DrawText("New Game", NewGame.x + NewGame.width / 2 - MeasureText("New Game", 20) / 2 - 20, NewGame.y + 10, 30, DARKGRAY);

            // Continue
            Rectangle Continue = {(2 * offset + cellCount * cellSize) / 2 - 100, (2 * offset + cellCount * cellSize) / 2 - 20, 200, 50};
            DrawRectangleRec(Continue, LIGHTGRAY);
            DrawText("Continue", Continue.x + Continue.width / 2 - MeasureText("Continue", 20) / 2 - 20, Continue.y + 10, 30, DARKGRAY);

            // Press Select
            // Press New Game
            if (CheckCollisionPointRec(GetMousePosition(), NewGame) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                game.GameOver();
                currentScreen = Map; // Chuyển sang màn hình Select Map
            }

            // Press Continue
            if (CheckCollisionPointRec(GetMousePosition(), Continue) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                currentScreen = IsPlaying; // Chuyển sang màn hình trò chơi
            }
        }

        if (currentScreen == Map)
        {
            ClearBackground(green);
            int posX = (2 * offset + cellCount * cellSize - textureMenu.width) / 2;
            int posY = (2 * offset + cellCount * cellSize - textureMenu.height) / 2;
            DrawTexture(textureMenu, posX, posY, green);
            // MENU GAME
            DrawText("SNAKE GAME", (2 * offset + cellCount * cellSize) / 2 - MeasureText("SNAKE GAME", 75) / 2, (2 * offset + cellCount * cellSize) / 2 - 250, 80, dark_green);

            // Select Map
            // Map 1
            Rectangle Map1 = {(2 * offset + cellCount * cellSize) / 2 - 100, (2 * offset + cellCount * cellSize) / 2 - 100, 200, 50};
            DrawRectangleRec(Map1, LIGHTGRAY);
            DrawText("MAP 1", Map1.x + Map1.width / 2 - MeasureText("MAP 1", 20) / 2 - 20, Map1.y + 10, 30, DARKGRAY);

            // Map 2
            Rectangle Map2 = {(2 * offset + cellCount * cellSize) / 2 - 100, (2 * offset + cellCount * cellSize) / 2 - 20, 200, 50};
            DrawRectangleRec(Map2, LIGHTGRAY);
            DrawText("MAP 2", Map2.x + Map2.width / 2 - MeasureText("MAP 2", 20) / 2 - 20, Map2.y + 10, 30, DARKGRAY);

            // Map 3
            Rectangle Map3 = {(2 * offset + cellCount * cellSize) / 2 - 100, (2 * offset + cellCount * cellSize) / 2 + 60, 200, 50};
            DrawRectangleRec(Map3, LIGHTGRAY);
            DrawText("MAP 3", Map3.x + Map3.width / 2 - MeasureText("MAP 3", 20) / 2 - 20, Map3.y + 10, 30, DARKGRAY);

            // Press Select Map
            // Press Map 1
            if (CheckCollisionPointRec(GetMousePosition(), Map1) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                game.mapGame.Select = 1;
                currentScreen = IsPlaying;
            }

            // Press Map 2
            if (CheckCollisionPointRec(GetMousePosition(), Map2) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                game.mapGame.Select = 2;
                currentScreen = IsPlaying;
            }

            // Press Map 3
            if (CheckCollisionPointRec(GetMousePosition(), Map3) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                game.mapGame.Select = 3;
                currentScreen = IsPlaying;
            }
        }

        if (currentScreen == IsPlaying)
        {
            ClearBackground(green);
            if (eventLastTriggered(inveral)) // Giảm thời gian di chuyển Snake
            {
                game.Update();
                inveral = 0.2;
            }
            // Control Snake
            {
                if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
                {
                    game.snake.direction = Vector2{0, -1};
                    inveral = 0.1;
                    game.running = true;
                }
                if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
                {
                    game.snake.direction = Vector2{0, 1};
                    inveral = 0.1;
                    game.running = true;
                }
                if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
                {
                    game.snake.direction = Vector2{1, 0};
                    inveral = 0.1;
                    game.running = true;
                }
                if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
                {
                    game.snake.direction = Vector2{-1, 0};
                    inveral = 0.1;
                    game.running = true;
                }
            }

            // Drawing Screen Game
            {
                ClearBackground(green);
                DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellCount * cellSize + 10, (float)cellCount * cellSize + 10}, 5, dark_green);
                DrawText("Retro Snake", offset - 5, 20, 40, dark_green);
                DrawText(TextFormat("%i", game.score), offset - 5, offset + cellCount * cellSize + 10, 40, dark_green);
                game.Draw();
            }

            // Reture Screen Menu
            DrawText("Press BACKSPACE to return to MENU", offset * 2 + cellCount * cellSize - MeasureText("Press BACKSPACE to return to MENU", 20) - offset - 5, offset * 2 + cellCount * cellSize - offset + 10, 20, dark_green);
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                currentScreen = MENU;
            }
        }

        EndDrawing();
    }
    UnloadTexture(textureMenu);
    CloseWindow();
    return 0;
}