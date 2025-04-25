#include "game.h"

// Constructor của lớp Game
Game::Game()
{
    // Khởi tạo đường dẫn tới tài nguyên
    string resPath = getResourcePath();

    // Tải các texture cho game
    backgroundImage = loadTexture(resPath + "map.png", Globals::renderer); // Tải ảnh nền
    splashImage = loadTexture(resPath + "cyborgtitle.png", Globals::renderer); // Tải ảnh màn hình splash
    overlayImage = loadTexture(resPath + "overlay.png", Globals::renderer); // Tải ảnh overlay khi thua
    victoryImage = loadTexture(resPath + "victory.png", Globals::renderer); // Tải ảnh màn hình chiến thắng

    // Thiết lập trạng thái ban đầu của game
    splashShowing = true; // Hiển thị màn hình splash khi bắt đầu
    overlayTimer = 2; // Thiết lập bộ đếm thời gian hiển thị overlay
    gameWon = false; // Thiết lập trạng thái chưa chiến thắng

    // Thiết lập camera ban đầu
    Globals::camera.x = 0; // Vị trí x của camera
    Globals::camera.y = 0; // Vị trí y của camera
    Globals::camera.w = Globals::ScreenWidth; // Chiều rộng camera
    Globals::camera.h = Globals::ScreenHeight; // Chiều cao camera

    // Tải và thiết lập các âm thanh
    SoundManager::soundManager.loadSound("hit", resPath + "Randomize2.wav"); // Âm thanh khi nhân vật bị đánh
    SoundManager::soundManager.loadSound("enemyHit", resPath + "Hit_Hurt9.wav"); // Âm thanh khi đánh trúng boss
    SoundManager::soundManager.loadSound("swing", resPath + "Randomize21.wav"); // Âm thanh khi nhân vật tấn công
    SoundManager::soundManager.loadSound("dash", resPath + "dash.wav"); // Âm thanh khi nhân vật lướt
    SoundManager::soundManager.loadSound("enemyDie", resPath + "Randomize41.wav"); // Âm thanh khi quái chết

    // Tải và phát nhạc nền
    song = Mix_LoadMUS(string(resPath + "musictheme.wav").c_str()); // Tải file nhạc nền
    if (song != NULL)
    {
        Mix_PlayMusic(song, -1); // Phát nhạc nền lặp lại vô hạn
    }

    // Thiết lập các loại dữ liệu nhóm (collision, hitbox, damage)
    list<DataGroupType> dataGroupTypes;
    DataGroupType colBoxType = { "collisionBox", DataGroupType::DATATYPE_BOX }; // Hộp va chạm
    DataGroupType hitBoxType = { "hitBox", DataGroupType::DATATYPE_BOX }; // Hộp tấn công
    DataGroupType dmgType = { "damage", DataGroupType::DATATYPE_NUMBER }; // Giá trị sát thương
    dataGroupTypes.push_back(colBoxType); // Thêm hộp va chạm vào danh sách
    dataGroupTypes.push_back(hitBoxType); // Thêm hộp tấn công vào danh sách
    dataGroupTypes.push_back(dmgType); // Thêm giá trị sát thương vào danh sách

    // Thiết lập giao diện người dùng
    playerHpBar = HPBar(Globals::ScreenWidth - 210, 10, 200, 30, nullptr, { 0, 255, 0, 255 }); // Thanh máu của nhân vật chính

    // Tải các bộ animation
    heroAnimSet = new AnimationSet(); // Tạo bộ animation cho nhân vật chính
    heroAnimSet->loadAnimationSet("udemyCyborg.fdset", dataGroupTypes, true, 0, true); // Tải animation nhân vật

    globAnimSet = new AnimationSet(); // Tạo bộ animation cho quái Glob
    globAnimSet->loadAnimationSet("glob.fdset", dataGroupTypes, true, 0, true); // Tải animation quái Glob

    wallAnimSet = new AnimationSet(); // Tạo bộ animation cho tường
    wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes); // Tải animation tường

    // Thiết lập nhân vật chính
    hero = new Hero(heroAnimSet); // Tạo nhân vật chính với bộ animation
    hero->invincibleTimer = 0; // Thiết lập thời gian bất tử ban đầu
    hero->x = Globals::ScreenWidth / 2; // Đặt vị trí x ban đầu ở giữa màn hình
    hero->y = Globals::ScreenHeight / 2; // Đặt vị trí y ban đầu ở giữa màn hình
    heroInput.hero = hero; // Gán đầu vào bàn phím cho nhân vật chính
    Entity::entities.push_back(hero); // Thêm nhân vật chính vào danh sách thực thể
    camController.target = hero; // Thiết lập camera theo dõi nhân vật chính

    // Gắn thanh máu cho nhân vật chính
    playerHpBar.entity = hero; // Gán thực thể cho thanh máu
    playerHpBar.currentHp = hero->hpMax; // Thiết lập máu hiện tại bằng máu tối đa

    // Tạo các bức tường bao quanh màn chơi
    int tileSize = 32; // Kích thước mỗi ô tường
    for (int i = 0; i < Globals::ScreenWidth / tileSize; i++)
    {
        Wall* newWall = new Wall(wallAnimSet); // Tạo tường mới
        newWall->x = i * tileSize + tileSize / 2; // Đặt vị trí x cho tường trên
        newWall->y = tileSize / 2; // Đặt vị trí y cho tường trên
        walls.push_back(newWall); // Thêm tường vào danh sách
        Entity::entities.push_back(newWall); // Thêm tường vào danh sách thực thể

        newWall = new Wall(wallAnimSet); // Tạo tường mới
        newWall->x = i * tileSize + tileSize / 2; // Đặt vị trí x cho tường dưới
        newWall->y = Globals::ScreenHeight - tileSize / 2; // Đặt vị trí y cho tường dưới
        walls.push_back(newWall); // Thêm tường vào danh sách
        Entity::entities.push_back(newWall); // Thêm tường vào danh sách thực thể
    }
    for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++)
    {
        Wall* newWall = new Wall(wallAnimSet); // Tạo tường mới
        newWall->x = tileSize / 2; // Đặt vị trí x cho tường trái
        newWall->y = i * tileSize + tileSize / 2; // Đặt vị trí y cho tường trái
        walls.push_back(newWall); // Thêm tường vào danh sách
        Entity::entities.push_back(newWall); // Thêm tường vào danh sách thực thể

        newWall = new Wall(wallAnimSet); // Tạo tường mới
        newWall->x = Globals::ScreenWidth - tileSize / 2; // Đặt vị trí x cho tường phải
        newWall->y = i * tileSize + tileSize / 2; // Đặt vị trí y cho tường phải
        walls.push_back(newWall); // Thêm tường vào danh sách
        Entity::entities.push_back(newWall); // Thêm tường vào danh sách thực thể
    }

    // Tải animation cho boss và đạn
    bossAnimSet = new AnimationSet(); // Tạo bộ animation cho boss
    bossAnimSet->loadAnimationSet("boss.fdset", dataGroupTypes, true, 0, true); // Tải animation boss

    bulletAnimSet = new AnimationSet(); // Tạo bộ animation cho đạn
    bulletAnimSet->loadAnimationSet("bullet.fdset", dataGroupTypes, true, 0, true); // Tải animation đạn
}

