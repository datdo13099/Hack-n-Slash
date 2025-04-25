# ⚔️ Hack 'n' Slash 🩸

![Banner](https://camo.githubusercontent.com/950dd97b9a5573ce16d4f987f7cbf1b0786f9cf87f10e1bb566148a3a3840b18/68747470733a2f2f63646e2e616e682e6d6f652f662f5369356544722e706e67)

Một tựa game **Hack 'n' Slash** cực cháy, nơi người chơi nhập vai chiến binh dũng mãnh, lao vào các trận chiến kịch tính với quái vật đông đảo.

---

## 🏆 Tính năng nổi bật
- ⚔️ Chặt chém đã tay với combo mượt mà.
- 👹 Đa dạng quái vật với hành vi và kỹ năng riêng.
- 💀 Boss khủng xuất hiện định kỳ với cơ chế độc đáo.
- 🎨 Đồ hoạ pixel phong cách retro đậm chất game cổ điển.

---

## 📣 Giới thiệu và phá đảo game
- Giới thiệu và phá đảo game: https://bit.ly/hacknslash_game

---

## 🎮 Hướng dẫn chơi
- **Di chuyển**: `W`, `A`, `S`, `D`
- **Tấn công**: `N`
- **Kỹ năng đặc biệt**: `M`
- **Tạm dừng**: `P`
- **Tắt tiếng**: `O`
- **Bắt đầu / Chơi lại**: `Space`
- **Tính năng ẩn**: `Người chơi tự khám phá :)`

---

## 🗺️ Cấu trúc game

**Tổng số màn chơi:** 10  
**Số lượng quái vật và boss trong mỗi màn:**  
- **Màn 1:** 20 Glob.
- **Màn 2:** 24 Glob.
- **Màn 3:** 20 Glob, 1 boss.  
- **Màn 4:** 30 Glob.
- **Màn 5:** 34 Glob.
- **Màn 6:** 30 Glob, 2 boss.  
- **Màn 7:** 40 Glob.
- **Màn 8:** 44 Glob.
- **Màn 9:** 40 Glob, 3 boss.  
- **Màn 10 (Final):** 50 Glob, 4 boss.

**Về Glob:** Đây là đám quái vật cấp thấp, chúng sẽ tự động lao đến gần người chơi, rồi tụ tập nội công để ra đòn, khiến đối phương phải đau đầu.

**Về Boss:** Mỗi boss là một con quái vật mạnh mẽ hơn, với những chiêu thức đặc biệt như nhảy, chém, thậm chí bắn đạn vào người chơi. Đừng coi thường chúng!

**Mục tiêu:** Vượt qua tất cả các màn chơi, đánh bại tất cả quái vật và boss, để giành chiến thắng cuối cùng và trở thành người hùng duy nhất!

---

## 🛠️ Hướng dẫn chạy game
### 1. `Hướng dẫn Cài đặt và Mở Project cho Giảng viên (Visual Studio)`

#### **Yêu cầu hệ thống:**
- **Hệ điều hành**: Windows  
- **IDE**: Visual Studio 2022  
- **Thư viện cần thiết**:  
  - SDL2  
  - SDL2_image  
  - SDL2_mixer  
  - SDL2_ttf  

#### **Tải Project**
- **Link tải project đầy đủ**: [Tải tại đây](https://github.com/datdo13099/Hack-n-Slash/archive/refs/heads/main.zip)

#### **Cài đặt SDL2 và các thư viện liên quan:**
- **Tải thư viện** từ trang chính của SDL: [https://libsdl.org/](https://libsdl.org/)
- **Giải nén và cấu hình**:
  - Giải nén thư viện đã tải và **copy các file `.dll`** vào thư mục `Debug` hoặc `Release` trong project.

#### **Cấu hình Visual Studio**:
- **VC++ Directories**:
  - Vào **Project > Properties**.
  - Trong **VC++ Directories**:
     - **Include Directories**: Trỏ đến thư mục `include` của SDL2.
     - **Library Directories**: Trỏ đến thư mục `lib` của SDL2.

- **Linker > Input**:
  - Trong **Linker > Input > Additional Dependencies**, thêm các thư viện sau:
    ```txt
    SDL2.lib  
    SDL2main.lib  
    SDL2_image.lib  
    SDL2_mixer.lib  
    SDL2_ttf.lib
    ```

> ⚠️ **Lưu ý**: Đảm bảo cấu hình cho `x64` nếu hệ điều hành của bạn là 64-bit.


---

### 2. `Hướng dẫn Cài đặt và Chạy Game cho Người Dùng (Thông qua Releases)`

#### **Yêu cầu hệ thống**:
- **Hệ điều hành**: Windows

#### **Cài đặt và Chạy Game**:
- Tải bản phát hành game từ mục **[Releases](https://github.com/datdo13099/Hack-n-Slash/releases/tag/v1.0)** của dự án.
- Giải nén thư mục tải về vào một thư mục dễ tìm trên máy tính.
- Chạy file `.exe` trong thư mục đã giải nén. Không cần cài đặt thêm thư viện SDL2 vì tất cả các file cần thiết đã được đính kèm sẵn trong thư mục.


---

## 📁 Cấu Trúc Dự Án

### 1. `main.cpp`
- **Mục đích**: Điểm bắt đầu của trò chơi.
- **Hàm**:
  - `int main(int argc, char* argv[])`: Hàm chính của trò chơi. Khởi tạo SDL, tạo đối tượng `Game`, và bắt đầu vòng lặp trò chơi.


### 2. `game.cpp` và `game.h`
- **Mục đích**: Quản lý logic chính của trò chơi.
- **Hàm**:
  - `Game::Game()`: Khởi tạo các tài nguyên, thực thể, và trạng thái ban đầu của trò chơi.
  - `Game::~Game()`: Dọn dẹp tài nguyên và giải phóng bộ nhớ.
  - `void Game::update()`: Cập nhật trạng thái trò chơi, bao gồm di chuyển thực thể, xử lý va chạm và logic vòng chơi.
  - `void Game::draw()`: Vẽ các thực thể và giao diện lên màn hình.
  - `void Game::setupNextRound()`: Thiết lập vòng chơi tiếp theo, bao gồm số lượng quái vật và boss.
  - `void Game::setEnemyCounts(int round)`: Xác định số lượng quái vật và boss cho từng vòng.
  - `void Game::resetGame()`: Đặt lại trạng thái trò chơi về ban đầu.
  - `int Game::getBossCountForRound(int round)`: Trả về số lượng boss cho một vòng chơi cụ thể.

### 3. `hero.cpp` và `hero.h`
- **Mục đích**: Định nghĩa nhân vật chính.
- **Hàm**:
  - `Hero::Hero(AnimationSet* animSet)`: Khởi tạo nhân vật chính với tập hợp hoạt ảnh.
  - `void Hero::update()`: Cập nhật trạng thái của nhân vật chính, bao gồm di chuyển và tấn công.
  - `void Hero::draw()`: Vẽ nhân vật chính lên màn hình.
  - `void Hero::changeAnimation(int newState, bool resetFrameToBeginning)`: Thay đổi hoạt ảnh dựa trên trạng thái.
  - `void Hero::die()`: Xử lý logic khi nhân vật chính chết.
  - `void Hero::revive()`: Hồi sinh nhân vật chính.

### 4. `glob.cpp` và `glob.h`
- **Mục đích**: Định nghĩa quái vật cơ bản (Glob).
- **Hàm**:
  - `Glob::Glob(AnimationSet* animSet)`: Khởi tạo quái vật với tập hợp hoạt ảnh.
  - `void Glob::update()`: Cập nhật trạng thái và hành vi của quái vật.
  - `void Glob::draw()`: Vẽ quái vật lên màn hình.
  - `void Glob::die()`: Xử lý logic khi quái vật chết.

### 5. `boss.cpp` và `boss.h`
- **Mục đích**: Định nghĩa boss với cơ chế độc đáo.
- **Hàm**:
  - `Boss::Boss(AnimationSet* animSet, AnimationSet* bulletAnimSet)`: Khởi tạo boss với tập hợp hoạt ảnh và đạn.
  - `void Boss::update()`: Cập nhật trạng thái và hành vi của boss.
  - `void Boss::draw()`: Vẽ boss lên màn hình.
  - `void Boss::think()`: Quyết định hành động tiếp theo của boss.
  - `void Boss::die()`: Xử lý logic khi boss chết.

### 6. `wall.cpp` và `wall.h`
- **Mục đích**: Định nghĩa các tường tĩnh trong trò chơi.
- **Hàm**:
  - `Wall::Wall(AnimationSet* animSet)`: Khởi tạo tường với tập hợp hoạt ảnh.
  - `void Wall::update()`: Cập nhật trạng thái của tường.
  - `void Wall::draw()`: Vẽ tường lên màn hình.

### 7. `animation.cpp` và `animation.h`
- **Mục đích**: Quản lý hoạt ảnh cho các thực thể.
- **Hàm**:
  - `void Animation::loadAnimation(ifstream& file, list<DataGroupType>& groupTypes)`: Tải hoạt ảnh từ file.
  - `int Animation::getNextFrameNumber(int frameNumber)`: Lấy số thứ tự frame tiếp theo.
  - `Frame* Animation::getNextFrame(Frame* frame)`: Lấy con trỏ đến frame tiếp theo.

### 8. `animationSet.cpp` và `animationSet.h`
- **Mục đích**: Nhóm nhiều hoạt ảnh thành một tập hợp.
- **Hàm**:
  - `void AnimationSet::loadAnimationSet(string fileName, list<DataGroupType>& groupTypes, ...)`: Tải tập hợp hoạt ảnh từ file.

### 9. `frame.cpp` và `frame.h`
- **Mục đích**: Đại diện cho một frame trong hoạt ảnh.
- **Hàm**:
  - `void Frame::Draw(SDL_Texture* spriteSheet, float x, float y)`: Vẽ frame lên màn hình.

### 10. `entity.cpp` và `entity.h`
- **Mục đích**: Lớp cơ sở cho tất cả các thực thể trong trò chơi.
- **Hàm**:
  - `void Entity::update()`: Cập nhật trạng thái thực thể.
  - `void Entity::draw()`: Vẽ thực thể lên màn hình.

### 11. `livingEntity.cpp` và `livingEntity.h`
- **Mục đích**: Lớp cơ sở cho các thực thể có máu (nhân vật, quái vật, boss).
- **Hàm**:
  - `void LivingEntity::die()`: Xử lý logic khi thực thể chết.

### 12. `hpBar.cpp` và `hpBar.h`
- **Mục đích**: Hiển thị thanh máu cho các thực thể.
- **Hàm**:
  - `void HPBar::draw()`: Vẽ thanh máu lên màn hình.

### 13. `cameraController.cpp` và `cameraController.h`
- **Mục đích**: Điều khiển vị trí và chuyển động của camera.
- **Hàm**:
  - `void CameraController::update()`: Cập nhật vị trí camera.

### 14. `soundManager.cpp` và `soundManager.h`
- **Mục đích**: Quản lý âm thanh và nhạc nền.
- **Hàm**:
  - `void SoundManager::loadSound(string name, string filePath)`: Tải âm thanh từ file.

### 15. `timeController.cpp` và `timeController.h`
- **Mục đích**: Quản lý thời gian trong trò chơi.
- **Hàm**:
  - `void TimeController::updateTime()`: Cập nhật thời gian delta.

### 16. `randomNumber.cpp` và `randomNumber.h`
- **Mục đích**: Sinh số ngẫu nhiên cho các cơ chế trò chơi.
- **Hàm**:
  - `int getRandomNumber(int max)`: Trả về số ngẫu nhiên trong khoảng [0, max].

### 17. `res_path.cpp` và `res_path.h`
- **Mục đích**: Cung cấp đường dẫn đến tài nguyên trò chơi.
- **Hàm**:
  - `string getResourcePath()`: Trả về đường dẫn tài nguyên.

### 18. `cleanup.cpp` và `cleanup.h`
- **Mục đích**: Dọn dẹp tài nguyên SDL.
- **Hàm**:
  - `void cleanup(SDL_Texture* tex)`: Giải phóng texture.

### 19. `drawing_functions.cpp` và `drawing_functions.h`
- **Mục đích**: Cung cấp các hàm tiện ích để hiển thị.
- **Hàm**:
  - `void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y)`: Hiển thị texture lên màn hình.

### 20. `globals.cpp` và `globals.h`
- **Mục đích**: Lưu trữ các biến và hằng số toàn cục.
- **Hàm**:
  - `int Globals::ScreenWidth`: Chiều rộng màn hình.
  - `int Globals::ScreenHeight`: Chiều cao màn hình.

### 21. `keyboardInput.cpp` và `keyboardInput.h`
- **Mục đích**: Xử lý đầu vào từ bàn phím.
- **Hàm**:
  - `void KeyboardInput::update(SDL_Event* event)`: Cập nhật trạng thái bàn phím.

### 22. `groupBuilder.cpp` và `groupBuilder.h`
- **Mục đích**: Xây dựng các nhóm dữ liệu cho hoạt ảnh và thực thể.
- **Hàm**:
  - `void GroupBuilder::buildGroup(string groupName, ...)`: Tạo nhóm dữ liệu từ file.

### 23. `bullet.cpp` và `bullet.h`
- **Mục đích**: Định nghĩa đạn được bắn bởi boss hoặc nhân vật.
- **Hàm**:
  - `Bullet::Bullet(AnimationSet* animSet)`: Khởi tạo đạn với tập hợp hoạt ảnh.
  - `void Bullet::update()`: Cập nhật trạng thái và vị trí của đạn.
  - `void Bullet::draw()`: Vẽ đạn lên màn hình.

---

## 🎨 Nguồn tài nguyên

- [Lucifer Necromancer - Foozlecc](https://foozlecc.itch.io/lucifer-necromancer)  
- [Lucifer Skeleton King Boss - Foozlecc](https://foozlecc.itch.io/lucifer-skeleton-king-boss)  
- [Tổng hợp game assets miễn phí - Itch.io](https://itch.io/game-assets)

---

## 🔗 Thông tin liên hệ

[![Gmail](https://img.shields.io/badge/gmail-D14836?style=for-the-badge&logo=gmail&logoColor=white)](mailto:24021403@vnu.edu.vn)

---

Made with ❤️ by datdo
