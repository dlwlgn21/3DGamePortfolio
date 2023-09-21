// 2DGamePortfolio.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <iostream>
#include <memory>
#include <windows.h>
#include "framework.h"
#include "2DGamePortfolio.h"
#include "D3DApp.h"

using namespace std;

// main()은 앱을 초기화하고 실행시키는 기능만 합니다.
// 콘솔창이 있으면 디버깅에 편리합니다.
// 디버깅할 때 애매한 값들을 cout으로 출력해서 확인해보세요.
int main() {
    jh::D3DApp& instance = jh::D3DApp::GetInstance();
    if (!instance.Initialize()) 
    {
        assert(false);
        return -1;
    }

    return instance.Run();
}