// Destructor của lớp Game
Game::~Game()
{
    // Dọn dẹp các texture
    cleanup(backgroundImage); // Giải phóng texture ảnh nền
    cleanup(splashImage); // Giải phóng texture màn hình splash
    cleanup(overlayImage); // Giải phóng texture overlay
    cleanup(victoryImage); // Giải phóng texture màn hình chiến thắng
    if (scoreTexture != NULL)
    {
        cleanup(scoreTexture); // Giải phóng texture điểm số nếu có
    }
    if (pauseTexture != nullptr) {
        cleanup(pauseTexture); // Giải phóng texture thông báo tạm dừng nếu có
    }
    if (muteTexture != nullptr) {
        cleanup(muteTexture); // Giải phóng texture thông báo tắt âm nếu có
    }

    // Dọn dẹp các thanh máu của boss
    for (auto bar : bossHealthBars) {
        delete bar; // Xóa từng thanh máu
    }
    bossHealthBars.clear(); // Xóa danh sách thanh máu

    // Dọn dẹp âm thanh
    Mix_PausedMusic(); // Tạm dừng nhạc
    Mix_FreeMusic(song); // Giải phóng nhạc nền

    // Dọn dẹp các thực thể
    Entity::removeAllFromList(&Entity::entities, false); // Xóa tất cả thực thể không xóa bộ nhớ
    Entity::removeAllFromList(reinterpret_cast<std::list<Entity*>*>(&walls), true); // Xóa và giải phóng bộ nhớ tường
    Entity::removeAllFromList(&enemies, true); // Xóa và giải phóng bộ nhớ quái

    // Dọn dẹp các bộ animation
    delete heroAnimSet; // Giải phóng bộ animation nhân vật chính
    delete globAnimSet; // Giải phóng bộ animation quái Glob
    delete wallAnimSet; // Giải phóng bộ animation tường
    delete bossAnimSet; // Giải phóng bộ animation boss
    delete bulletAnimSet; // Giải phóng bộ animation đạn

    // Dọn dẹp nhân vật chính
    delete hero; // Giải phóng bộ nhớ nhân vật chính
}

