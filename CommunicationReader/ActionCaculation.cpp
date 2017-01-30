#include "ActionCaculation.h"
//#include "MotionCard.h"
#include "include\\Preferences.h"

#pragma warning(disable:4996)

Action::Action()
	: m_spinwise1(1)
	, m_spinwise2(1)
{
	FirstSrvAlready = 0;
}

int Action::Clock180()
{
	return 1;
}


int Action::CounterClock180()
{
	return 1;
}


int Action::CounterClock90()
{
	return 1;
}


int Action::Clock90()
{
	return 1;
}


void Action::SecondMotorRun()
{
	return;
}


//���������ԭ��FisrtMotorBackToOrigin
int Action::FisrtMotorBackToOrigin()
{

	//0��ʾ������ԭ��
	int compansate = ini.m_Motor1Wise*ini.m_Motor1Compensate;
	int speed = ini.m_Motor1Wise*ini.m_Motor1BackSpeed;//�Ҽ���λ���ٶ�
	int backspeed = speed/10;//��λԶ����ٶȣ�����������ΪҪ���׼
	int initspeed = speed / 1000;
	double acc = 0.05;
	DWORD StartTick = GetTickCount();
	//�����˼���λ�ã�
	if (read_input_state(MOTOR1_UP_LIMIT))			//��Ӧ���ӷ�������bug//
	{
		for (; read_input_state(MOTOR1_UP_LIMIT);)
		{
			d1000_start_tv_move(FIRST_MOTOR, -1 * initspeed, -1 * backspeed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		d1000_immediate_stop(FIRST_MOTOR);		
	}
	else
	{	//���ڼ���λ�ã�������λ�õķ����ߣ�Ȼ��
		for (; !read_input_state(MOTOR1_UP_LIMIT);)
		{
			d1000_start_tv_move(FIRST_MOTOR, initspeed, speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		//��ʵ������Զ�ֹͣ
		d1000_immediate_stop(FIRST_MOTOR);
		for (; read_input_state(MOTOR1_UP_LIMIT);)
		{
			d1000_start_tv_move(FIRST_MOTOR, -1 * initspeed, -1 * backspeed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 8000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		d1000_immediate_stop(FIRST_MOTOR);
	}
	g.RunDisable = NoError;
	d1000_set_command_pos(FIRST_MOTOR, 0);
	return 0;





















	return 0;

	//d1000_home_move(FIRST_MOTOR, initspeed, speed, acc);
	//˵���Ѿ��ڼ���λ�ã�������
	if (read_input_state(MOTOR1_UP_LIMIT))			//��Ӧ���ӷ�������bug
	{
		for (; read_input_state(MOTOR1_UP_LIMIT);)
		{ 
			d1000_start_tv_move(FIRST_MOTOR, -1*initspeed, -1*speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		d1000_immediate_stop(FIRST_MOTOR);
#ifdef BACK1
		for (; read_input_state(MOTOR1_ORG);)
		{ 
			d1000_start_tv_move(FIRST_MOTOR, -1 * initspeed, -1 * speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		Sleep(100);
		d1000_decel_stop(FIRST_MOTOR);
	//	d1000_start_t_move(FIRST_MOTOR, -1 * compansate, initspeed, speed, 0.05);
#else
		d1000_start_t_move(FIRST_MOTOR,-1*compansate, initspeed, speed, 0.05);
#endif
	}
	else
	{	//���ڼ���λ�ã�������λ�õķ����ߣ�Ȼ��
		for (; !read_input_state(MOTOR1_UP_LIMIT);)
		{
			d1000_start_tv_move(FIRST_MOTOR, initspeed, speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		//��ʵ������Զ�ֹͣ
		d1000_immediate_stop(FIRST_MOTOR);
		for (; read_input_state(MOTOR1_UP_LIMIT);)
		{
			d1000_start_tv_move(FIRST_MOTOR, -1*initspeed, -1*speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 8000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		d1000_immediate_stop(FIRST_MOTOR);


#ifdef BACK1
		for (; read_input_state(MOTOR1_ORG);)
		{
			d1000_start_tv_move(FIRST_MOTOR, -1 * initspeed, -1 * speed, 0.05);
			if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return EMERGENGCY;
			}
			if ((GetTickCount() - StartTick) > 6000)
			{
				d1000_immediate_stop(FIRST_MOTOR);
				return BACKTIMEOUT;
			}
		}
		Sleep(100);
		d1000_decel_stop(FIRST_MOTOR);
		//d1000_immediate_stop(FIRST_MOTOR);
#else
		d1000_start_t_move(FIRST_MOTOR,-1*compansate, initspeed, speed, 0.05);
#endif
	}

	StartTick = GetTickCount();
#ifdef BACK1
	for (; !read_input_state(MOTOR1_ORG);)
	{
		d1000_start_tv_move(FIRST_MOTOR, initspeed, speed/10, 0.05);
		if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return EMERGENGCY;
		}
		if ((GetTickCount() - StartTick) > 6000)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return BACKTIMEOUT;
		}
	}
#else
	for (; !read_input_state(MOTOR1_UP_LIMIT);)
	{
		d1000_start_tv_move(FIRST_MOTOR, initspeed, speed/10, 0.05);
		if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return EMERGENGCY;
		}
		if ((GetTickCount() - StartTick) > 6000)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return BACKTIMEOUT;
		}
	}
#endif
	d1000_immediate_stop(FIRST_MOTOR);
	g.RunDisable &= ~NOT_ALREADY;
	d1000_set_command_pos(FIRST_MOTOR, 0);
	return 0;
	//�ҵ�����λ��
	StartTick = GetTickCount();
	d1000_start_tv_move(SECOND_MOTOR, backspeed,backspeed, 0.05); //���ܲ���bug����Ҫ��������?
	StartTick = GetTickCount();
	for (; d1000_in_bit(FIRST_ORIGIN_SENSOR);) //origin is for (; !d1000_in_bit(FIRST_ORIGIN_SENSOR);) 
	{
		if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return EMERGENGCY;
		}
		if ((GetTickCount() - StartTick) > 2000)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return BACKTIMEOUT;
		}
	}
	for (; !d1000_in_bit(FIRST_ORIGIN_SENSOR);)
	{
		if (::WaitForSingleObject(g.evtEmgencyStop, 0) == WAIT_OBJECT_0)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return EMERGENGCY;
		}
		if ((GetTickCount() - StartTick) > 6000)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return BACKTIMEOUT;
		}
	}
	d1000_immediate_stop(FIRST_MOTOR);
//	g.RunDisable &= ~NOT_ALREADY;
	g.RunDisable = NoError;
	return 0;
}

//�ŷ������ԭ��
int Action::SeconMotorBackToOrigin()
{
	return 1;
}



int Action::ChopStickCYLPushOut()
{
	//write_output(OUT_PUSHOUTCHOPSTIC, PUSH_OUT);
	return 1;
}


int Action::ChopStickCYLPushBack()
{
	//write_output(OUT_PUSHOUTCHOPSTIC, PUSH_BACK);
	return 1;
}


int Action::PushCylPush()
{

	return 1;
}

int Action::PushCylPushBack()
{
	return 1;
}

int Action::TrapOpen()
{
	return 1;
}


int Action::TrapClose()
{
	return 1;
}



int Action::ContiuneRun()
{
	return 0;
}


int Action::FirstMotorStepRunCW()
{
#ifdef STEPVERSION
	long Dis = ini.m_Motor1Wise * ini.m_Motor1Step - d1000_get_command_pos(FIRST_MOTOR);
	start_t_move(FIRST_MOTOR, Dis, 500, ini.m_Motor1Speed, 0.05);
	bool flag = true;
	long changeSpeedWaves = long(0.90*Dis);
	long changeSpeed = ini.m_Motor1Speed / 10;
	DWORD Tick = GetTickCount();
	for (; !d1000_check_done(FIRST_MOTOR);)
	{
		if ((true == flag) && (abs(d1000_get_command_pos(FIRST_MOTOR)) > abs(changeSpeedWaves)))
		{
			d1000_change_speed(FIRST_MOTOR, changeSpeed);
			flag = false;
		}
		if ((GetTickCount() - Tick) > 12000)
		{
			d1000_immediate_stop(FIRST_MOTOR);
			return 1;
		}
		Sleep(1);
	}
#endif

#ifdef DOWNLIMITVERSION
	for(;!read_input_state(MOTOR1_DOWN_LIMIT);)
	{
		start_tv_move(FIRST_MOTOR, -1 * ini.m_Motor1Wise * 500, -1 * ini.m_Motor1Wise * ini.m_Motor1Speed, 0.05);
	}
	d1000_immediate_stop(FIRST_MOTOR);
#endif
	return 0;
}

int Action::FirstMotorStepRunCCW()
{
#ifdef STEPVERSION
	//int tmp = -1*ini.m_Motor1Wise * ini.m_Motor1Step;
	long Dis = 0-d1000_get_command_pos(FIRST_MOTOR);//0-speed is faster than -1*speed
	start_t_move(FIRST_MOTOR, Dis, 500, ini.m_Motor1Speed, 0.05);
#endif

#ifdef DOWNLIMITVERSION
	for (; !read_input_state(MOTOR1_UP_LIMIT);)
	{
		start_tv_move(FIRST_MOTOR, ini.m_Motor1Wise * 500, ini.m_Motor1Wise * ini.m_Motor1Speed, 0.05);
	}
	d1000_immediate_stop(FIRST_MOTOR);
#endif
	return 1;
}

int Action::FirstMotorStepRunCWByCtrl()
{

	int tmp = ini.m_Motor1Wise * ini.m_Motor1StepByContol;
#ifdef MYDEBUG
	cout << ini.m_Motor1Wise << endl;
	cout << tmp << endl;
#endif
	start_t_move(FIRST_MOTOR, tmp, 500, ini.m_Motor1Speed, 0.05);
	return 1;
}

int Action::FirstMotorStepRunCCWByCtrl()
{
	int tmp = -1*ini.m_Motor1Wise * ini.m_Motor1StepByContol;
	start_t_move(FIRST_MOTOR, tmp, 500, ini.m_Motor1Speed, 0.05);
	return 1;
}

int Action::SecondMotorStepRunCW()
{
	return 1;
}

int Action::SecondMotorStepRunCCW()
{
	return 1;
}


int Action::ConveyorStepRun()
{
	return 1;
}


bool Action::wait_input_norm(short Bits, DWORD Time)
{
	DWORD startTick = GetTickCount();
	for (; d1000_in_bit(Bits);)
	{
		int endTick = GetTickCount();
		if ((endTick - startTick) > Time) return false;
		Sleep(1);
	}
	return true;
}

bool Action::wait_input_inverse(short Bits, DWORD Time)
{
	DWORD startTick = GetTickCount();
	for (;!d1000_in_bit(Bits);)
	{
		int endTick = GetTickCount();
		if ((endTick - startTick) > Time) return false;
		Sleep(1);
	}
	return true;
}

bool Action::WaitMotorTimeout(short No, DWORD time)
{
	DWORD startTick = GetTickCount();
	while (!d1000_check_done(No))
	{
		if ((GetTickCount() - startTick) > time)
		{
			return false;
		}
		Sleep(1);
	}
	return true;	
}


void Action::AllCylBack()
{
	return;

	//write_output(OUT_PUSHOUTCHOPSTIC, PUSH_BACK);
}


void Action::CloseCard()
{
	DisableSrv(FIRST_SRV);
	d1000_board_close();
}

int Action::InitCard()
{

	if (!d1000_board_init())
	{
	//	d1000_set_pls_outmode(0, 1);//���� �½�����Ч
		return -1;
	}
	EnableSrv(FIRST_SRV);
	d1000_in_enable(0, 25);						 //define IO������˵��25����������֪�����Բ�ѯDMC1380��˵����
	//d1000_in_enable(ini->deviceNumber, 3);	
	d1000_set_pls_outmode(0, 1);
	d1000_set_pls_outmode(1, 1);
	d1000_set_command_pos(0, 0);
	write_output(OUT_ALM, ON);
	write_output(DETECTOVER, ON);
	return 0;//0��ʾ�ɹ�
}



int Action::EnableSrv(int MotorNo)
{
	//������bug��Ҫ�ظ����ε��źŲ���������
	for (int i = 0; i < 512; i++, write_output(MotorNo, ON));
	return 0;
}

int Action::DisableSrv(int MotorNo)
{
	for (int i = 0; i < 512; i++, write_output(MotorNo, OFF));
	return 0;
}


bool Action::StartFirstBackThread()
{
	RunFirstBackThread = AfxBeginThread(FirstBackThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

UINT Action::FirstBackThread(LPVOID lParam)
{
	Action* pAction = (Action*)lParam;
//	pAction->firstBackInfo = 0;
	if (pAction->RunFirstBackThread) return THREADRUNNING;
	int res = pAction->FisrtMotorBackToOrigin();
	delete pAction->RunFirstBackThread;
	pAction->RunFirstBackThread = NULL;
//	pAction->firstBackInfo = res;
	return res;
}

int Action::Procedure()
{
	
	//��Ҫ���Ұ�CW()����Ϊ�½�����Ϊ�����ط����ϣ�����Ҫ��CW��ĺ�����ǧ��ҪŪ��
	if (read_input_state(IN_MOTOR_ALM)) return MOTOR_ALM; //g.RunDisable
	if (NOT_ALREADY == (NOT_ALREADY&g.RunDisable)) return NOT_ALREADY;
	FirstMotorStepRunCW();
	
	if (!WaitMotorTimeout(SECOND_MOTOR, 5000))	return MOTOR_TIME_OUT;
	//return 0;
	Sleep(500);//�ȴ���Ӧ���ȶ�
	//g.StartRecord = true; //��ʾ���Կ�ʼ��¼���ݣ����ݼ�¼��ʽ�Ǵ�OnComm�￪ʼ�ģ���Ϊ����������ַ���
	Sleep(200);//�ȴ���¼���
	FirstMotorStepRunCCW();
	if (!WaitMotorTimeout(SECOND_MOTOR, 5000))	return MOTOR_TIME_OUT;
	return NoError;
}
