#include "game.h"

Game::Game()
{
    // Khởi tạo tài nguyên
    string resPath = getResourcePath();
    backgroundImage = loadTexture(resPath + "map.png", Globals::renderer); // Ảnh nền
    splashImage = loadTexture(resPath + "cyborgtitle.png", Globals::renderer); // Ảnh splash
    overlayImage = loadTexture(resPath + "overlay.png", Globals::renderer); // Ảnh overlay
    victoryImage = loadTexture(resPath + "victory.png", Globals::renderer); // Ảnh chiến thắng

    // Trạng thái ban đầu
    splashShowing = true; // Hiển thị màn hình splash
    overlayTimer = 2; // Bộ đếm thời gian overlay
    gameWon = false; // Trạng thái chiến thắng ban đầu

    // Cài đặt camera
    Globals::camera.x = 0;
    Globals::camera.y = 0;
    Globals::camera.w = Globals::ScreenWidth;
    Globals::camera.h = Globals::ScreenHeight;

    // Cài đặt âm thanh
    SoundManager::soundManager.loadSound("hit", resPath + "Randomize2.wav"); // Âm thanh khi bị đánh
    SoundManager::soundManager.loadSound("enemyHit", resPath + "Hit_Hurt9.wav"); // Âm thanh khi đánh boss
    SoundManager::soundManager.loadSound("swing", resPath + "Randomize21.wav"); // Âm thanh khi tấn công
    SoundManager::soundManager.loadSound("dash", resPath + "dash.wav"); // Âm thanh khi lướt
    SoundManager::soundManager.loadSound("enemyDie", resPath + "Randomize41.wav"); // Âm thanh khi quái chết

    song = Mix_LoadMUS(string(resPath + "musictheme.wav").c_str()); // Nhạc nền
    if (song != NULL)
    {
        Mix_PlayMusic(song, -1); // Phát nhạc nền lặp lại
    }

    // Cài đặt các loại dữ liệu nhóm
    list<DataGroupType> dataGroupTypes;
    DataGroupType colBoxType = { "collisionBox", DataGroupType::DATATYPE_BOX }; // Hộp va chạm
    DataGroupType hitBoxType = { "hitBox", DataGroupType::DATATYPE_BOX }; // Hộp đánh
    DataGroupType dmgType = { "damage", DataGroupType::DATATYPE_NUMBER }; // Sát thương
    dataGroupTypes.push_back(colBoxType);
    dataGroupTypes.push_back(hitBoxType);
    dataGroupTypes.push_back(dmgType);

    // Cài đặt giao diện người dùng
    playerHpBar = HPBar(Globals::ScreenWidth - 210, 10, 200, 30, nullptr, { 0, 255, 0, 255 }); // Thanh máu người chơi

    // Cài đặt animation
    heroAnimSet = new AnimationSet();
    heroAnimSet->loadAnimationSet("udemyCyborg.fdset", dataGroupTypes, true, 0, true);

    globAnimSet = new AnimationSet();
    globAnimSet->loadAnimationSet("glob.fdset", dataGroupTypes, true, 0, true);

    wallAnimSet = new AnimationSet();
    wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

    // Cài đặt nhân vật chính
    hero = new Hero(heroAnimSet);
    hero->invincibleTimer = 0; // Thời gian bất tử ban đầu
    hero->x = Globals::ScreenWidth / 2; // Vị trí ban đầu của nhân vật
    hero->y = Globals::ScreenHeight / 2;
    heroInput.hero = hero; // Gán đầu vào bàn phím cho nhân vật
    Entity::entities.push_back(hero);
    camController.target = hero; // Gán camera theo dõi nhân vật

    // Gắn thanh máu cho nhân vật
    playerHpBar.entity = hero;
    playerHpBar.currentHp = hero->hpMax;

    // Tạo tường bao quanh
    int tileSize = 32; // Kích thước mỗi ô tường
    for (int i = 0; i < Globals::ScreenWidth / tileSize; i++)
    {
        Wall* newWall = new Wall(wallAnimSet);
        newWall->x = i * tileSize + tileSize / 2;
        newWall->y = tileSize / 2; // Tường trên
        walls.push_back(newWall);
        Entity::entities.push_back(newWall);

        newWall = new Wall(wallAnimSet);
        newWall->x = i * tileSize + tileSize / 2;
        newWall->y = Globals::ScreenHeight - tileSize / 2; // Tường dưới
        walls.push_back(newWall);
        Entity::entities.push_back(newWall);
    }
    for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++)
    {
        Wall* newWall = new Wall(wallAnimSet);
        newWall->x = tileSize / 2;
        newWall->y = i * tileSize + tileSize / 2; // Tường trái
        walls.push_back(newWall);
        Entity::entities.push_back(newWall);

        newWall = new Wall(wallAnimSet);
        newWall->x = Globals::ScreenWidth - tileSize / 2;
        newWall->y = i * tileSize + tileSize / 2; // Tường phải
        walls.push_back(newWall);
        Entity::entities.push_back(newWall);
    }

    // Khởi tạo các animation liên quan đến boss
    bossAnimSet = new AnimationSet();
    bossAnimSet->loadAnimationSet("boss.fdset", dataGroupTypes, true, 0, true);

    bulletAnimSet = new AnimationSet();
    bulletAnimSet->loadAnimationSet("bullet.fdset", dataGroupTypes, true, 0, true);
}