// Vòng lặp chính của game
void Game::update()
{
    bool quit = false; // Biến kiểm tra thoát game

    SDL_Event e; // Biến sự kiện SDL
    TimeController::timeController.reset(); // Đặt lại bộ đếm thời gian
    while (!quit)
    {
        TimeController::timeController.updateTime(); // Cập nhật thời gian

        // Xóa các thực thể không hoạt động
        Entity::removeInactiveEntitiesFromList(&Entity::entities, false); // Xóa thực thể không hoạt động
        Entity::removeInactiveEntitiesFromList(&enemies, true); // Xóa và giải phóng quái không hoạt động

        // Xử lý sự kiện SDL
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true; // Thoát game nếu nhấn nút thoát
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    quit = true; // Thoát game khi nhấn ESC
                    break;
                case SDL_SCANCODE_SPACE:
                    if (splashShowing)
                        splashShowing = false; // Tắt màn hình splash khi nhấn phím cách
                    else if (overlayTimer <= 0 && hero->hp <= 0)
                    {
                        resetGame(); // Reset game khi nhân vật chết và nhấn phím cách
                    }
                    break;
                case SDL_SCANCODE_P:
                    isPaused = !isPaused; // Tạm dừng hoặc tiếp tục game khi nhấn P
                    break;
                case SDL_SCANCODE_O:
                    isMuted = !isMuted; // Tắt hoặc bật âm thanh khi nhấn O
                    if (isMuted)
                    {
                        Mix_VolumeMusic(0); // Tắt nhạc
                        SoundManager::soundManager.muteAllSounds(); // Tắt tất cả âm thanh
                    }
                    else
                    {
                        Mix_VolumeMusic(MIX_MAX_VOLUME); // Bật nhạc
                        SoundManager::soundManager.unmuteAllSounds(); // Bật tất cả âm thanh
                    }
                    break;
                }
            }
            heroInput.update(&e); // Cập nhật đầu vào cho nhân vật chính
        }

        // Nếu game đang tạm dừng, chỉ vẽ màn hình
        if (isPaused)
        {
            draw(); // Vẽ màn hình tạm dừng
            continue;
        }

        // Giảm thời gian overlay khi nhân vật chết
        if (hero->hp <= 0 && overlayTimer > 0)
        {
            overlayTimer -= TimeController::timeController.dT; // Giảm bộ đếm thời gian overlay
        }

        // Cập nhật trạng thái tất cả thực thể
        for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            (*entity)->update(); // Cập nhật từng thực thể
        }

        // Cập nhật logic game nếu nhân vật còn sống và không ở màn hình splash
        if (hero->hp > 0 && !splashShowing && !gameWon) // Không cập nhật nếu đã thắng
        {
            // Chuyển sang vòng tiếp theo hoặc chiến thắng
            if (enemiesToBuild == enemiesBuilt && enemies.size() == 0 && !bossActive)
            {
                if (currentRound == 10)
                {
                    gameWon = true; // Thiết lập trạng thái chiến thắng ở vòng cuối
                }
                else
                {
                    currentRound++; // Tăng số vòng
                    setupNextRound(); // Thiết lập vòng tiếp theo
                }
            }

            // Giảm thời gian tạo quái
            enemyBuildTimer -= TimeController::timeController.dT;

            // Tạo quái mới nếu chưa đủ số lượng và không phải vòng boss
            if (!buildBossNext && !bossActive && enemiesBuilt < enemiesToBuild && enemyBuildTimer <= 0)
            {
                if (enemiesBuilt < numGlobs)
                {
                    // Tạo quái Glob ngẫu nhiên trên màn hình
                    int x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    int y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;

                    Glob* enemy = new Glob(globAnimSet); // Tạo quái Glob mới
                    enemy->x = x; // Đặt vị trí x
                    enemy->y = y; // Đặt vị trí y
                    enemy->invincibleTimer = 0.1; // Thiết lập thời gian bất tử
                    enemy->speed *= 1.0 + (currentRound - 1) * 0.2; // Tăng tốc độ theo vòng
                    enemy->damage = 5 * currentRound; // Tăng sát thương theo vòng
                    enemy->hp = 1; // Thiết lập máu

                    enemies.push_back(enemy); // Thêm quái vào danh sách
                    Entity::entities.push_back(enemy); // Thêm quái vào danh sách thực thể
                    enemiesBuilt++; // Tăng số quái đã tạo
                    enemyBuildTimer = 0.5; // Đặt lại thời gian tạo quái
                }
                else if (enemiesBuilt < numGlobs + numGrobs)
                {
                    // Tạo quái Grob (chưa được triển khai đầy đủ trong code gốc)
                    int x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    int y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
                }
            }

            // Tạo boss khi cần
            if (buildBossNext && enemies.size() == 0 && !bossActive)
            {
                bossHealthBars.clear(); // Xóa các thanh máu boss cũ

                // Tạo boss
                for (int i = 0; i < bossCount; i++)
                {
                    Boss* round = new Boss(bossAnimSet, bulletAnimSet); // Tạo boss mới
                    round->invincibleTimer = 0.1; // Thiết lập thời gian bất tử
                    round->damage = 10 * currentRound; // Thiết lập sát thương theo vòng
                    round->hp = 100 + currentRound * 20; // Thiết lập máu theo vòng
                    round->hpMax = round->hp; // Lưu giá trị máu tối đa

                    // Đặt vị trí ngẫu nhiên cho boss
                    round->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    round->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;

                    enemies.push_back(round); // Thêm boss vào danh sách quái
                    Entity::entities.push_back(round); // Thêm boss vào danh sách thực thể

                    // Thiết lập màu sắc thanh máu boss
                    SDL_Color barColor = { 0, 255, 255, 255 }; // Màu mặc định
                    if (i == 1) barColor = { 128, 0, 128, 255 }; // Màu tím
                    else if (i == 2) barColor = { 255, 165, 0, 255 }; // Màu cam
                    else if (i == 3) barColor = { 0, 128, 128, 255 }; // Màu xanh lam

                    // Đặt vị trí thanh máu boss
                    int barX = Globals::ScreenWidth / 2 - (bossCount * 110) + (i * 220);
                    int barY = Globals::ScreenHeight - 50;

                    HPBar* bossHpBar = new HPBar(barX, barY, 200, 30, round, barColor); // Tạo thanh máu boss
                    bossHpBar->currentHp = round->hp; // Thiết lập máu hiện tại
                    bossHealthBars.push_back(bossHpBar); // Thêm thanh máu vào danh sách
                }

                // Tạo thêm quái Glob và Grob cho vòng boss
                int additionalGlobs = numGlobs / 2; // Số quái Glob bổ sung
                int additionalGrobs = numGrobs / 2; // Số quái Grob bổ sung
                for (int i = 0; i < additionalGlobs; i++)
                {
                    // Tạo quái Glob ngẫu nhiên
                    int x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    int y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;

                    Glob* enemy = new Glob(globAnimSet); // Tạo quái Glob mới
                    enemy->x = x; // Đặt vị trí x
                    enemy->y = y; // Đặt vị trí y
                    enemy->invincibleTimer = 0.1; // Thiết lập thời gian bất tử
                    enemy->speed *= 1.0 + (currentRound - 1) * 0.2; // Tăng tốc độ theo vòng
                    enemy->damage = 5 * currentRound; // Tăng sát thương theo vòng
                    enemy->hp = 1; // Thiết lập máu

                    enemies.push_back(enemy); // Thêm quái vào danh sách
                    Entity::entities.push_back(enemy); // Thêm quái vào danh sách thực thể
                }

                for (int i = 0; i < additionalGrobs; i++)
                {
                    // Tạo quái Grob (chưa được triển khai đầy đủ trong code gốc)
                    int x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    int y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
                }

                bossActive = true; // Kích hoạt trạng thái boss
                buildBossNext = false; // Tắt trạng thái chờ tạo boss
            }

            // Xóa thanh máu boss khi boss bị tiêu diệt
            if (bossActive && enemies.size() == 0)
            {
                bossHealthBars.clear(); // Xóa danh sách thanh máu
                bossActive = false; // Tắt trạng thái boss
            }
        }

        camController.update(); // Cập nhật vị trí camera
        draw(); // Vẽ toàn bộ khung hình
    }
}

