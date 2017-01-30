#include "stdafx.h"
#include "stdlib.h"
#include "include/Dmc1380.h"
#include <vector>
using namespace std;

#pragma comment(lib, "lib/Dmc1380.lib")

#if !defined(__ACTION___)
#define __ACTION___








#define TOTAL_CARD_NUM           1
#define MAX_AXIS_LEN             5
#define MAX_IO_NUM               16
#define INPUT_NUM				 16
#define OUTPUT_NUM				 16
#define dwPortCount              2
#define dwPortBit                16
#define EXCUTE_OK				 0

#define STATUS_BIT_NUMS          32
#define RUN_STATUS_BIT           10
#define MIN_COMPAREA_VALUE       1E-6
#define MIN_REL_MOVE_MM          0.001

#define RETURN_OK                0
#define RETURN_CYL_ERROR         252
#define RETURN_ESTOP             254

#define POSDIR                   1
#define NEGDIR                   -1

#define MSG_MOTION_CARD          (WM_USER+200)
#define MSG_IO_CARD				 (WM_USER+201)


#define PUSH_BACK												1
#define PUSH_OUT												0

#define OFF														1
#define ON														0

#define FIRST_MOTOR												0
#define SECOND_MOTOR											1
#define THIRD_MOTOR												2

#define FIRST_ORIGIN_SENSOR										1
#define SECOND_ORIGIN_SENSOR									2


#define read_input_state(x)										!d1000_in_bit(x)
#define check_motor_origin(serial)								d1000_check_done(serial)
#define write_output(BitNo,BitData)								d1000_out_bit(BitNo,BitData)
#define estop_move(axis)										d1000_immediate_stop(axis)
#define get_position(axis)										d1000_get_command_pos(axis)		
#define start_tv_move(axis,strVel,MaxVel,Tacc)					d1000_start_tv_move(axis, strVel, MaxVel, Tacc)
#define start_t_move(axis, Dist, Vel, MaxVel, Tacc)				d1000_start_t_move(axis, Dist, Vel, MaxVel, Tacc)
#define set_in_enable(CardNo,Bits)								d1000_in_enable(CardNo,Bits)


#define MAX_COUNTER 512
#define BACK1
#define BACK2

//原点位置
#define MOTOR1_ORG	1
//9
//马达上限//限位上限 3号口，因为默认就是
#define MOTOR1_UP_LIMIT 4
//12
//马达下限			4号口，因为默认就是
#define MOTOR1_DOWN_LIMIT 5
//13
//为了保持原来的口被使用，所以in——enable是25
//也就是set_in_enable(0,25)

//急停按钮
#define IN_EMGENCY	6
//14
//机器人的输入
#define IN_START_DECT 7
//15
//伺服报警输入
#define IN_MOTOR_ALM	8
//16
//
#define IN_HAVE_GUIDE_BOARD 9


//检测完成信号
#define DETECTOVER 1
//24
//检测中信号
#define DETECTING 2 
//25
//NG信号
#define OUT_NG	3
//26
//OK信号
#define OUT_OK	4
//27
//刹车信号
#define OUT_BRAKE 5
//28先不结
//ALM信号
#define OUT_ALM 6
//29
//使能信号
#define FIRST_SRV 7
//30


//#define 

#define Error_Code int
class Action : public  CObject
{
public:
	Action();

	int CounterClock180();
	int CounterClock90();
	int Clock90();
	int FisrtMotorBackToOrigin();
	int SeconMotorBackToOrigin();
	int PushCylPushBack();
	int PushCylPush();
	int TrapOpen();
	int TrapClose();
	int ContiuneRun();
	int FirstMotorStepRunCW();
	int FirstMotorStepRunCCW();
	//手动控制正反转
	int FirstMotorStepRunCWByCtrl();
	int FirstMotorStepRunCCWByCtrl();
	int SecondMotorStepRunCW();
	int SecondMotorStepRunCCW();
	int ChopStickCYLPushOut();
	int ChopStickCYLPushBack();
	void SecondMotorRun();
	int Clock180();
	int m_spinwise1;
	int m_spinwise2;
	int ConveyorStepRun();
	bool wait_input_norm(short Bits, DWORD Time);
	bool wait_input_inverse(short Bits, DWORD Time);
	bool WaitMotorTimeout(short No, DWORD time);
	void AllCylBack();
	void CloseCard();
	int InitCard();
	int EnableSrv(int MotorNo);
	int DisableSrv(int MotorNo);
	CWinThread *RunFirstBackThread;
	bool StartFirstBackThread();
	static UINT FirstBackThread(LPVOID lParam);
	int FirstSrvAlready;
	int Procedure();
	int isOK(){ for (int i = 0; i < MAX_COUNTER; i++) write_output(OUT_OK, ON); return 0; }// Sleep(200);  for (int i = 0; i < MAX_COUNTER; i++) write_output(OUT_OK, OFF); return 0; }
	int isNotOK(){ for (int i = 0; i < MAX_COUNTER; i++) write_output(OUT_NG, ON); return 0; }// Sleep(200);  for (int i = 0; i < MAX_COUNTER; i++) write_output(OUT_NG, OFF); return 0; }
};


#endif