// Dọn dẹp tài nguyên
Game::~Game()
{
    // Dọn dẹp texture
    cleanup(backgroundImage);
    cleanup(splashImage);
    cleanup(overlayImage);
    cleanup(victoryImage); // Dọn dẹp texture chiến thắng
    if (scoreTexture != NULL)
    {
        cleanup(scoreTexture);
    }
    if (pauseTexture != nullptr) {
        cleanup(pauseTexture);
    }
    if (muteTexture != nullptr) {
        cleanup(muteTexture);
    }

    // Dọn dẹp boss health bars
    for (auto bar : bossHealthBars) {
        delete bar;
    }
    bossHealthBars.clear();

    // Dọn dẹp âm thanh
    Mix_PausedMusic();
    Mix_FreeMusic(song);

    // Dọn dẹp thực thể
    Entity::removeAllFromList(&Entity::entities, false);
    Entity::removeAllFromList(reinterpret_cast<std::list<Entity*>*>(&walls), true);
    Entity::removeAllFromList(&enemies, true);

    // Dọn dẹp animation
    delete heroAnimSet;
    delete globAnimSet;
    delete wallAnimSet;
    delete bossAnimSet;
    delete bulletAnimSet;

    // Dọn dẹp nhân vật chính
    delete hero;
}

// Main Game Loop
void Game::update()
{
    bool quit = false;

    SDL_Event e;
    TimeController::timeController.reset();
    while (!quit)
    {
        TimeController::timeController.updateTime();

        Entity::removeInactiveEntitiesFromList(&Entity::entities, false);
        Entity::removeInactiveEntitiesFromList(&enemies, true);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    quit = true;
                    break;
                case SDL_SCANCODE_SPACE:
                    if (splashShowing)
                        splashShowing = false;
                    else if (overlayTimer <= 0 && hero->hp <= 0)
                    {
                        resetGame();
                    }
                    break;
                case SDL_SCANCODE_P:
                    isPaused = !isPaused;
                    break;
                case SDL_SCANCODE_O:
                    isMuted = !isMuted;
                    if (isMuted)
                    {
                        Mix_VolumeMusic(0);
                        SoundManager::soundManager.muteAllSounds();
                    }
                    else
                    {
                        Mix_VolumeMusic(MIX_MAX_VOLUME);
                        SoundManager::soundManager.unmuteAllSounds();
                    }
                    break;
                }
            }
            heroInput.update(&e);
        }

        if (isPaused)
        {
            draw();
            continue;
        }

        if (hero->hp <= 0 && overlayTimer > 0)
        {
            overlayTimer -= TimeController::timeController.dT;
        }

        for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            (*entity)->update();
        }

        if (hero->hp > 0 && !splashShowing && !gameWon) // Không cập nhật nếu đã thắng
        {
            if (enemiesToBuild == enemiesBuilt && enemies.size() == 0 && !bossActive)
            {
                if (currentRound == 10)
                {
                    gameWon = true; // Đặt trạng thái chiến thắng
                }
                else
                {
                    currentRound++;
                    setupNextRound();
                }
            }

            enemyBuildTimer -= TimeController::timeController.dT;

            if (!buildBossNext && !bossActive && enemiesBuilt < enemiesToBuild && enemyBuildTimer <= 0)
            {
                if (enemiesBuilt < numGlobs)
                {
                    int x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    int y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;

                    Glob* enemy = new Glob(globAnimSet);
                    enemy->x = x;
                    enemy->y = y;
                    enemy->invincibleTimer = 0.1;
                    enemy->speed *= 1.0 + (currentRound - 1) * 0.2;
                    enemy->damage = 5 * currentRound;
                    enemy->hp = 1;

                    enemies.push_back(enemy);
                    Entity::entities.push_back(enemy);
                    enemiesBuilt++;
                    enemyBuildTimer = 0.5;
                }
                else if (enemiesBuilt < numGlobs + numGrobs)
                {
                    int x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    int y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
                }
            }

            if (buildBossNext && enemies.size() == 0 && !bossActive)
            {
                bossHealthBars.clear();

                // Spawn boss
                for (int i = 0; i < bossCount; i++)
                {
                    Boss* round = new Boss(bossAnimSet, bulletAnimSet);
                    round->invincibleTimer = 0.1;
                    round->damage = 10 * currentRound;
                    round->hp = 100 + currentRound * 20;
                    round->hpMax = round->hp;

                    round->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    round->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;

                    enemies.push_back(round);
                    Entity::entities.push_back(round);

                    SDL_Color barColor = { 0, 255, 255, 255 };
                    if (i == 1) barColor = { 128, 0, 128, 255 };
                    else if (i == 2) barColor = { 255, 165, 0, 255 };
                    else if (i == 3) barColor = { 0, 128, 128, 255 };

                    int barX = Globals::ScreenWidth / 2 - (bossCount * 110) + (i * 220);
                    int barY = Globals::ScreenHeight - 50;

                    HPBar* bossHpBar = new HPBar(barX, barY, 200, 30, round, barColor);
                    bossHpBar->currentHp = round->hp;
                    bossHealthBars.push_back(bossHpBar);
                }

                // Spawn additional Glob and Grob for boss rounds
                int additionalGlobs = numGlobs / 2;
                int additionalGrobs = numGrobs / 2;
                for (int i = 0; i < additionalGlobs; i++)
                {
                    int x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    int y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;

                    Glob* enemy = new Glob(globAnimSet);
                    enemy->x = x;
                    enemy->y = y;
                    enemy->invincibleTimer = 0.1;
                    enemy->speed *= 1.0 + (currentRound - 1) * 0.2;
                    enemy->damage = 5 * currentRound;
                    enemy->hp = 1;

                    enemies.push_back(enemy);
                    Entity::entities.push_back(enemy);
                }

                for (int i = 0; i < additionalGrobs; i++)
                {
                    int x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    int y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
                }

                bossActive = true;
                buildBossNext = false;
            }

            // Clear boss health bars when boss is defeated
            if (bossActive && enemies.size() == 0)
            {
                bossHealthBars.clear();
                bossActive = false;
            }
        }

        camController.update();
        draw();
    }
}

