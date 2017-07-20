//��x�����v���O�������Ăяo���錾
#pragma once
#include "task.h"
#include <sys/socket.h>
//�}���`�[�Y�ǉ�raspberrypi���m�Ń`���b�g�i���݂��̏��𑗂�j������v���O����
/*
�����Linux��œ���C++��p����TCP�ʐM�̃T�[�o�[�v���O�����ł��B
�Q�l�T�C�g(Geek�ȃy�[�W)�@http://www.geekpage.jp/programming/winsock/tcp.php
(Game engineering MAGAZINE��L�������ƃG���[���o��) http://gmagazine00.blog115.fc2.com/blog-category-2.html
*/

//�T�[�o�[�N���X
class Send : public TaskBase
{
private:
	//Linux�Ń\�P�b�g��int�ŕ\�������t�@�C���f�B�X�N���v�^
	int sock0;
	//�\���̂ŊȒP��
	struct sockaddr_in addr;
	struct sockaddr_in client;
	//
	int len;
	//
	int sock;
	//�G���[�����ŗp����nn
	int nn;
	//while���ŗp���鐔�i�����𑗂��)
	int k;
protected:
	//�����͉�������񂾂낤
	virtual bool onInit(const struct timespec& time);

	virtual void onClean();

	virtual void onUpdate(const struct timespec& time);

	virtual bool onCommand(const std::vector<std::string>& args);

	//void error_check(sock);
public:
	void send();
	Send();
	~Send();

};
//�N���C�A���g�N���X
class Rec : public TaskBase
{
private:
	//�\���̃T�[�o�[�Ɋւ���
	struct sockaddr_in server;
	int sock1;
	//���M���镶��
	char buf[32];
	//������
	int n;
protected:
	//int y�͓K���Ȓl�Ȃ̂ł̂��̂��C��
	virtual bool onInit(const struct timespec& time);

	virtual void onClean();

	virtual void onUpdate(const struct timespec& time);

	virtual bool onCommand(const std::vector<std::string>& args);
public:
	void receive();
	Rec();
	~Rec();
};
/*
class Chat : public TaskBase
{
private:
protected:
	virtual bool onInit(const struct timespec& time);
	virtual void onClean();
	virtual bool onCommand(const std::vector<std::string>& args);
public:
	Chat();
	~Chat();
};
*/
extern Send gSend;
extern Rec gRec;
//extern Chat gChat;
