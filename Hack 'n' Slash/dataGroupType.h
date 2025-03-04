#ifndef DATAGROUPTYPE_H
#define DATAGROUPTYPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// chứa thông tin về loại dữ liệu trong nhóm dữ liệu (Frame data)
class DataGroupType 
{
public:
	string groupName; // tên nhóm dữ liệu, ví dụ: hitBoxes (hộp va chạm)
	int dataType; // loại dữ liệu trong nhóm, ví dụ: chuỗi, tọa độ, hình chữ nhật, góc xoay, v.v.
	bool singleItem; // nhóm dữ liệu này có chỉ chứa 1 phần tử không (thường là false, nhưng có thể có ngoại lệ)

	// lưu và tải nhóm dữ liệu cho RuleSets (RS)
	static void saveRSDataGroupType(ofstream& file, DataGroupType dataGroupType);
	static DataGroupType loadRSDataGroupType(ifstream& file);

	// các loại dữ liệu tĩnh
	static const int DATATYPE_STRING = 0;  // kiểu chuỗi
	static const int DATATYPE_POSITION = 1; // kiểu tọa độ
	static const int DATATYPE_BOX = 2; // kiểu vùng hình chữ nhật
	static const int DATATYPE_NUMBER = 3; // kiểu số
};

#endif