// Draw function
void Game::draw()
{
    SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Globals::renderer);

    if (gameWon)
    {
        renderTexture(victoryImage, Globals::renderer, 0, 0); // Hiển thị màn hình chiến thắng
    }
    else if (splashShowing)
    {
        renderTexture(splashImage, Globals::renderer, 0, 0);
    }
    else
    {
        renderTexture(backgroundImage, Globals::renderer, 0 - Globals::camera.x, 0 - Globals::camera.y);

        Entity::entities.sort(Entity::EntityCompare);
        for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            (*entity)->draw();
        }

        for (auto& hpBar : bossHealthBars)
        {
            if (hpBar->entity && hpBar->entity->active)
                hpBar->draw();
        }

        playerHpBar.draw();

        SDL_Color color = { 255, 255, 255, 255 };
        string resPath = getResourcePath();
        SDL_Texture* levelNameTexture = renderText(levelName, resPath + "font.ttf", color, 30, Globals::renderer);
        renderTexture(levelNameTexture, Globals::renderer, 20, 20);
        cleanup(levelNameTexture);

        if (overlayTimer <= 0 && hero->hp <= 0)
        {
            renderTexture(overlayImage, Globals::renderer, 0, 0);
            if (scoreTexture == NULL)
            {
                SDL_Color color = { 255, 255, 255, 255 };
                stringstream ss;
                ss << "so quai vat da tieu diet: " << Glob::globsKilled + Boss::bossesKilled;                
                string resPath = getResourcePath();
                scoreTexture = renderText(ss.str(), resPath + "font.ttf", color, 30, Globals::renderer);
            }
            renderTexture(scoreTexture, Globals::renderer, 20, 50);
        }

        if (isPaused)
        {
            if (pauseTexture == nullptr)
            {
                SDL_Color color = { 255, 255, 255, 255 };
                string resPath = getResourcePath();
                pauseTexture = renderText("Game Paused", resPath + "font.ttf", color, 50, Globals::renderer);
            }
            int x = (Globals::ScreenWidth - 200) / 2;
            int y = (Globals::ScreenHeight - 50) / 2;
            renderTexture(pauseTexture, Globals::renderer, x, y);
        }

        if (isMuted)
        {
            if (muteTexture == nullptr)
            {
                SDL_Color color = { 255, 255, 255, 255 };
                string resPath = getResourcePath();
                muteTexture = renderText("Muted", resPath + "font.ttf", color, 30, Globals::renderer);
            }
            int x = 20;
            int y = Globals::ScreenHeight - 50;
            renderTexture(muteTexture, Globals::renderer, x, y);
        }
    }
    SDL_RenderPresent(Globals::renderer);
}

