#include "groupBuilder.h"

// Định nghĩa biến tĩnh savedInGroups (mặc định là false)
// TODO: Thay đổi giá trị này nếu fdset được thiết lập để lưu theo nhóm
const bool GroupBuilder::savedInGroups = false;

// Hàm xây dựng một Group dựa trên kiểu dữ liệu DataGroupType
Group* GroupBuilder::buildGroup(DataGroupType dataType)
{
    Group* group = NULL;  // Khởi tạo con trỏ Group là NULL

    // Chọn loại Group dựa trên kiểu dữ liệu
    if (dataType.dataType == DataGroupType::DATATYPE_NUMBER)
    {
        group = new GroupNumber(dataType);  // Tạo GroupNumber nếu là số
    }
    else if (dataType.dataType == DataGroupType::DATATYPE_POSITION)
    {
        group = new GroupPosition(dataType);  // Tạo GroupPosition nếu là vị trí
    }
    else if (dataType.dataType == DataGroupType::DATATYPE_BOX)
    {
        group = new GroupBox(dataType);  // Tạo GroupBox nếu là hộp
    }
    else
    {
        group = new GroupString(dataType);  // Mặc định là GroupString nếu không khớp kiểu nào
    }

    return group;  // Trả về con trỏ tới Group vừa tạo
}

// Hàm xây dựng nhiều Group từ danh sách kiểu dữ liệu
void GroupBuilder::buildGroups(list<DataGroupType> groupTypes, list<Group*>& groups)
{
    for (list<DataGroupType>::iterator dgt = groupTypes.begin(); dgt != groupTypes.end(); dgt++)
    {
        Group* group = buildGroup((*dgt));  // Tạo Group từ kiểu dữ liệu hiện tại
        groups.push_back(group);            // Thêm Group vào danh sách
    }
}

// Hàm thêm một GroupString vào danh sách groups
Group* GroupBuilder::addGroupStringToGroup(string name, list<Group*>& groups)
{
    DataGroupType dgt;  // Tạo một DataGroupType mới
    dgt.dataType = DataGroupType::DATATYPE_STRING;  // Đặt kiểu dữ liệu là chuỗi
    dgt.groupName = name;                           // Gán tên nhóm
    dgt.singleItem = false;                         // Đặt là không phải mục đơn lẻ
    Group* group = new GroupString(dgt);            // Tạo GroupString mới
    groups.push_back(group);                        // Thêm vào danh sách groups
    return group;                                   // Trả về con trỏ tới Group vừa tạo
}

// Hàm tải các Group từ file
void GroupBuilder::loadGroups(ifstream& file, list<Group*>& groups)
{
    // Đọc từng nhóm từ file cho đến khi hết
    while (!file.eof())
    {
        // Lấy dòng tiếp theo từ file
        int positionBeforeRead = file.tellg();  // Lưu vị trí trước khi đọc
        string line;                            // Chuỗi để lưu dòng
        getline(file, line);                    // Đọc một dòng
        if (line.empty() || line == "")
            break;  // Thoát nếu dòng rỗng, có thể là hết file

        if (savedInGroups)  // Nếu dữ liệu được lưu theo nhóm
        {
            // Tìm dấu hai chấm
            int pos = line.find(":", 0);
            if (pos == -1)
            {
                file.seekg(positionBeforeRead);  // Quay lại vị trí trước khi đọc
                break;  // Có thể đang đọc dữ liệu khác (như animation), thoát vòng lặp
                // TODO: Xử lý cách quay lại để đọc các phần khác
            }
            Group* group = findGroupByName(line.substr(0, pos), groups);  // Tìm nhóm theo tên
            if (group == NULL)
            {
                // Nếu không tìm thấy nhóm, tạo mới GroupString để lưu dữ liệu
                group = addGroupStringToGroup(line.substr(0, pos), groups);
            }
            // Lấy số lượng mục từ phần sau dấu hai chấm
            string numStr = line.substr(pos + 1, line.length() - pos + 2);
            stringstream ss;
            ss << numStr << endl;
            int num;
            ss >> num;  // Chuyển đổi thành số nguyên

            // Đọc dữ liệu cho từng mục trong nhóm
            for (int i = 0; i < num; i++)
            {
                if (!file.good())
                    break;  // Thoát nếu file không còn đọc được

                getline(file, line);  // Đọc dòng tiếp theo
                group->addToGroup(line);  // Thêm dữ liệu vào nhóm
            }
        }
        else  // Nếu dữ liệu không được lưu theo nhóm
        {
            // Tìm dấu hai chấm
            int pos = line.find(":", 0);
            if (pos == -1)
            {
                file.seekg(positionBeforeRead);  // Quay lại vị trí trước khi đọc
                break;  // Có thể đang đọc dữ liệu khác (như animation), thoát vòng lặp
                // TODO: Xử lý cách quay lại để đọc các phần khác
            }
            cout << "sub: " << line.substr(0, pos) << endl;  // In tên nhóm (dùng để debug)
            Group* group = findGroupByName(line.substr(0, pos), groups);  // Tìm nhóm theo tên
            if (group == NULL)
            {
                // Nếu không tìm thấy nhóm, tạo mới GroupString để lưu dữ liệu
                group = addGroupStringToGroup(line.substr(0, pos), groups);
            }
            // Loại bỏ tiêu đề và thêm dữ liệu vào nhóm
            line = Globals::clipOffDataHeader(line);  // Cắt bỏ phần "groupName: "
            group->addToGroup(line);                  // Thêm dữ liệu vào nhóm
        }
    }
}

// Hàm tìm Group theo tên trong danh sách
Group* GroupBuilder::findGroupByName(string str, list<Group*>& groups)
{
    for (list<Group*>::iterator group = groups.begin(); group != groups.end(); group++)
    {
        if (str == (*group)->type.groupName)  // So sánh tên nhóm
            return (*group);                  // Trả về Group nếu tìm thấy
    }

    return NULL;  // Trả về NULL nếu không tìm thấy
}