#include "game.h"

// Hàm khởi tạo trò chơi
Game::Game()
{
	string resPath = getResourcePath();                     // Lấy đường dẫn tới thư mục tài nguyên
	backgroundImage = loadTexture(resPath + "map.png", Globals::renderer); // Tải hình nền bản đồ
	splashImage = loadTexture(resPath + "cyborgtitle.png", Globals::renderer); // tải texture cho màn hình chào
	overlayImage = loadTexture(resPath + "overlay.png", Globals::renderer); // tải texture cho lớp phủ (overlay)
	
	splashShowing = true;
	overlayTimer = 2;


	// Danh sách các loại nhóm dữ liệu mà frame có thể có
	list<DataGroupType> dataGroupTypes;

	// Định nghĩa các loại dữ liệu cho frame
	// Hộp va chạm (collision box)
	DataGroupType colBoxType;
	{
		colBoxType.groupName = "collisionBox";              // Tên nhóm: collisionBox
		colBoxType.dataType = DataGroupType::DATATYPE_BOX;  // Loại dữ liệu: hộp
	}

	// Hộp đánh (hitbox)
	DataGroupType hitBoxType;
	{
		hitBoxType.groupName = "hitBox";                    // Tên nhóm: hitBox
		hitBoxType.dataType = DataGroupType::DATATYPE_BOX;  // Loại dữ liệu: hộp
	}

	// Sát thương (damage)
	DataGroupType dmgType;
	{
		dmgType.groupName = "damage";                       // Tên nhóm: damage
		dmgType.dataType = DataGroupType::DATATYPE_NUMBER;  // Loại dữ liệu: số
	}

	// Thêm các loại dữ liệu vào danh sách
	dataGroupTypes.push_back(colBoxType);
	dataGroupTypes.push_back(hitBoxType);
	dataGroupTypes.push_back(dmgType);

	// Tạo và tải tập hợp animation cho nhân vật chính (hero)
	heroAnimSet = new AnimationSet();
	heroAnimSet->loadAnimationSet("udemyCyborg.fdset", dataGroupTypes, true, 0, true);

	// Tạo và tải tập hợp animation cho kẻ địch (glob)
	globAnimSet = new AnimationSet();
	globAnimSet->loadAnimationSet("glob.fdset", dataGroupTypes, true, 0, true);

	// Tạo và tải tập hợp animation cho tường (wall)
	wallAnimSet = new AnimationSet();
	wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

	// Tạo nhân vật chính
	hero = new Hero(heroAnimSet);
	{
		hero->invincibleTimer = 0;                          // Đặt thời gian bất tử ban đầu là 0
		hero->x = Globals::ScreenWidth / 2;                 // Đặt vị trí x ở giữa màn hình
		hero->y = Globals::ScreenHeight / 2;                // Đặt vị trí y ở giữa màn hình
		heroInput.hero = hero;                              // Gán hero cho bộ điều khiển bàn phím
		Entity::entities.push_back(hero);                   // Thêm hero vào danh sách thực thể
	}

	int tileSize = 32;                                      // Kích thước ô vuông (tile)

	// Tạo các bức tường cho trò chơi
	// Tạo tường ở trên và dưới màn hình
	for (int i = 0; i < Globals::ScreenWidth / tileSize; i++)
	{
		// Tạo tường ở hàng trên cùng
		Wall* newWall = new Wall(wallAnimSet);
		{
			newWall->x = i * tileSize + tileSize / 2;       // Đặt vị trí x của tường
			newWall->y = tileSize / 2;                      // Đặt vị trí y ở trên cùng
			walls.push_back(newWall);                       // Thêm vào danh sách tường
			Entity::entities.push_back(newWall);            // Thêm vào danh sách thực thể
		}

		// Tái sử dụng con trỏ để tạo tường ở hàng dưới cùng
		newWall = new Wall(wallAnimSet);
		{
			newWall->x = i * tileSize + tileSize / 2;       // Đặt vị trí x của tường
			newWall->y = Globals::ScreenHeight - tileSize / 2; // Đặt vị trí y ở dưới cùng
			walls.push_back(newWall);                       // Thêm vào danh sách tường
			Entity::entities.push_back(newWall);            // Thêm vào danh sách thực thể
		}
	}

	// Tạo tường ở hai bên màn hình
	for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++)
	{
		// Tạo tường ở cột bên trái
		Wall* newWall = new Wall(wallAnimSet);
		{
			newWall->x = tileSize / 2;                      // Đặt vị trí x ở bên trái
			newWall->y = i * tileSize + tileSize / 2;       // Đặt vị trí y của tường
			walls.push_back(newWall);                       // Thêm vào danh sách tường
			Entity::entities.push_back(newWall);            // Thêm vào danh sách thực thể
		}

		// Tái sử dụng con trỏ để tạo tường ở cột bên phải
		newWall = new Wall(wallAnimSet);
		{
			newWall->x = Globals::ScreenWidth - tileSize / 2; // Đặt vị trí x ở bên phải
			newWall->y = i * tileSize + tileSize / 2;       // Đặt vị trí y của tường
			walls.push_back(newWall);                       // Thêm vào danh sách tường
			Entity::entities.push_back(newWall);            // Thêm vào danh sách thực thể
		}
	}
}

