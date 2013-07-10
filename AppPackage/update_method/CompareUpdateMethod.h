#pragma once
#include <vector>
#include "json/json.h"
using namespace std;

#include "AbstractUpdateMethod.h"

class CompareUpdateMethod : public AbstractUpdateMethod
{
public:
	CompareUpdateMethod(void);
	~CompareUpdateMethod(void);

	virtual DECOMPRESS_ERROR prepare();
	virtual DECOMPRESS_ERROR copyUpdateFile();
private:
	DECOMPRESS_ERROR makeCopyList_();
	bool getDiskJsonObject_(Json::Value& jsonObject);
	DECOMPRESS_ERROR copyAllUpdateFile_();
	vector<int> updateList_;
};
