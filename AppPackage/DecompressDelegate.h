#pragma once

typedef enum 
{
	CREATE_THREAD_ERROR,				//创建线程失败
	SOURCE_FILE_PATH_ERROR,				//源路径错误
	TARGET_DIR_PATH_ERROR,				//目标路径错误
	OPEN_ZIP_FILE_ERROR,				//打开ZIP文件错误，可能是格式错误或ZIP不存在
	GET_ZIP_HEAD_ERROR,					//取ZIP文件头失败
	GET_ZIP_ITEM_ERROR,					//取ZIP单元失败
	UNZIP_PROC_ERROR,					//解压过程失败
	NO_ENOUGH_SPACE_ERROR,				//没有足够的空间
	JSON_FILE_PARSE_ERROR,				//JSON文件解析失败
	MD5_CHECK_ERROR,					//MD5文件校验失败

}DECOMPRESS_ERROR;
class DecompressDelegate
{
public:
	DecompressDelegate(void);
	~DecompressDelegate(void);
	//错误发生时的回调
	virtual void decompressError(DECOMPRESS_ERROR errorCode) = 0;
	//取得头信息时的回调,参数为zip文件包含单元的个数
	virtual void updateZipInfo(DWORD zipFileCount) = 0;
	//解压每个单元后会回调,参数解压的文件名称
	virtual void updateStep(const TCHAR* fileName) = 0;
	//解压结束后的回调
	virtual void decompressFinish(){};
};
