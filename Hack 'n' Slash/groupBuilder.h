#ifndef GROUPBUILDER  // Kiểm tra nếu GROUPBUILDER chưa được định nghĩa
#define GROUPBUILDER  // Định nghĩa GROUPBUILDER để tránh include file nhiều lần

#include "globals.h"        // Bao gồm file header chứa các định nghĩa toàn cục
#include "group.h"          // Bao gồm file header của lớp Group
#include "groupBox.h"       // Bao gồm file header của lớp GroupBox
#include "groupPosition.h"  // Bao gồm file header của lớp GroupPosition
#include "GroupNumber.h"    // Bao gồm file header của lớp GroupNumber
#include "groupString.h"    // Bao gồm file header của lớp GroupString

class GroupBuilder  // Định nghĩa lớp GroupBuilder để xây dựng và quản lý các nhóm dữ liệu
{
public:
    /**
     * LƯU Ý: Đây là giá trị duy nhất người dùng cần thiết lập. Tùy thuộc vào việc dữ liệu được lưu dưới dạng nhóm hay không
     * Ví dụ dạng nhóm:
     * hitboxes: 2
     * 12 12 34 56
     * 24 45 57 79
     * Ví dụ không theo nhóm (LỰA CHỌN MẶC ĐỊNH):
     * hitboxes: 12 12 34 56
     * hitboxes: 24 45 57 79
     * Nếu không chắc, kiểm tra fdset để xem dữ liệu được xử lý thế nào
     */
    static const bool savedInGroups;  // TODO: Thay đổi dựa trên giải thích ở trên

    /**
     * Xây dựng một Group dựa trên kiểu dữ liệu (dataType). Mặc định là STRING (vì có thể muốn bỏ qua một số dữ liệu trong tập hợp, nhưng vẫn tải nó)
     */
    static Group* buildGroup(DataGroupType dataType);  // Hàm tĩnh tạo một Group theo kiểu dữ liệu đã cho

    /**
     * Xây dựng một tập hợp các Group dựa trên danh sách các kiểu dữ liệu nhóm (groupTypes)
     */
    static void buildGroups(list<DataGroupType> groupTypes, list<Group*>& groups);  // Hàm tĩnh tạo nhiều Group từ danh sách kiểu dữ liệu

    /**
     * Thêm một GroupString vào danh sách groups (xây dựng ngay lập tức). Trả về tham chiếu nếu cần
     */
    static Group* addGroupStringToGroup(string name, list<Group*>& groups);  // Hàm tĩnh thêm GroupString vào danh sách

    /**
     * Tải một nhóm dữ liệu từ file vào danh sách groups
     */
    static void loadGroups(ifstream& file, list<Group*>& groups);  // Hàm tĩnh tải các nhóm từ file

    /**
     * Tìm một nhóm dựa trên tên. Có thể trả về null nếu không tìm thấy
     */
    static Group* findGroupByName(string str, list<Group*>& groups);  // Hàm tĩnh tìm Group theo tên trong danh sách
};

#endif  // Kết thúc khối điều kiện định nghĩa GROUPBUILDER