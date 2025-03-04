#include "game.h"

// Hàm khởi tạo trò chơi
Game::Game()
{
    string resPath = getResourcePath();  // Lấy đường dẫn tài nguyên
    backgroundImage = loadTexture(resPath + "map.png", Globals::renderer);  // Tải hình nền

    // Danh sách các kiểu dữ liệu nhóm mà các khung hình có thể có
    list<DataGroupType> dataGroupTypes;

    // Định nghĩa các kiểu dữ liệu cho khung hình
    // Hộp va chạm (collisionBox)
    DataGroupType colBoxType;
    colBoxType.groupName = "collisionBox";  // Tên nhóm là "collisionBox"
    colBoxType.dataType = DataGroupType::DATATYPE_BOX;  // Kiểu dữ liệu là hộp

    // Hộp gây sát thương (hitBox)
    DataGroupType hitBoxType;
    hitBoxType.groupName = "hitBox";  // Tên nhóm là "hitBox"
    hitBoxType.dataType = DataGroupType::DATATYPE_BOX;  // Kiểu dữ liệu là hộp

    // Sát thương (damage)
    DataGroupType dmgType;
    dmgType.groupName = "damage";  // Tên nhóm là "damage"
    dmgType.dataType = DataGroupType::DATATYPE_NUMBER;  // Kiểu dữ liệu là số

    // Thêm các kiểu dữ liệu vào danh sách
    dataGroupTypes.push_back(colBoxType);
    dataGroupTypes.push_back(hitBoxType);
    dataGroupTypes.push_back(dmgType);

    // Tạo và tải tập hợp animation cho Hero
    heroAnimSet = new AnimationSet();
    heroAnimSet->loadAnimationSet("udemyCyborg.fdset", dataGroupTypes, true, 0, true);

    // Tạo và tải tập hợp animation cho Wall
    wallAnimSet = new AnimationSet();
    wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

    // Tạo thực thể Hero
    hero = new Hero(heroAnimSet);
    hero->invincibleTimer = 0;  // Đặt thời gian bất tử ban đầu là 0
    hero->x = Globals::ScreenWidth / 2;  // Đặt vị trí x giữa màn hình
    hero->y = Globals::ScreenHeight / 2;  // Đặt vị trí y giữa màn hình
    heroInput.hero = hero;  // Gán Hero cho KeyboardInput để điều khiển
    Entity::entities.push_back(hero);  // Thêm Hero vào danh sách thực thể

    int tileSize = 32;  // Kích thước mỗi ô là 32 pixel

    // Tạo các bức tường cho trò chơi
    // Tạo tường trên và dưới màn hình
    for (int i = 0; i < Globals::ScreenWidth / tileSize; i++)
    {
        // Tạo tường hàng trên
        Wall* newWall = new Wall(wallAnimSet);
        newWall->x = i * tileSize + tileSize / 2;  // Đặt vị trí x
        newWall->y = tileSize / 2;  // Đặt vị trí y (gần đỉnh màn hình)
        walls.push_back(newWall);  // Thêm vào danh sách tường
        Entity::entities.push_back(newWall);  // Thêm vào danh sách thực thể

        // Tái sử dụng con trỏ để tạo hàng dưới
        newWall = new Wall(wallAnimSet);
        newWall->x = i * tileSize + tileSize / 2;  // Đặt vị trí x
        newWall->y = Globals::ScreenHeight - tileSize / 2;  // Đặt vị trí y (gần đáy màn hình)
        walls.push_back(newWall);  // Thêm vào danh sách tường
        Entity::entities.push_back(newWall);  // Thêm vào danh sách thực thể
    }

    // Tạo tường hai bên màn hình
    for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++)
    {
        // Tạo tường cột trái
        Wall* newWall = new Wall(wallAnimSet);
        newWall->x = tileSize / 2;  // Đặt vị trí x (gần cạnh trái)
        newWall->y = i * tileSize + tileSize / 2;  // Đặt vị trí y
        walls.push_back(newWall);  // Thêm vào danh sách tường
        Entity::entities.push_back(newWall);  // Thêm vào danh sách thực thể

        // Tái sử dụng con trỏ để tạo cột phải
        newWall = new Wall(wallAnimSet);
        newWall->x = Globals::ScreenWidth - tileSize / 2;  // Đặt vị trí x (gần cạnh phải)
        newWall->y = i * tileSize + tileSize / 2;  // Đặt vị trí y
        walls.push_back(newWall);  // Thêm vào danh sách tường
        Entity::entities.push_back(newWall);  // Thêm vào danh sách thực thể
    }
}

// Hàm hủy trò chơi để giải phóng tài nguyên
Game::~Game()
{
    cleanup(backgroundImage);  // Giải phóng texture của hình nền

    Entity::removeAllFromList(&Entity::entities, false);  // Xóa tất cả thực thể khỏi danh sách (không xóa đối tượng)

    delete heroAnimSet;  // Giải phóng tập hợp animation của Hero
    delete wallAnimSet;  // Giải phóng tập hợp animation của Wall

    delete hero;  // Giải phóng đối tượng Hero

    Entity::removeAllFromList(&walls, true);  // Xóa và giải phóng tất cả các tường
}

// Hàm cập nhật trạng thái trò chơi
void Game::update()
{
    bool quit = false;  // Biến kiểm soát vòng lặp game

    SDL_Event e;  // Biến để xử lý sự kiện SDL
    TimeController::timeController.reset();  // Đặt lại bộ đếm thời gian trước khi bắt đầu game

    // Vòng lặp chính của game
    while (!quit)
    {
        TimeController::timeController.updateTime();  // Cập nhật thời gian

        Entity::removeInactiveEntitiesFromList(&Entity::entities, false);  // Xóa các thực thể không hoạt động

        // Kiểm tra các sự kiện SDL
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)  // Nếu người dùng đóng cửa sổ
                quit = true;
            if (e.type == SDL_KEYDOWN)  // Nếu có phím được nhấn
            {
                switch (e.key.keysym.scancode)  // Xử lý theo phím được nhấn
                {
                case SDL_SCANCODE_ESCAPE:  // Phím ESC
                    quit = true;  // Thoát game
                    break;
                case SDL_SCANCODE_SPACE:  // Phím Space
                    hero->revive();  // Hồi sinh Hero
                    break;
                }
            }
            heroInput.update(&e);  // Cập nhật đầu vào từ bàn phím cho Hero
        }

        // Cập nhật tất cả thực thể
        for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            (*entity)->update();  // Gọi hàm update cho từng thực thể (đa hình)
        }

        draw();  // Vẽ mọi thứ lên màn hình
    }
}

// Hàm vẽ các thành phần của trò chơi
void Game::draw()
{
    // Xóa màn hình với màu nền
    SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Globals::renderer);

    // Vẽ hình nền
    renderTexture(backgroundImage, Globals::renderer, 0, 0);

    // Sắp xếp các thực thể theo độ sâu (y) để vẽ đúng thứ tự
    Entity::entities.sort(Entity::EntityCompare);

    // Vẽ tất cả thực thể
    for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
    {
        (*entity)->draw();  // Gọi hàm draw cho từng thực thể
    }

    // Hiển thị nội dung đã vẽ lên màn hình
    SDL_RenderPresent(Globals::renderer);
}