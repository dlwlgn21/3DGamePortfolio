#pragma once
#include "D3DApp.h"

namespace jh
{
	class Time final
	{
	public:
		static void Initialize();
		static void Update();
		static void Render(HDC hdc);

		static void StartTimeCounting();
		static void Reset();
		static void Stop();
		static void Resume();
		static float TotalTime();
		static void SetScale(float scale);


		__forceinline static float DeltaTime() { return mDeltaTime * mScale; }
		__forceinline static bool IsStoped() { return mbIsStoped; }

	private:
		// CPU 고유 진동수 ( 1초당 몇 번 )
		static LARGE_INTEGER mCPUFrequency;		// CPU 진동수
		static HWND mHwnd;
		static float mDeltaTime;

		// 추가된 파트
		static float mScale;
		static double mdSecondPerCount;


		static long long mllBaseTime;		// 기준 시간으로 게임이 처음 구동되었을 때의 시간
		static long long mllPausedTime;		// 멈춰있었던 시간. 즉, Stop()을 호출한 후, Resume()을 호출할 때까지 걸린 시간
		static long long mllStopTime;		// 멈춘 시간. 즉, Stop()을 호출했을 때의 시간
		static long long mllPrevTime;		// 이전 시간. 즉, 이전 Tick()의 시간.
		static long long mllCurrTime;		// 현재 시간. 즉, 현재 Tick()의 시간.

		static bool mbIsStoped;
	};
}
