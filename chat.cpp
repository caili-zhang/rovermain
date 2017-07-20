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

//20170630マルチ�Eズ追加チャチE��プログラム
//斁E���E委を受けとるserverのセチE��アチE�E
//Send�N���X�͑���Ƀ��b�Z�[�W��������܂Ŏ��s�����B
//����ꂽ��sock����ďI���A���݂͑����Ȃ��ƃv���O�����̏I�����ł��Ȃ��Ȃ�B
bool Send::onInit(const struct timespec& time)
{
	return true;
}

//何度も接続要求受付を試みめE
void Send::onUpdate(const struct timespec& time)
{
}
//sock操作を一端終亁E��電力消費軽減らしい�E�！E
void Send::onClean()
{
}
bool Send::onCommand(const vector<string>& args)
{
  if(args.size()==2)
	{
		if (args[1].compare("sen")==0)
		{
			/* ソケチE��の作�E */
			sock0 = socket(AF_INET, SOCK_STREAM, 0);
			//�����܂ł͓����Ă���
			Debug::print(LOG_PRINT, "Buzzer is already stopping\r\n");
			/* ソケチE��の設宁E*/
			addr.sin_family = AF_INET;
			addr.sin_port = htons(12345);
			addr.sin_addr.s_addr = INADDR_ANY;
			bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
			Debug::print(LOG_PRINT, "FIRE");
			//5回ほど相手にメチE��ージを送ったら終亁E��めE
			/* TCPクライアントから�E接続要求を征E��る状態にする */
			listen(sock0, 5);
			//while(k < 5){
			/* TCPクライアントから�E接続要求を受け付けめE*/
			len = sizeof(client);
			sock = accept(sock0, (struct sockaddr *)&client, (socklen_t *)&len);
			/* 5斁E��送信 */
			Debug::print(LOG_PRINT, "accepted connection from %s, port=%d\n",
				inet_ntoa(client.sin_addr), ntohs(client.sin_port));
			send(sock, "HELLO", 5);
			/* TCPセチE��ョンの終亁E*/
			close(sock);
			/* listen するsocketの終亁E*/
			close(sock0);
			return true;
		}
		return false;
//		}
	}
  else {
	  Debug::print(LOG_PRINT, "chat_s              : show chat state\r\n\
chat_s sen: send messeage to client\r\n\"");
	  return true;
  }
}

Send::Send()
{
	setName("chat_s");
	setPriority(TASK_PRIORITY_SEND, TASK_INTERVAL_SEND);
}

Send::~Send()
{
}

//引数としてサーバ�EのIPアドレスが忁E��E
bool Rec::onInit(const struct timespec& time)
{
	return true;
}

void Rec::onUpdate(const struct timespec& time)
{
}

void Rec::onClean()
{
}

bool Rec::onCommand(const std::vector<std::string>& args)
{
	//Debug::print(LOG_PRINT, "FIRE_soto");
	if(args.size() == 2)
	{
		if (args[1].compare("rec") == 0)
		{
			/* ソケチE��の作�E */
			sock = socket(AF_INET, SOCK_STREAM, 0);
			//�����܂ł͓����Ă���
			Debug::print(LOG_PRINT, "Buzzer is already stopping\r\n");
			/* 接続�E持E��用構造体�E準備 */
			server.sin_family = AF_INET;
			server.sin_port = htons(12345);
			server.sin_addr.s_addr = inet_addr("10.0.0.12");

			/* サーバに接綁E*/
			connect(sock, (struct sockaddr *)&server, sizeof(server));

			/* サーバからデータを受信 */
			memset(buf, 0, sizeof(buf));
			//�����܂œ����Ă���
			Debug::print(LOG_PRINT, "Buzzer is already stopping\r\n");
			n = read(sock, buf, sizeof(buf));
			if (n < 0) {
				perror("read");
				printf("相手�Eプログラムから何も送られてきてなぁE��");
				return 1;
			}
			Debug::print(LOG_PRINT, "Buzzer is already stopping\r\n");
			Debug::print(LOG_PRINT,"%d, %s\n", n, buf);
			/* socketの終亁E*/
			close(sock);
			return true;
		}
		//Debug::print(LOG_PRINT, "FIREF");
		return false;
	}
	else {
		Debug::print(LOG_PRINT, "chat_r              : show chat state\r\n\
chat_r rec: recieve message from server\r\n\"");
		return true;
	}
}
//レシーブ関数

Rec::Rec():buf(),n(0)
{
	setName("chat_r");
	setPriority(TASK_PRIORITY_REC, TASK_INTERVAL_REC);
}

Rec::~Rec()
{
}

/*
//サーバ�Eとクライアントをまとめたクラス
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
		//サーバ�E
		if (args[1].compare("sen") == 0)
		{
			gSend.send();
		}
		//クライアンチE
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
//初期化するものはちめE��と決める
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
//繧�E�繝ｩ繧�E�繧�E�繝ｳ繝医・繧�E�繝ｳ繧�E�繧�E�繝ｳ繧�E�繧剁E��懊ｋ縺�E�繝励Ο繧�E�繝ｩ繝�縺檎ｵめE��・☁E��・
Rec gRec;
//Chat gChat;
