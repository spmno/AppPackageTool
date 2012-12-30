#pragma once

typedef enum 
{
	CREATE_THREAD_ERROR,				//�����߳�ʧ��
	SOURCE_FILE_PATH_ERROR,				//Դ·������
	TARGET_DIR_PATH_ERROR,				//Ŀ��·������
	OPEN_ZIP_FILE_ERROR,				//��ZIP�ļ����󣬿����Ǹ�ʽ�����ZIP������
	GET_ZIP_HEAD_ERROR,					//ȡZIP�ļ�ͷʧ��
	GET_ZIP_ITEM_ERROR,					//ȡZIP��Ԫʧ��
	UNZIP_PROC_ERROR,					//��ѹ����ʧ��
	NO_ENOUGH_SPACE_ERROR,				//û���㹻�Ŀռ�
	JSON_FILE_PARSE_ERROR,				//JSON�ļ�����ʧ��
	MD5_CHECK_ERROR,					//MD5�ļ�У��ʧ��

}DECOMPRESS_ERROR;
class DecompressDelegate
{
public:
	DecompressDelegate(void);
	~DecompressDelegate(void);
	//������ʱ�Ļص�
	virtual void decompressError(DECOMPRESS_ERROR errorCode) = 0;
	//ȡ��ͷ��Ϣʱ�Ļص�,����Ϊzip�ļ�������Ԫ�ĸ���
	virtual void updateZipInfo(DWORD zipFileCount) = 0;
	//��ѹÿ����Ԫ���ص�,������ѹ���ļ�����
	virtual void updateStep(const TCHAR* fileName) = 0;
	//��ѹ������Ļص�
	virtual void decompressFinish(){};
};