// Hàm hủy trò chơi
Game::~Game()
{
	cleanup(backgroundImage);                               // Dọn dẹp hình nền
	cleanup(splashImage); // giải phóng bộ nhớ của màn hình chào  
	cleanup(overlayImage); // giải phóng bộ nhớ của lớp phủ (overlay)  

	if (scoreTexture != NULL) // kiểm tra nếu scoreTexture tồn tại thì mới giải phóng  
		cleanup(scoreTexture);

	Entity::removeAllFromList(&Entity::entities, false);    // Xóa tất cả thực thể khỏi danh sách (không xóa bộ nhớ)

	delete heroAnimSet;                                     // Giải phóng tập hợp animation của hero
	delete globAnimSet;                                     // Giải phóng tập hợp animation của glob
	delete wallAnimSet;                                     // Giải phóng tập hợp animation của tường

	delete hero;                                            // Giải phóng hero

	// Xóa và giải phóng bộ nhớ của tất cả tường và kẻ địch
	Entity::removeAllFromList(&walls, true);
	Entity::removeAllFromList(&enemies, true);
}

// Hàm cập nhật trạng thái trò chơi
void Game::update()
{
	// Các biến liên quan đến kẻ địch
	int enemiesToBuild = 2;                                 // Số lượng kẻ địch cần tạo
	int enemiesBuilt = 0;                                   // Số lượng kẻ địch đã tạo
	float enemyBuildTimer = 1;                              // Thời gian chờ để tạo kẻ địch

	bool quit = false;                                      // Cờ thoát trò chơi

	SDL_Event e;                                            // Biến xử lý sự kiện SDL
	TimeController::timeController.reset();                 // Đặt lại bộ đếm thời gian trước khi bắt đầu

	// vòng lặp chính của trò chơi
	while (!quit)
	{
		TimeController::timeController.updateTime(); // cập nhật thời gian  

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false); // xóa thực thể không hoạt động  
		Entity::removeInactiveEntitiesFromList(&enemies, true); // xóa và giải phóng kẻ địch không hoạt động  

		// xử lý các sự kiện  
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) // nếu người dùng đóng cửa sổ  
			{
				quit = true; // thoát trò chơi  
			}
			if (e.type == SDL_KEYDOWN) // nếu có phím được nhấn  
			{
				switch (e.key.keysym.scancode) // kiểm tra phím nào được nhấn  
				{
				case SDL_SCANCODE_ESCAPE: // phím esc  
					quit = true; // thoát trò chơi  
					break;
				case SDL_SCANCODE_SPACE: // phím space  
					if (splashShowing)
						splashShowing = false;
					if (overlayTimer <= 0 && hero->hp < 1)
					{
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemyBuildTimer = 3;
						overlayTimer = 2;

						Glob::globsKilled = 0;
						if (scoreTexture != NULL)
						{
							cleanup(scoreTexture);
							scoreTexture = NULL;
						}

						for (list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++)
						{
							(*enemy)->active = false; // vô hiệu hóa kẻ địch  
						}
						hero->revive(); // hồi sinh hero  
					}
					break;
				}
			}
			heroInput.update(&e); // cập nhật đầu vào từ bàn phím cho hero  
		}

		// giảm dần bộ đếm thời gian của overlay
		if (hero->hp < 1 && overlayTimer > 0)
		{
			overlayTimer -= TimeController::timeController.dT; // trừ đi thời gian deltaTime  
		}

		// Cập nhật tất cả thực thể
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
		{
			(*entity)->update();                            // Cập nhật từng thực thể (sử dụng tính đa hình)
		}

		// Sinh ra kẻ địch nếu hero còn sống
		if (hero->hp > 0 && !splashShowing)
		{
			if (enemiesToBuild == enemiesBuilt)             // Nếu đã tạo đủ kẻ địch
			{
				enemiesToBuild = enemiesToBuild * 2;        // Tăng gấp đôi số lượng cần tạo
				enemiesBuilt = 0;                           // Đặt lại số đã tạo
				enemyBuildTimer = 4;                        // Đặt lại thời gian chờ
			}
			enemyBuildTimer -= TimeController::timeController.dT; // Giảm thời gian chờ
			if (enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10)
			{
				Glob* enemy = new Glob(globAnimSet);        // Tạo kẻ địch mới
				{
					enemy->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16; // Vị trí x ngẫu nhiên
					enemy->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16; // Vị trí y ngẫu nhiên
					enemy->invincibleTimer = 0.1;           // Thời gian bất tử ngắn khi sinh ra
					enemies.push_back(enemy);               // Thêm vào danh sách kẻ địch
					Entity::entities.push_back(enemy);      // Thêm vào danh sách thực thể
				}
			}
		}

		draw();                                             // Vẽ tất cả lên màn hình
	}
}

