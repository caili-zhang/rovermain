
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "chat.h"
#include "utils.h"
using namespace std;

//20170630�}���`�[�Y�ǉ��`���b�g�v���O����
//������ς��󂯂Ƃ�server�̃Z�b�g�A�b�v

bool Send::onInit(const struct timespec& time)
{
	/*
	//�\�P�b�g�̍쐬
	//�����̓A�h���X�t�@�~���A�\�P�b�g�^�C�v�A�v���g�R��
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	//sock��-1��Ԃ����玸�s
	if (sock < 0)
	{
		//�G���[��\�����鏈��
		perror("socket�̃G���[���o�܂���");
		printf("%d\n", errno);
		//return 1;
	}
	//�\�P�b�g�̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	//TCP�N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ���
	listen(sock0, 5);
	return true;
	*/
}

//���x���ڑ��v����t�����݂�
void Send::onUpdate(const struct timespec& time)
{
	/*
	//TCP�N���C�A���g����̐ڑ��v�����󂯕t����
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, (socklen_t *)&len);
	*/
}
//sock�������[�I���i�d�͏���y���炵���H�j
void Send::onClean()
{
	/*
	//listen����socket�̏I��
	close(sock0);
	mes = NULL;
	*/
}
bool Send::onCommand(const vector<string>& args)
{

	switch (args.size())
	{
	case 2:
		if (args[1].compare("sen"))
		{
			/* �\�P�b�g�̍쐬 */
			sock0 = socket(AF_INET, SOCK_STREAM, 0);

			/* �\�P�b�g�̐ݒ� */
			addr.sin_family = AF_INET;
			addr.sin_port = htons(12345);
			addr.sin_addr.s_addr = INADDR_ANY;
			bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
			//5��قǑ���Ƀ��b�Z�[�W�𑗂�����I������
			/* TCP�N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ��� */
			listen(sock0, 5);
			//while(k < 5){
			/* TCP�N���C�A���g����̐ڑ��v�����󂯕t���� */
			len = sizeof(client);
			sock = accept(sock0, (struct sockaddr *)&client, (socklen_t *)&len);
			/* 5�������M */
			n = write(sock, "HELLO", 5);
			if (n < 1)
			{
				perror("write");
				break;
			}
			/* TCP�Z�b�V�����̏I�� */
		   // k++;
			close(sock);
			/* listen ����socket�̏I�� */
			close(sock0);
		}
//		}
		else
		{
			Debug::print(LOG_PRINT, "send              : show chat state\r\n\
send sen: send messeage to client\r\n\"");
			}
return true;
}
/*
void Send::send()
{
	//client��5����HELLO�𑗂�
	write(sock, "KOUKI", 5);
	close(sock);
}
*/
Send::Send():sock(0),sock(0),mes(NULL)
{
	setName("send");
	setPriority(TASK_PRIORITY_CHAT, TASK_INTERVAL_CHAT);
}
}

Send::~Send()
{
}

//�����Ƃ��ăT�[�o�[��IP�A�h���X���K�v
bool Rec::onInit(const struct timespec& time)
{
	//�\�P�b�g�̍쐬
	//�����̓A�h���X�t�@�~���A�\�P�b�g�^�C�v�A�v���g�R��
	//sock = socket(AF_INET, SOCK_STREAM, 0);

	//�\�P�b�g�̐ݒ�
	//server.sin_family = AF_INET;
	//server.sin_port = htons(12345);
	//server.sin_addr.s_addr = inet_addr("10.0.0.5");

	return true;
}

void Rec::onUpdate(const struct timespec& time)
{
  //�\�P�b�g�̍쐬
  //�����̓A�h���X�t�@�~���A�\�P�b�g�^�C�v�A�v���g�R��
	//sock���T�[�o�[��
  
	/* �T�[�o�ɐڑ� */
	//connect(sock1, (struct sockaddr *)&server, sizeof(server));
}

void Rec::onClean()
{
}

bool Rec::onCommand(const std::vector<std::string>& args)
{
	switch (args.size())
	{
	case 2:
		if (args[1].compare("rec"))
		{
			/* �\�P�b�g�̍쐬 */
			sock = socket(AF_INET, SOCK_STREAM, 0);

			/* �ڑ���w��p�\���̂̏��� */
			server.sin_family = AF_INET;
			server.sin_port = htons(12345);
			server.sin_addr.s_addr = inet_addr("10.0.0.12");

			/* �T�[�o�ɐڑ� */
			connect(sock, (struct sockaddr *)&server, sizeof(server));

			/* �T�[�o����f�[�^����M */
			memset(buf, 0, sizeof(buf));
			n = read(sock, buf, sizeof(buf));
			if (n < 0) {
				perror("read");
				printf("����̃v���O�������牽�������Ă��ĂȂ���");
				return 1;
			}

			printf("%d, %s\n", n, buf);

			/* socket�̏I�� */
			close(sock);
			
		}
		else
		{
			Debug::print(LOG_PRINT, "rec              : show chat state\r\n\
rec rec: recieve message from server\r\n\"");
		}
	}
	return true;
}
//���V�[�u�֐�
/*
void Rec::receive() 
{
	sock1 = socket(AF_INET, SOCK_STREAM, 0);

	//�\�P�b�g�̐ݒ�
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.s_addr = inet_addr("10.0.0.10");

	connect(sock1, (struct sockaddr *)&server, sizeof(server));
	memset(buf, 0, sizeof(buf));
	n = read(sock1, buf, sizeof(buf));

	printf("%d, %s\n", n, buf);
	close(sock1);
}
*/
Rec::Rec():sock1(0),buf(),n(0)
{
	setName("rec");
	setPriority(TASK_PRIORITY_CHAT, TASK_INTERVAL_CHAT);
}

Rec::~Rec()
{
}

/*
//�T�[�o�[�ƃN���C�A���g���܂Ƃ߂��N���X
bool Chat::onInit(const struct timespec& time)
{
	gSend.setRunMode(true);
	//gRec.setRunMode(true);
	return true;
}
void Chat::onClean()
{
}

bool Chat::onCommand(const std::vector<std::string>& args)
{
	if (args.size() == 2)
	{
		//�T�[�o�[
		if (args[1].compare("sen") == 0)
		{
			gSend.send();
		}
		//�N���C�A���g
		else if (args[1].compare("rec") == 0)
		{
			//gRec.receive();
		}
		return true;
	}
	else
	{
		Debug::print(LOG_PRINT, "chat              : show chat state\r\n\
chat sen: send messeage to client\r\n\
chat rec: recieve message from server\r\n\"");
	}
	return false;
}
//������������̂͂����ƌ��߂�
Chat::Chat()
{
	setName("chat");
	setPriority(TASK_PRIORITY_CHAT, TASK_INTERVAL_CHAT);
}
Chat::~Chat()
{
}
*/

Send gSend;
//クライアント�Eインスタンスを作るとプログラムが終亁E��めE
Rec gRec;
//Chat gChat;
