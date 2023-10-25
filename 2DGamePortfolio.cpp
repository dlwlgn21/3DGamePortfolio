// 2DGamePortfolio.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <iostream>
#include <memory>
#include <windows.h>
#include "framework.h"
#include "2DGamePortfolio.h"
#include "D3DApp.h"

using namespace std;


int main() {
    jh::D3DApp& instance = jh::D3DApp::GetInstance();
    if (!instance.Initialize()) 
    {
        assert(false);
        return -1;
    }

    return instance.Run();
}