#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "chat.h"
#include "utils.h"
using namespace std;

//�}���`�[�Y�ǉ��`���b�g�v���O����

bool Server::init()
{
	//�\�P�b�g�̍쐬
	//�����̓A�h���X�t�@�~���A�\�P�b�g�^�C�v�A�v���g�R��
	sock0 = socket(AF_INET, SOCK_STREAM, 0);


}

void error_check(int sock) 
{
	//sock��-1��Ԃ����玸�s
	if (sock < 0)
	{
		//�G���[��\�����鏈��
		perror("socket�̃G���[���o�܂���");
		printf("%d\n", errno);
		return 1;
	}
}