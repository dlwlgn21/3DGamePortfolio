#include "Time.h"

namespace jh
{
	LARGE_INTEGER Time::mCPUFrequency;
	float Time::mDeltaTime = 0.0f;
	HWND Time::mHwnd;

	float Time::mScale = 1.0f;
	double Time::mdSecondPerCount{ 0.0 };
	long long Time::mllBaseTime{ 0 };
	long long Time::mllPausedTime{ 0 };
	long long Time::mllStopTime{ 0 };
	long long Time::mllPrevTime{ 0 };
	long long Time::mllCurrTime{ 0 };
	bool Time::mbIsStoped = false;

	void Time::Initialize()
	{
		// 시스템의 고성능 카운터는 주파수 형태로 값을 반환한다. 하지만 우리에게 필요한 것은 시간!
		// 따라서, 주파수(Hz)형태를 구해와서 역으로 1 Cycle 당 경과 시간을 구해둔다.
		// 이 값은 변하지 않는 값으로 앞으로 구해올 주파수에 이 값을 곱하면, 원하는 시간(초)를 구할 수 있게 됨.

		QueryPerformanceFrequency(&mCPUFrequency);			// CPU 초당 반복 되는 진동수를 얻어오는 함수 처음엔 0임.


		//QueryPerformanceCounter(&mPrevFrequency);			// 프로그램 시작 했을 때에 CPU의 클럭수 

		// ADD PART
		mdSecondPerCount = 1.0 / static_cast<double>(mCPUFrequency.QuadPart);


		// ADD PART END

		mHwnd = D3DApp::GetInstance().GetHwnd();
		StartTimeCounting();

	}

	void Time::Update()
	{
		//QueryPerformanceCounter(&mCurFrequency);
		//float differntFrequency
		//	= static_cast<float>(mCurFrequency.QuadPart - mPrevFrequency.QuadPart);
		//mDeltaTime = differntFrequency / mCPUFrequency.QuadPart;
		//mPrevFrequency.QuadPart = mCurFrequency.QuadPart;	// 갱신

		// ADD PART
		if (mbIsStoped)
		{
			mDeltaTime = 0.0f;
			return;
		}

		long long currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

		mllCurrTime = currTime;
		mDeltaTime = static_cast<float>((mllCurrTime - mllPrevTime) * mdSecondPerCount);
		mllPrevTime = mllCurrTime;
		if (mDeltaTime < 0.0f)
		{
			mDeltaTime = 0.0f;
		}
	}

	void Time::Render(HDC hdc)
	{
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;

		++frameCnt;

		// 1초 동안의 평균 프레임 수를 계산한다.

		if ((TotalTime() - timeElapsed) >= 1.0f)
		{
			wchar_t buffer[128] = {};
			float fps = (float)frameCnt;
			float mspf = 1000.0f / fps;

			swprintf_s(buffer, 128, L"FramePerSecond : %.3f, FrameTime : %.3f", fps, mspf);
			//size_t strLen = static_cast<size_t>(wcsnlen_s(buffer, 128));

			SetWindowText(mHwnd, buffer);

			// 다음 평균을 위해 초기화 한다.
			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}

	void Time::StartTimeCounting()
	{
		long long currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

		mllBaseTime = currTime;
		mllPrevTime = currTime;
		mllStopTime = 0;
		mllPausedTime = 0;
		mbIsStoped = false;
	}

	void Time::Reset()
	{
		long long currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
		mllBaseTime = 0;
		mllPrevTime = currTime;
		mllStopTime = 0;
		mllPausedTime = 0;
		mbIsStoped = false;
	}

	void Time::Stop()
	{
		if (!mbIsStoped)
		{
			long long currTime;
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

			mllStopTime = currTime;
			mbIsStoped = true;
		}
	}

	void Time::Resume()
	{
		long long startTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

		if (mbIsStoped)
		{
			mllPausedTime += (startTime - mllStopTime);

			mllPrevTime = startTime;
			mllStopTime = 0;
			mbIsStoped = false;
		}

	}

	float Time::TotalTime()
	{
		if (mbIsStoped)
		{
			return static_cast<float>(((mllStopTime - mllPausedTime) - mllBaseTime) * mdSecondPerCount);
		}
		else
		{
			return static_cast<float>(((mllCurrTime - mllPausedTime) - mllBaseTime) * mdSecondPerCount);
		}
	}

	void Time::SetScale(float scale)
	{
		mScale = scale;
	}

}