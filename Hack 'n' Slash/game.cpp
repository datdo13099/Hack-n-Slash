#include "game.h"

Game::Game()
{
    string resPath = getResourcePath();
    backgroundImage = loadTexture(resPath + "map.png", Globals::renderer);
    splashImage = loadTexture(resPath + "cyborgtitle.png", Globals::renderer);
    overlayImage = loadTexture(resPath + "overlay.png", Globals::renderer);

    splashShowing = true;
    overlayTimer = 2;

    SoundManager::soundManager.loadSound("hit", resPath + "Randomize2.wav");
    SoundManager::soundManager.loadSound("enemyHit", resPath + "Hit_Hurt9.wav");
    SoundManager::soundManager.loadSound("swing", resPath + "Randomize21.wav");
    SoundManager::soundManager.loadSound("dash", resPath + "dash.wav");
    SoundManager::soundManager.loadSound("growl", resPath + "Randomize34.wav");
    SoundManager::soundManager.loadSound("enemyDie", resPath + "Randomize41.wav");

    song = Mix_LoadMUS(string(resPath + "Fatal Theory.wav").c_str());
    if (song != NULL)
    {
        Mix_PlayMusic(song, -1);
    }

    list<DataGroupType> dataGroupTypes;

    DataGroupType colBoxType;
    {
        colBoxType.groupName = "collisionBox";
        colBoxType.dataType = DataGroupType::DATATYPE_BOX;
    }

    DataGroupType hitBoxType;
    {
        hitBoxType.groupName = "hitBox";
        hitBoxType.dataType = DataGroupType::DATATYPE_BOX;
    }

    DataGroupType dmgType;
    {
        dmgType.groupName = "damage";
        dmgType.dataType = DataGroupType::DATATYPE_NUMBER;
    }

    dataGroupTypes.push_back(colBoxType);
    dataGroupTypes.push_back(hitBoxType);
    dataGroupTypes.push_back(dmgType);

    heroAnimSet = new AnimationSet();
    heroAnimSet->loadAnimationSet("udemyCyborg.fdset", dataGroupTypes, true, 0, true);

    globAnimSet = new AnimationSet();
    globAnimSet->loadAnimationSet("glob.fdset", dataGroupTypes, true, 0, true);

    grobAnimSet = new AnimationSet();
    grobAnimSet->loadAnimationSet("grob.fdset", dataGroupTypes, true, 0, true);

    wallAnimSet = new AnimationSet();
    wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

    hero = new Hero(heroAnimSet);
    {
        hero->invincibleTimer = 0;
        hero->x = Globals::ScreenWidth / 2;
        hero->y = Globals::ScreenHeight / 2;
        heroInput.hero = hero;
        Entity::entities.push_back(hero);
    }

    int tileSize = 32;
    for (int i = 0; i < Globals::ScreenWidth / tileSize; i++)
    {
        Wall* newWall = new Wall(wallAnimSet);
        {
            newWall->x = i * tileSize + tileSize / 2;
            newWall->y = tileSize / 2;
            walls.push_back(newWall);
            Entity::entities.push_back(newWall);
        }

        newWall = new Wall(wallAnimSet);
        {
            newWall->x = i * tileSize + tileSize / 2;
            newWall->y = Globals::ScreenHeight - tileSize / 2;
            walls.push_back(newWall);
            Entity::entities.push_back(newWall);
        }
    }

    for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++)
    {
        Wall* newWall = new Wall(wallAnimSet);
        {
            newWall->x = tileSize / 2;
            newWall->y = i * tileSize + tileSize / 2;
            walls.push_back(newWall);
            Entity::entities.push_back(newWall);
        }

        newWall = new Wall(wallAnimSet);
        {
            newWall->x = Globals::ScreenWidth - tileSize / 2;
            newWall->y = i * tileSize + tileSize / 2;
            walls.push_back(newWall);
            Entity::entities.push_back(newWall);
        }
    }
}

Game::~Game()
{
    cleanup(backgroundImage);
    cleanup(splashImage);
    cleanup(overlayImage);

    Mix_PausedMusic();
    Mix_FreeMusic(song);

    if (scoreTexture != NULL)
    {
        cleanup(scoreTexture);
    }

    Entity::removeAllFromList(&Entity::entities, false);

    delete heroAnimSet;
    delete globAnimSet;
    delete grobAnimSet;
    delete wallAnimSet;

    delete hero;

    Entity::removeAllFromList(&walls, true);
    Entity::removeAllFromList(&enemies, true);
}

void Game::update()
{
    int enemiesToBuild = 2;
    int enemiesBuilt = 0;
    float enemyBuildTimer = 1;

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
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    quit = true;
                    break;
                case SDL_SCANCODE_SPACE:
                    if (splashShowing)
                    {
                        splashShowing = false;
                    }
                    if (overlayTimer <= 0 && hero->hp < 1)
                    {
                        enemiesToBuild = 2;
                        enemiesBuilt = 0;
                        enemyBuildTimer = 3;
                        overlayTimer = 2;

                        Glob::globsKilled = 0;
                        Grob::grobsKilled = 0;
                        if (scoreTexture != NULL)
                        {
                            cleanup(scoreTexture);
                            scoreTexture = NULL;
                        }

                        for (list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++)
                        {
                            (*enemy)->active = false;
                        }
                        hero->revive();
                    }
                    break;
                }
            }
            heroInput.update(&e);
        }

        if (hero->hp < 1 && overlayTimer > 0)
        {
            overlayTimer -= TimeController::timeController.dT;
        }

        for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            (*entity)->update();
        }

        // Sinh kẻ địch nếu hero còn sống và không hiển thị màn hình chào
        if (hero->hp > 0 && !splashShowing)
        {
            if (enemiesToBuild == enemiesBuilt)
            {
                enemiesToBuild = enemiesToBuild * 2;
                enemiesBuilt = 0;
                enemyBuildTimer = 4;
            }
            enemyBuildTimer -= TimeController::timeController.dT;
            if (enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10)
            {
                Glob* enemy = new Glob(globAnimSet);
                {
                    enemy->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                    enemy->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
                    enemy->invincibleTimer = 0.1;
                    enemies.push_back(enemy);
                    Entity::entities.push_back(enemy);
                }

                if (enemiesBuilt % 5 == 0)
                {
                    Grob* grob = new Grob(grobAnimSet);
                    {
                        grob->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
                        grob->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
                        grob->invincibleTimer = 0.01;
                        enemies.push_back(grob);
                        Entity::entities.push_back(grob);
                    }
                }
            }
        }

        draw();
    }
}

void Game::draw()
{
    SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Globals::renderer);

    if (splashShowing)
    {
        renderTexture(splashImage, Globals::renderer, 0, 0);
    }
    else
    {
        renderTexture(backgroundImage, Globals::renderer, 0, 0);

        Entity::entities.sort(Entity::EntityCompare);
        for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            (*entity)->draw();
        }

        if (overlayTimer <= 0 && hero->hp < 1)
        {
            renderTexture(overlayImage, Globals::renderer, 0, 0);
            if (scoreTexture == NULL)
            {
                SDL_Color color = { 255, 255, 255, 255 };
                stringstream ss;
                ss << "Số quái vật tiêu diệt: " << Glob::globsKilled + Grob::grobsKilled;

                string resPath = getResourcePath();
                scoreTexture = renderText(ss.str(), resPath + "font.ttf", color, 30, Globals::renderer);
            }
            renderTexture(scoreTexture, Globals::renderer, 20, 50);
        }
    }

    SDL_RenderPresent(Globals::renderer);
}