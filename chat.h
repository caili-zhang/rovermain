
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
class Server : public TaskBase
{
private:
	//Linux�Ń\�P�b�g��int�ŕ\�������t�@�C���f�B�X�N���v�^
	int sock0;

	//�\���̂ŊȒP��
	struct sockaddr_in addr;
	struct sockaddr_in client;

	//���M���郁�b�Z�[�W
	char* mes;

	//�悭�������ĂȂ�
	int len;

	//sock0�Ɖ����Ⴄ�񂾂낤
	int sock;

	//�N���C�A���g����擾���镶����32
	char buf_client[32];

public:
	//�����͉�������񂾂낤
	bool init();

	void clean();

	void update(double x);
	//�G���[�`�F�b�N�p�̊֐�

	void onCommand(const std::vector<std::string>& args);

	//void error_check(sock);

	Server();
	~Server();

};
//�N���C�A���g�N���X
class Client : public TaskBase
{
private:
	//�\���̃T�[�o�[�Ɋւ���
	struct sockaddr_in server;
	int sock;
	//���M���镶��
	char buf[32];
    //������
	int n;

public:
	//int y�͓K���Ȓl�Ȃ̂ł̂��̂��C��
	bool init(int y);

	void clean();

	void update();

	void onCommand(const std::vector<std::string>& args);
	Client();
	~Client();
};