// Hàm vẽ khung hình
void Game::draw()
{
    // Thiết lập màu nền
    SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Globals::renderer); // Xóa màn hình

    // Vẽ màn hình chiến thắng
    if (gameWon)
    {
        renderTexture(victoryImage, Globals::renderer, 0, 0); // Hiển thị ảnh chiến thắng
    }
    // Vẽ màn hình splash
    else if (splashShowing)
    {
        renderTexture(splashImage, Globals::renderer, 0, 0); // Hiển thị ảnh splash
    }
    // Vẽ màn chơi chính
    else
    {
        // Vẽ ảnh nền với offset camera
        renderTexture(backgroundImage, Globals::renderer, 0 - Globals::camera.x, 0 - Globals::camera.y);

        // Sắp xếp thực thể theo thứ tự vẽ
        Entity::entities.sort(Entity::EntityCompare);
        for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            (*entity)->draw(); // Vẽ từng thực thể
        }

        // Vẽ các thanh máu của boss
        for (auto& hpBar : bossHealthBars)
        {
            if (hpBar->entity && hpBar->entity->active)
                hpBar->draw(); // Vẽ thanh máu nếu boss còn hoạt động
        }

        // Vẽ thanh máu của nhân vật chính
        playerHpBar.draw();

        // Vẽ tên vòng chơi
        SDL_Color color = { 255, 255, 255, 255 }; // Màu trắng
        string resPath = getResourcePath();
        SDL_Texture* levelNameTexture = renderText(levelName, resPath + "font.ttf", color, 30, Globals::renderer); // Tạo texture tên vòng
        renderTexture(levelNameTexture, Globals::renderer, 20, 20); // Vẽ tên vòng
        cleanup(levelNameTexture); // Giải phóng texture

        // Vẽ overlay khi nhân vật chết
        if (overlayTimer <= 0 && hero->hp <= 0)
        {
            renderTexture(overlayImage, Globals::renderer, 0, 0); // Vẽ ảnh overlay
            if (scoreTexture == NULL)
            {
                SDL_Color color = { 255, 255, 255, 255 }; // Màu trắng
                stringstream ss;
                ss << "so quai vat da tieu diet: " << Glob::globsKilled + Boss::bossesKilled; // Tạo chuỗi điểm số
                string resPath = getResourcePath();
                scoreTexture = renderText(ss.str(), resPath + "font.ttf", color, 30, Globals::renderer); // Tạo texture điểm số
            }
            renderTexture(scoreTexture, Globals::renderer, 20, 50); // Vẽ điểm số
        }

        // Vẽ thông báo tạm dừng
        if (isPaused)
        {
            if (pauseTexture == nullptr)
            {
                SDL_Color color = { 255, 255, 255, 255 }; // Màu trắng
                string resPath = getResourcePath();
                pauseTexture = renderText("Game Paused", resPath + "font.ttf", color, 50, Globals::renderer); // Tạo texture tạm dừng
            }
            int x = (Globals::ScreenWidth - 200) / 2; // Tọa độ x trung tâm
            int y = (Globals::ScreenHeight - 50) / 2; // Tọa độ y trung tâm
            renderTexture(pauseTexture, Globals::renderer, x, y); // Vẽ thông báo tạm dừng
        }

        // Vẽ thông báo tắt âm
        if (isMuted)
        {
            if (muteTexture == nullptr)
            {
                SDL_Color color = { 255, 255, 255, 255 }; // Màu trắng
                string resPath = getResourcePath();
                muteTexture = renderText("Muted", resPath + "font.ttf", color, 30, Globals::renderer); // Tạo texture tắt âm
            }
            int x = 20; // Tọa độ x góc dưới trái
            int y = Globals::ScreenHeight - 50; // Tọa độ y góc dưới trái
            renderTexture(muteTexture, Globals::renderer, x, y); // Vẽ thông báo tắt âm
        }
    }
    SDL_RenderPresent(Globals::renderer); // Hiển thị khung hình
}