// Hàm vẽ trò chơi lên màn hình  
void Game::draw()
{
	// Đặt màu nền và xóa màn hình  
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);

	// Kiểm tra xem có hiển thị màn hình splash không  
	if (splashShowing)
	{
		renderTexture(splashImage, Globals::renderer, 0, 0); // Vẽ màn hình splash  
	}
	else
	{
		renderTexture(backgroundImage, Globals::renderer, 0, 0); // Vẽ hình nền  

		// Sắp xếp danh sách thực thể theo trục y để vẽ đúng thứ tự  
		Entity::entities.sort(Entity::EntityCompare);

		// Duyệt qua danh sách thực thể và vẽ từng thực thể  
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
		{
			(*entity)->draw();
		}

		// Nếu hero đã chết và bộ đếm overlay kết thúc, hiển thị overlay  
		if (overlayTimer <= 0 && hero->hp < 1)
		{
			renderTexture(overlayImage, Globals::renderer, 0, 0); // Vẽ overlay khi hero chết  

			// Nếu chưa tạo texture điểm số thì tạo  
			if (scoreTexture == NULL)
			{
				SDL_Color color = { 255, 255, 255, 255 }; // Màu trắng  

				stringstream ss;
				ss << "So quai vat da ha guc: " << Glob::globsKilled; // Tạo chuỗi hiển thị số kẻ địch đã tiêu diệt  

				string resPath = getResourcePath();
				scoreTexture = renderText(ss.str(), resPath + "font.ttf", color, 30, Globals::renderer);
			}

			renderTexture(scoreTexture, Globals::renderer, 20, 50); // Vẽ điểm số  
		}
	}

	SDL_RenderPresent(Globals::renderer); // Cập nhật màn hình  
}