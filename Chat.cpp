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
bool Server::onInit()
{
	//�\�P�b�g�̍쐬
	//�����̓A�h���X�t�@�~���A�\�P�b�g�^�C�v�A�v���g�R��
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	//sock��-1��Ԃ����玸�s
	if (sock < 0)
		{
			//�G���[��\�����鏈��
			perror("socket�̃G���[���o�܂���");
			printf("%d\n", errno);
			return 1;
		}
		//�\�P�b�g�̐ݒ�
		addr.sin_family = AF_INET;
		//�|�[�g�w��
		addr.sin_port = htons(12345);
		addr.sin_addr.s_addr = INADDR_ANY;
		bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

		//TCP�N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ���
		listen(sock0, 5);

		return true;
}
//���x���ڑ��v����t�����݂�
void Server::onUpdate(double elapsedSeconds)
{
	//TCP�N���C�A���g����̐ڑ��v�����󂯕t����
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, (socklen_t *)&len);
	printf("accepted connection from %s, port=%d\n",
		inet_ntoa(client.sin_addr), ntohs(client.sin_port));

}
//sock�������[�I���i�d�͏���y���炵���H�j
void Server::onClean()
{
	//listen����socket�̏I��
	close(sock0);
	
}
bool Server::onCommand(const vector<string>& args)
{
	switch(args.size())
	{
		case 2:
		if(args[1].compare("se"))
		{
			//client��5����HELLO�𑗂�
			write(sock, "HELLO", 5);
			close(sock);
		}
	  
	}
	Debug::print(LOG_PRINT, "chat              : show chat state\r\n\
chat se: semd messeage to client\r\n\
chat rec: recieve message from server|r\n\
");
	return true;
}

Server::Server()
{
	setName("chat");
	setPriority(TASK_PRIORITY_CHAT,  TASK_INTERVAL_CHAT);
}

Server::~Server()
{
}

//�����Ƃ��ăT�[�o�[��IP�A�h���X���K�v
bool Client::onInit(const struct timespec& time)
{
		//�\�P�b�g�̍쐬
		//�����̓A�h���X�t�@�~���A�\�P�b�g�^�C�v�A�v���g�R��
		sock = socket(AF_INET, SOCK_STREAM, 0);
		deststr = host_name;

			//�\�P�b�g�̐ݒ�
			server.sin_family = AF_INET;
			server.sin_port = htons(12345);
			//�z�X�g������Ip�A�h���X��ǂݍ��ފ֐������ƂŎg��
			server.sin_addr.s_addr = inet_addr(deststr);
			//inet_addr�����s���Ă�����host_name����Ip�A�h���X���擾
			if (server.sin_addr.s_addr == 0xffffffff) {
				struct hostent *host;

				host = gethostbyname(deststr);
				//host_name�����ip�𒲂ׂ��Ȃ������玸�s
				if (host == NULL) {
					//IP��͎��s�I
					return 1;
				}
				//(umsigned int *)�����邱�Ƃ�32�r�b�g�̒l���擾�ł���悤�ɂ��Ă���B
				server.sin_addr.s_addr = *(unsigned int *)host->h_addr_list[0];
			}

			return true;
}

void Client::onUpdate()
{
}

void Client::onClean()
{
	
}

bool Client::onCommand(const std::vector<std::string>& args)
{
	switch(args.size())
	{
		case 2:
		if(args[1].compare("rec"))
		{
			/* �T�[�o�ɐڑ� */
			connect(sock, (struct sockaddr *)&server, sizeof(server));
			memset(buf, 0, sizeof(buf));
			n = read(sock, buf, sizeof(buf));

			printf("%d, %s\r\n", n, buf);
			close(sock);
		}
	}
	return true;
}

Client::Client()
{
}

Client::~Client()
{
}

Server gServer;
Client gClient;