// Hàm reset game
void Game::resetGame()
{
    // Dọn dẹp các thanh máu của boss
    for (auto bar : bossHealthBars) {
        delete bar; // Xóa từng thanh máu
    }
    bossHealthBars.clear(); // Xóa danh sách thanh máu

    // Xóa tất cả thực thể không phải nhân vật chính hoặc tường
    for (auto it = Entity::entities.begin(); it != Entity::entities.end(); )
    {
        Entity* entity = *it;
        if (entity != hero && !dynamic_cast<Wall*>(entity)) // Giữ lại nhân vật và tường
        {
            it = Entity::entities.erase(it); // Xóa thực thể
        }
        else
        {
            ++it; // Chuyển sang thực thể tiếp theo
        }
    }

    // Xóa và giải phóng bộ nhớ tất cả quái
    Entity::removeAllFromList(&enemies, true);
    enemies.clear(); // Xóa danh sách quái

    // Reset nhân vật chính
    hero->hp = hero->hpMax; // Khôi phục máu tối đa
    hero->revive(); // Hồi sinh nhân vật

    // Đặt lại vị trí nhân vật chính ở trung tâm màn hình
    hero->x = Globals::ScreenWidth / 2;
    hero->y = Globals::ScreenHeight / 2;

    // Đặt lại các thuộc tính di chuyển và va chạm
    hero->moveSpeed = 0; // Đặt lại tốc độ di chuyển
    hero->slideAmount = 0; // Đặt lại độ trượt
    hero->totalXMove = 0; // Đặt lại tổng di chuyển x
    hero->totalYMove = 0; // Đặt lại tổng di chuyển y
    hero->lastCollisionBox = { 0, 0, 0, 0 }; // Đặt lại hộp va chạm trước đó
    hero->updateCollisionBox(); // Cập nhật hộp va chạm hiện tại

    // Kiểm tra va chạm với tường để đảm bảo nhân vật không nằm trong tường
    for (auto wall : walls) {
        if (Entity::checkCollision(hero->collisionBox, wall->collisionBox)) {
            // Điều chỉnh vị trí nhân vật nếu nằm trong tường
            hero->x += (hero->collisionBox.x < wall->collisionBox.x) ?
                -(hero->collisionBox.w / 2 + 1) : (hero->collisionBox.w / 2 + 1);
            hero->y += (hero->collisionBox.y < wall->collisionBox.y) ?
                -(hero->collisionBox.h / 2 + 1) : (hero->collisionBox.h / 2 + 1);
            hero->updateCollisionBox(); // Cập nhật lại hộp va chạm
        }
    }

    // Reset giao diện người dùng
    playerHpBar.currentHp = hero->hpMax; // Khôi phục máu thanh máu
    playerHpBar.entity = hero; // Gán lại thực thể cho thanh máu
    overlayTimer = 2; // Đặt lại thời gian overlay

    // Dọn dẹp các texture còn sót lại
    if (scoreTexture != NULL) { cleanup(scoreTexture); scoreTexture = NULL; } // Giải phóng texture điểm số
    if (pauseTexture != nullptr) { cleanup(pauseTexture); pauseTexture = nullptr; } // Giải phóng texture tạm dừng
    if (muteTexture != nullptr) { cleanup(muteTexture); muteTexture = nullptr; } // Giải phóng texture tắt âm

    // Reset các thông số vòng chơi
    currentRound = 1; // Đặt lại về vòng 1
    bossActive = false; // Tắt trạng thái boss
    buildBossNext = false; // Tắt trạng thái chờ tạo boss
    gameWon = false; // Đặt lại trạng thái chưa chiến thắng
    enemiesBuilt = 0; // Đặt lại số quái đã tạo
    enemiesToBuild = 0; // Đặt lại số quái cần tạo
    enemyBuildTimer = 3; // Đặt lại thời gian tạo quái

    // Thiết lập số lượng quái cho vòng 1
    setEnemyCounts(currentRound);

    // Thiết lập vòng chơi đầu tiên
    setupNextRound();

    // Đảm bảo nhân vật chính có trong danh sách thực thể
    if (std::find(Entity::entities.begin(), Entity::entities.end(), hero) == Entity::entities.end()) {
        Entity::entities.push_back(hero); // Thêm lại nhân vật chính
    }

    // Cập nhật một chu kỳ cho nhân vật chính để đảm bảo va chạm được xử lý đúng
    hero->update();
}

