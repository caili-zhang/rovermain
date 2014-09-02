/*
	センサ制御プログラム

	モータ以外の実世界から情報を取得するモジュールを操作します
	task.hも参照
*/
#pragma once
#include "task.h"
#include "utils.h"
#include <pthread.h>
#include <list>

//MPL115A2からデータを取得するクラス
//気圧の値はhPa単位で+-10hPaの誤差あり
class PressureSensor : public TaskBase
{
private:
	float mA0,mB1,mB2,mC12;//気圧計算用の係数
	int mPressure;//最後に取得した気圧
	int mFileHandle;//winringPi i2c　のファイルハンドラ

	struct timespec mLastUpdateRequest;//最後に気圧の更新をMPL115A2に指示した時刻

	float val2float(unsigned int val, int total_bits, int fractional_bits, int zero_pad);
	void requestSample();
protected:
	//気圧センサを初期化
	virtual bool onInit(const struct timespec& time);
	//センサの使用を終了する
	virtual void onClean();

	//一定間隔ごとに気圧をアップデートする
	virtual void onUpdate(const struct timespec& time);

	//コマンドを処理する
	virtual bool onCommand(const std::vector<std::string>& args);
public:
	//最後にアップデートされた気圧を返す
	int get();

	PressureSensor();
	~PressureSensor();
};

//Navigatron v2からデータを取得するクラス
class GPSSensor : public TaskBase
{
private:
	struct timespec mLastCheckTime;//前回のチェック時刻
	int mFileHandle;//winringPi i2c　のファイルハンドラ
	VECTOR3 mPos;//座標(経度、緯度、高度)
	int mSatelites;//補足した衛星の数
	bool mIsNewData;//新しい座標データがあれば真
	bool mIsLogger;//真なら1秒ごとにgpsコマンドを実行

	void showState();//補足した衛星数と座標を表示
protected:
	//GPSを初期化
	virtual bool onInit(const struct timespec& time);
	//センサの使用を終了する
	virtual void onClean();
	//現在の座標をアップデートする
	virtual void onUpdate(const struct timespec& time);
	//コマンドを処理する
	virtual bool onCommand(const std::vector<std::string>& args);

public:
	//現在の座標を取得する(falseを返した場合は場所が不明)
	//disableNewFlagをfalseにすると座標が新しいという情報を削除
	bool get(VECTOR3& pos, bool disableNewFlag = false);

	//前回の座標取得以降にデータが更新された場合は真
	bool isNewPos();

	GPSSensor();
	~GPSSensor();
};

//L3GD20からデータを取得するクラス
class GyroSensor : public TaskBase
{
private:
	int mFileHandle;//winringPi i2c　のファイルハンドラ
	VECTOR3 mRVel;//角速度
	VECTOR3 mRAngle;//角度
	struct timespec mLastSampleTime;

	//ドリフト誤差補正用
	std::list<VECTOR3> mRVelHistory;//過去の角速度
	VECTOR3 mRVelOffset;//サンプルあたりのドリフト誤差の推定値
	bool mIsCalculatingOffset;//ドリフト誤差計算中フラグ
protected:
	//ジャイロセンサを初期化
	virtual bool onInit(const struct timespec& time);
	//センサの使用を終了する
	virtual void onClean();

	//一定間隔ごとにデータをアップデートする
	virtual void onUpdate(const struct timespec& time);

	//コマンドを処理する
	virtual bool onCommand(const std::vector<std::string>& args);
public:
	//最後にアップデートされたデータを返す
	bool getRVel(VECTOR3& vel);
	double getRvx();
	double getRvy();
	double getRvz();

	//////////////////////////////////////////////////
	//角速度から計算された角度を処理する関数

	//現在の角度を基準とする
	void setZero();

	//現在の角度を返す(-180〜+180)
	bool getRPos(VECTOR3& pos);
	double getRx();
	double getRy();
	double getRz();

	//ドリフト誤差を補正する(静止状態で呼び出すこと)
	void calibrate();

	//引数のベクトルを(-180〜+180)の範囲に修正
	static void normalize(VECTOR3& pos);
	static double normalize(double pos);

	GyroSensor();
	~GyroSensor();
};

//MMA8451Qからデータを取得するクラス
class AccelerationSensor : public TaskBase
{
private:
	int mFileHandle;//winringPi i2c　のファイルハンドラ
	VECTOR3 mAccel;//加速度
protected:
	virtual bool onInit(const struct timespec& time);
	virtual void onClean();
	virtual void onUpdate(const struct timespec& time);
	virtual bool onCommand(const std::vector<std::string>& args);
public:
	//最後にアップデートされたデータを返す
	bool getAccel(VECTOR3& acc);
	double getAx();
	double getAy();
	double getAz();
	
	double getTheta(); //XY
	double getPsi(); //YZ
	double getPhi(); //XZ

	AccelerationSensor();
	~AccelerationSensor();
};

//Cdsからデータを取得するクラス
class LightSensor : public TaskBase
{
private:
	int mPin;
protected:
	//初期化
	virtual bool onInit(const struct timespec& time);
	//センサの使用を終了する
	virtual void onClean();
	//コマンドを処理する
	virtual bool onCommand(const std::vector<std::string>& args);

public:
	//現在の明るさを取得する
	bool get();

	LightSensor();
	~LightSensor();
};

//Webカメラの動画をキャプチャするクラス
class WebCamera : public TaskBase
{
protected:
	virtual bool onCommand(const std::vector<std::string>& args);
	virtual void onClean();
public:
	void start(const char* filename = NULL);
	void stop();

	WebCamera();
	~WebCamera();
};

//距離センサーを操作するクラス
class DistanceSensor : public TaskBase
{
	double mLastDistance;
	struct timespec mLastSampleTime;
	pthread_t mPthread;
	volatile bool mIsCalculating;
	volatile bool mIsNewData;

	static void* waitingThread(void* arg);
protected:
	virtual bool onInit(const struct timespec& time);
	virtual void onClean();
	virtual void onUpdate(const struct timespec& time);
	virtual bool onCommand(const std::vector<std::string>& args);
public:
	bool ping();//距離センサーに計測を指示する

	//計測された距離を返す(新しいデータであればtrueを返す)
	//計測不能であれば-1を返す
	bool getDistance(double& distance);

	DistanceSensor();
	~DistanceSensor();
};

#include <opencv2/opencv.hpp>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
class CameraCapture : public TaskBase
{
	CvCapture* mpCapture;
	bool mIsWarming;
	Filename mFilename;
	unsigned int mCurVideoDeviceID;//現在使用しているカメラのデバイス番号(/dev/video*)

	const static int WIDTH = 320,HEIGHT = 240;
protected:
	virtual bool onInit(const struct timespec& time);
	virtual void onClean();
	virtual bool onCommand(const std::vector<std::string>& args);
	virtual void onUpdate(const struct timespec& time);

	void verifyCamera(bool reinitialize = true);
public:
	void startWarming();//getFrameする少し前に呼び出すこと.古い画像が取得されるのを防止できる
	IplImage* getFrame();

	void save(const std::string* name = NULL,IplImage* pImage = NULL, bool nolog = false);

	CameraCapture();
	~CameraCapture();
};

extern GyroSensor gGyroSensor;
extern GPSSensor gGPSSensor;
extern PressureSensor gPressureSensor;
extern LightSensor gLightSensor;
extern WebCamera gWebCamera;
extern DistanceSensor gDistanceSensor;
extern CameraCapture gCameraCapture;
extern AccelerationSensor gAccelerationSensor;