void Game::resetGame()
{
    // Clean up boss bars
    for (auto bar : bossHealthBars) {
        delete bar;
    }
    bossHealthBars.clear();

    // Remove all non-hero, non-wall from the global list WITHOUT deleting
    for (auto it = Entity::entities.begin(); it != Entity::entities.end(); )
    {
        Entity* entity = *it;
        if (entity != hero && !dynamic_cast<Wall*>(entity)) // Giữ lại hero và tường
        {
            it = Entity::entities.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Now safely remove and delete all from enemies
    Entity::removeAllFromList(&enemies, true);
    enemies.clear();

    // Reset hero
    hero->hp = hero->hpMax;
    hero->revive();

    // Đặt lại vị trí hero ở trung tâm màn hình, đảm bảo không chồng lấn với tường
    hero->x = Globals::ScreenWidth / 2;
    hero->y = Globals::ScreenHeight / 2;

    // Đặt lại các thuộc tính di chuyển và va chạm
    hero->moveSpeed = 0;
    hero->slideAmount = 0;
    hero->totalXMove = 0;
    hero->totalYMove = 0;
    hero->lastCollisionBox = { 0, 0, 0, 0 }; // Reset hộp va chạm trước đó
    hero->updateCollisionBox(); // Cập nhật hộp va chạm hiện tại

    // Đảm bảo hero không nằm trong tường bằng cách kiểm tra va chạm ngay lập tức
    for (auto wall : walls) {
        if (Entity::checkCollision(hero->collisionBox, wall->collisionBox)) {
            // Nếu hero nằm trong tường, điều chỉnh vị trí hero
            hero->x += (hero->collisionBox.x < wall->collisionBox.x) ?
                -(hero->collisionBox.w / 2 + 1) : (hero->collisionBox.w / 2 + 1);
            hero->y += (hero->collisionBox.y < wall->collisionBox.y) ?
                -(hero->collisionBox.h / 2 + 1) : (hero->collisionBox.h / 2 + 1);
            hero->updateCollisionBox(); // Cập nhật lại hộp va chạm sau khi điều chỉnh
        }
    }

    // Reset UI
    playerHpBar.currentHp = hero->hpMax;
    playerHpBar.entity = hero;
    overlayTimer = 2;

    // Clean leftover textures
    if (scoreTexture != NULL) { cleanup(scoreTexture); scoreTexture = NULL; }
    if (pauseTexture != nullptr) { cleanup(pauseTexture); pauseTexture = nullptr; }
    if (muteTexture != nullptr) { cleanup(muteTexture); muteTexture = nullptr; }

    // Round restarts
    currentRound = 1;
    bossActive = false;
    buildBossNext = false;
    gameWon = false;
    enemiesBuilt = 0;
    enemiesToBuild = 0;
    enemyBuildTimer = 3;

    // Reset enemy count for round 1
    setEnemyCounts(currentRound);

    // Setup first round
    setupNextRound();

    // Đảm bảo hero được thêm lại vào danh sách entities nếu bị xóa nhầm
    if (std::find(Entity::entities.begin(), Entity::entities.end(), hero) == Entity::entities.end()) {
        Entity::entities.push_back(hero);
    }

    // Process one update cycle for hero to ensure collision is properly registered
    hero->update();
}
void Game::setEnemyCounts(int round)
{
    switch (round)
    {
    case 1:
        numGlobs = 20;
        bossCount = 0;
        break;
    case 2:
        numGlobs = 24; 
        bossCount = 0;
        break;
    case 3:
        numGlobs = 20;
        bossCount = 1;
        break;
    case 4:
        numGlobs = 30;
        bossCount = 0;
        break;
    case 5:
        numGlobs = 34; 
        bossCount = 0;
        break;
    case 6:
        numGlobs = 30; 
        bossCount = 2;
        break;
    case 7:
        numGlobs = 40;
        bossCount = 0;
        break;
    case 8:
        numGlobs = 44;
        bossCount = 0;
        break;
    case 9:
        numGlobs = 40;
        bossCount = 3;
        break;
    case 10:
        numGlobs = 50;
        bossCount = 4;
        break;
    default:
        numGlobs = 0;
        bossCount = 0;
        break;

    }
}

void Game::setupNextRound()
{
    setEnemyCounts(currentRound);
    enemiesBuilt = 0;
    enemyBuildTimer = 4;
    enemiesToBuild = numGlobs;

    if (bossCount > 0)
    {
        buildBossNext = true;
    }
    else
    {
        buildBossNext = false;
    }

    if (currentRound == 10)
    {
        levelName = "Round 10 (Final)";
    }
    else
    {
        levelName = "Round " + std::to_string(currentRound);
    }
}