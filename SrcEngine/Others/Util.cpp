#include "Util.h"
#include "Util.h"
#include <random>
#include <GameManager.h>
#include <SpWindow.h>
#include <chrono>
#include <ctime>
#include <iomanip> 

std::random_device rnd;
std::mt19937 mt(rnd());

std::wstring Util::StrToWStr(const std::string& str, uint32_t page)
{
    // SJIS -> wstring
    int32_t iBufferSize = MultiByteToWideChar(page, 0, str.c_str(), -1, (wchar_t*)NULL, 0);

    // バッファの取得
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS -> wstring
    MultiByteToWideChar(page, 0, str.c_str(), -1, cpUCS2, iBufferSize);

    // stringの生成
    std::wstring ret(cpUCS2, cpUCS2 + iBufferSize - 1);

    // バッファの破棄
    delete[] cpUCS2;

    // 変換結果を返す
    return ret;
}

Float2 DLLExport Util::GetWinHalfSize()
{
    return Float2(static_cast<float>(GetSpWindow()->width) / 2, static_cast<float>(GetSpWindow()->height) / 2);
}

Float2 Util::GetWinSize()
{
    if (GameManager::sShowDebug)
    {

    }
    return Float2(static_cast<float>(GetSpWindow()->width), 
        static_cast<float>(GetSpWindow()->height));
}

int32_t Util::GetWinWidth()
{
    return static_cast<int32_t>(GetWinSize().x);
}

int32_t Util::GetWinHeight()
{
    return static_cast<int32_t>(GetWinSize().y);
}

bool Util::Chance(int32_t percentage)
{
    return (Util::RNG(0, 99, true) < percentage);
}

int32_t Util::RNG(int32_t min, int32_t max, bool preciseMode)
{
    if (!preciseMode) {
        return (mt() % (max + 1 - min) + min);
    }

    std::uniform_int_distribution<> rbrnd(min, max);

    return rbrnd(mt);
}

std::string Util::GetCurrentTimerString()
{
    // 現在のシステム時間を取得
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // localtime_s を使用してローカルタイムに変換
    std::tm localTime;
    localtime_s(&localTime, &currentTime);

    // std::put_time を使用して文字列に変換
    std::stringstream ss;
    ss << std::put_time(&localTime, "%Y_%m_%d_%H_%M_%S");
    return ss.str();
}