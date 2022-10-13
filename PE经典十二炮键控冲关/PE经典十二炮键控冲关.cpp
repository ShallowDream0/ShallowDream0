#include <avz.h>
#include <shallowdream.h>
#include "dance_cheat.h"

using namespace AvZ;
TickRunner smart_blover;

//智能收尾
void Endwave()
{
    for (auto wave : {9, 19, 20})
    {
        SetTime(341 - 3 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(500);
        InsertOperation([=]()
                        {
            if (IsHYExist(1, 3500) || IsHYExist(2, 3500) || IsHYExist(5, 3500) || IsHYExist(6, 3500)) //有无本波红眼
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 9}, {5, 9}});
                Delay(600);
                pao_operator.recoverPao({{2, 9}, {5, 9}});
            }

            else if (IsHYExist(1, 1800) || IsHYExist(2, 1800) || IsHYExist(5, 1800) || IsHYExist(6, 1800) && GetMainObject()->refreshCountdown() > 200 && wave == GetMainObject()->wave())
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 9}, {5, 9}});
                Delay(380);
                InsertOperation([=]()
                {
                    if ((IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsMSExist(6, 0)) && wave == GetMainObject()->wave() && GetMainObject()->refreshCountdown() > 200)
                    {
                        SetNowTime();
                        pao_operator.recoverPao({{2, 9}, {5, 9}});
                    }
                });
            }

            else if (IsBYExist(1, 500) || IsBYExist(2, 500) || IsBYExist(5, 500) || IsBYExist(6, 500))
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 9}, {5, 9}});
                Delay(380);
                InsertOperation([=]()
                {
                    if ((IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsMSExist(6, 0)) && wave == GetMainObject()->wave() && GetMainObject()->refreshCountdown() > 200)
                    {
                        SetNowTime();
                        pao_operator.recoverPao({{2, 9}, {5, 9}});
                    }
                });
            }

            else if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || 
                     IsCGExist(1, 0) || IsCGExist(2, 0) || IsCGExist(5, 0) || IsCGExist(6, 0))  //撑杆和慢速僵尸是否存在
                    {
                        SetNowTime();
                        pao_operator.recoverPao({{2, 9}, {5, 9}});
                        Delay(380);
                        InsertOperation([=]()
                        {
                        if ((IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsMSExist(6, 0)) && wave == GetMainObject()->wave() && GetMainObject()->refreshCountdown() > 200)
                        {
                            SetNowTime();
                            pao_operator.recoverPao({{2, 9}, {5, 9}});
                        }
                        });
                    } });
    }
}

void Script()
{
    SelectCards({
        CHERRY_BOMB,   //樱桃
        BLOVER,        //三叶草
        SQUASH,        //倭瓜
        COFFEE_BEAN,   //咖啡豆
        DOOM_SHROOM,   //毁灭菇
        M_CHERRY_BOMB, //模仿冰
        LILY_PAD,      //荷叶
        FLOWER_POT,    //花盆
        FUME_SHROOM,   //大喷菇
        COB_CANNON,    //玉米加农炮
    });

    pao_operator.autoGetPaoList();

    //手动设置游戏速度
    KeyConnect('1', [=]()
               { SetGameSpeed(1); });
    KeyConnect('2', [=]()
               { SetGameSpeed(2); });
    KeyConnect('3', [=]()
               { SetGameSpeed(3); });
    KeyConnect('4', [=]()
               { SetGameSpeed(4); });
    KeyConnect('5', [=]()
               { SetGameSpeed(5); });
    KeyConnect('6', [=]()
               { SetGameSpeed(6); });
    KeyConnect('7', [=]()
               { SetGameSpeed(7); });
    KeyConnect('8', [=]()
               { SetGameSpeed(8); });

    SetGameSpeed(8);
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        SetTime(341 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
    }

    SetTime(341 - 3 - 373, 10);
    pao_operator.pao({{2, 9}, {5, 9}});

    SetTime(-128, 20);
    pao_operator.pao({{4, 7.6}});

    Endwave();

    //智能三叶草，pushfunc是not in queue属性，因此必须在前面写insertoperation
    InsertOperation([=]()
                    { smart_blover.pushFunc(SanYeCao); });

    //处理刷新延迟
    DealYanchi();

    //普僵三人组加速
    DanceCheat(FAST);

    //连续运行(摁0停止运行，但是再次摁0不会重新启动)
    OpenMultipleEffective('0', AvZ::MAIN_UI_OR_FIGHT_UI);

    for (auto wave : {10})
    {
        SetTime(601, wave);
        InsertOperation([=]()
                        {
                        auto seed= GetMainObject()->seedArray();  //种子内存
                        auto YT_seed=GetMainObject()->seedArray()+GetSeedIndex(CHERRY_BOMB);
                            auto zombie = GetMainObject()->zombieArray();
                            if (GetMainObject()->refreshCountdown() >601 - 200 + 1 && wave == GetMainObject()->wave())
                            {
                                SetNowTime();
                                if (YT_seed[CHERRY_BOMB].isUsable())//如果樱桃可以用
                                {
                                    Card({{CHERRY_BOMB, 2, 9}});
                                }
                            } });
    }
}