// Hàm thiết lập số lượng quái và boss cho mỗi vòng
void Game::setEnemyCounts(int round)
{
    switch (round)
    {
    case 1:
        numGlobs = 20; // Số quái Glob
        bossCount = 0; // Không có boss
        break;
    case 2:
        numGlobs = 24; // Số quái Glob
        bossCount = 0; // Không có boss
        break;
    case 3:
        numGlobs = 20; // Số quái Glob
        bossCount = 1; // 1 boss
        break;
    case 4:
        numGlobs = 30; // Số quái Glob
        bossCount = 0; // Không có boss
        break;
    case 5:
        numGlobs = 34; // Số quái Glob
        bossCount = 0; // Không có boss
        break;
    case 6:
        numGlobs = 30; // Số quái Glob
        bossCount = 2; // 2 boss
        break;
    case 7:
        numGlobs = 40; // Số quái Glob
        bossCount = 0; // Không có boss
        break;
    case 8:
        numGlobs = 44; // Số quái Glob
        bossCount = 0; // Không có boss
        break;
    case 9:
        numGlobs = 40; // Số quái Glob
        bossCount = 3; // 3 boss
        break;
    case 10:
        numGlobs = 50; // Số quái Glob
        bossCount = 4; // 4 boss
        break;
    default:
        numGlobs = 0; // Không có quái
        bossCount = 0; // Không có boss
        break;
    }
}

// Hàm thiết lập vòng chơi tiếp theo
void Game::setupNextRound()
{
    setEnemyCounts(currentRound); // Thiết lập số lượng quái và boss
    enemiesBuilt = 0; // Đặt lại số quái đã tạo
    enemyBuildTimer = 4; // Đặt lại thời gian tạo quái
    enemiesToBuild = numGlobs; // Thiết lập số quái cần tạo

    // Kiểm tra nếu vòng có boss
    if (bossCount > 0)
    {
        buildBossNext = true; // Chuẩn bị tạo boss
    }
    else
    {
        buildBossNext = false; // Không tạo boss
    }

    // Thiết lập tên vòng chơi
    if (currentRound == 10)
    {
        levelName = "Round 10 (Final)"; // Tên vòng cuối
    }
    else
    {
        levelName = "Round " + std::to_string(currentRound); // Tên vòng thường
    }
}