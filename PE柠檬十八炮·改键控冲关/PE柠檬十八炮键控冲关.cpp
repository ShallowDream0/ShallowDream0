#include "avz.h"
#include <vector>
#include "PlantFixerPlus.h"
#include "dance_cheat.h"
#include <shallowdream.h>

using namespace AvZ;
PlantFixerPlus Cob_Cannon_fixer;
TickRunner fixer_manager;
TickRunner smart_blover;

void ManageFixer(PlantFixerPlus &fixer)
{
    struct TimeRange
    {
        int wave;
        int time_lower_bound;
        int time_upper_bound;
    };
    // 启用自动修补的时间段
    const std::vector<TimeRange> enable_time_ranges = {{1, -200, 601}, {2, -200, 601}, {3, -200, 601}, {4, -200, 601}, {5, -200, 601}, {6, -200, 601}, {7, -200, 601}, {8, -200, 601}, {10, -200, 601}, {11, -200, 601}, {12, -200, 601}, {13, -200, 601}, {14, -200, 601}, {15, -200, 601}, {16, -200, 601}, {17, -200, 601}, {18, -200, 601}};

    bool is_enable_time;
    int now_wave;

    is_enable_time = false;
    for (TimeRange each : enable_time_ranges)
    {
        now_wave = GetRunningWave();
        if (now_wave == each.wave && NowTime(now_wave) >= each.time_lower_bound && NowTime(now_wave) <= each.time_upper_bound)
        {
            is_enable_time = true;
            break;
        }
    }

    if (is_enable_time)
    {
        if (fixer.getStatus() != RUNNING)
        {
            {
                InsertGuard ig(false);
                fixer.goOn();
            }
        }
    }
    else
    {
        if (fixer.getStatus() == RUNNING)
        {
            {
                InsertGuard ig(false);
                fixer.pause();
            }
        }
    }
}

//智能收尾
void endwave(int time, int wave)
{
    SetTime(-95, wave);
    FindAndPao(pao_operator, {{2, 9}, {5, 9}});
    Delay(100);
    FindAndPao(pao_operator, {{5, 9}});
    SetTime(382, wave);
    InsertOperation([=]()
                    {
        if ((IsHYExist(1, 3000) || IsHYExist(2, 3000)) && (IsHYExist(5, 1500) || IsHYExist(6, 1500))) //上下半场都有本波红
        {
            SetNowTime();
            pao_operator.recoverPao({{2, 9}, {5, 9}});
            Delay(100);
            pao_operator.recoverPao({{5, 9}, {2, 9}});
            Delay(377);
            InsertOperation([=]()
            {
                if (GetZombieTypeList()[BUNGEE_ZOMBIE] && (IsHYExist(1, 0) || IsHYExist(2, 0) || IsHYExist(5, 0) || IsHYExist(6, 0)))
                {
                    Delay(600 - 377);
                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                }
                else if (IsHYExist(1, 0) || IsHYExist(2, 0) || IsHYExist(5, 0) || IsHYExist(6, 0))
                {
                    Delay(400 - 377);
                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                }
            });
            Delay(377);
            InsertOperation([=]()
            {
                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsMSExist(6, 0))
                {
                    SetNowTime();
                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                }
            });
        }

        else if (IsHYExist(1, 3000) || IsHYExist(2, 3000)) //只有上半场有本波红
        {
            SetNowTime();
            pao_operator.recoverPao({{2, 9}, {5, 9}});
            Delay(300);
            pao_operator.recoverPao({{2, 9}, {5, 9}});
            SetTime(1060, wave);
            InsertOperation([=]()
                            {
                                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0))//检测慢速僵尸是否存在
                                {
                                    SetNowTime();
                                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                                }
                            });
        }

        else if (IsHYExist(5, 1500) || IsHYExist(6, 1500)) //只有下半场有本波红
        {
            SetNowTime();
            pao_operator.recoverPao({{2, 9}, {5, 9}});
            Delay(100);
            pao_operator.recoverPao({{2, 9}, {5, 9}});
            SetTime(860, wave);
            InsertOperation([=]()
                            {
                                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0))//检测慢速僵尸是否存在
                                {
                                    SetNowTime();
                                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                                }
                            });
        }

        else if (IsHYExist(1, 1000) || IsHYExist(2, 1000)) //只有上半场有上波红（夹零）
        {
            SetNowTime();
            pao_operator.recoverPao({{2, 9}, {2, 9}});
            SetTime(760, wave);
            InsertOperation([=]()
                            {
                                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0))//检测慢速僵尸是否存在
                                {
                                    SetNowTime();
                                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                                }
                            });
            SetTime(1140, wave);
            InsertOperation([=]()
                            {
                                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0))//检测慢速僵尸是否存在
                                {
                                    setNowTime();
                                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                                }
                            });
        }

        else if (IsBYExist(1, 100) || IsBYExist(2, 100)) //无红有白
        {
            SetNowTime();
            pao_operator.recoverPao({{2, 9}, {5, 9}});
            SetTime(760, wave);
            InsertOperation([=]()
                            {
                                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0)) //检测慢速僵尸是否存在
                                {
                                    SetNowTime();
                                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                                }
                            });
            SetTime(1140, wave);
            InsertOperation([=]()
                            {
                                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0)) //检测慢速僵尸是否存在
                                {
                                    SetNowTime();
                                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                                }
                            });
        }

        else if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0)) //检测慢速僵尸是否存在
        {
            SetNowTime();
            pao_operator.recoverPao({{2, 9}, {5, 9}});
            SetTime(760, wave);
            InsertOperation([=]()
                            {
                                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0)) //检测慢速僵尸是否存在
                                {
                                    SetNowTime();
                                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                                }
                            });
            SetTime(1140, wave);
            InsertOperation([=]()
                            {
                                if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || IsMSExist(3, 0) || IsMSExist(4, 0)) //检测慢速僵尸是否存在
                                {
                                    SetNowTime();
                                    pao_operator.recoverPao({{2, 9}, {5, 9}});
                                }
                            });
        } });
}
void Script()
{
    //连续运行(摁0停止运行，但是再次摁0不会重新启动)
    OpenMultipleEffective('0', AvZ::MAIN_UI_OR_FIGHT_UI);

    //选卡（除了玉米加农炮套件、樱桃、荷叶、三叶草必带，其他可以替换）
    SelectCards({
        KERNEL_PULT,   //玉米投手
        COB_CANNON,    //玉米加农炮
        M_KERNEL_PULT, //模仿玉米投手
        DOOM_SHROOM,   //核弹
        LILY_PAD,      //荷叶
        ICE_SHROOM,    //寒冰菇
        CHERRY_BOMB,   //樱桃
        COFFEE_BEAN,   //咖啡豆
        PUMPKIN,       //南瓜头
        BLOVER,        //阳光菇
    });

    //八倍速冲关
    SetGameSpeed(8);

    //手动选择游戏速度
    GameSpeed();

    //智能三叶草，pushfunc是not in queue属性，因此必须在前面写insertoperation
    InsertOperation([=]()
                    { smart_blover.pushFunc(SanYeCao); });

    int GGnumber = 0;

    DanceCheat(FAST);

    //智能修补位于{1，1}，{2，1}，{3，1}，{4，1}的玉米加农炮，血量阈值为90，同时使用模仿种子
    Cob_Cannon_fixer.start(COB_CANNON, {{1, 1}, {2, 1}, {5, 1}, {6, 1}}, 0.3);

    //智能控制修补时机，在收尾波不修补炮
    InsertOperation([=]()
                    { fixer_manager.pushFunc([]()
                                             { ManageFixer(Cob_Cannon_fixer); }); });

    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8})
    {
        GGnumber = GGnumber + GetZombieNumber(GIGA_GARGANTUAR, wave);
        SetTime(-96, wave);
        InsertOperation([=]()
                        {
        if (GGnumber >= 50)
        {
            SetTime(-95, wave);
            FindAndPao(pao_operator, {{2, 9}, {5, 9}});
        }
        else
        {
            SetTime(-95, wave);
            FindAndPao(pao_operator, {{2, 9}, {5, 9}});
            Delay(100);
            FindAndPao(pao_operator, {{5, 9}});
        } });
    }

    SetTime(-199, 9);
    GGnumber = GGnumber + GetZombieNumber(GIGA_GARGANTUAR, 9);

    // P6循环，发三炮，两炮炸下
    for (auto wave : {10, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        GGnumber = GGnumber + GetZombieNumber(GIGA_GARGANTUAR, wave);
        SetTime(-96, wave);
        InsertOperation([=]()
                        {
        if (GGnumber >= 50)
        {
            if (GetZombieNumber(GIGA_GARGANTUAR, wave) || IsZombieExist(32, 5) || IsZombieExist(32, 6) || GetZombieTypeList()[POLE_VAULTING_ZOMBIE])
            {
                SetTime(-95, wave);
                FindAndPao(pao_operator, {{2, 9}, {5, 9}});
                Delay(100);
                FindAndPao(pao_operator, {{5, 9}});
            }
            else
            {
                SetTime(-95, wave);
                FindAndPao(pao_operator, {{2, 9}, {5, 9}});
            }
        }
        else
        {
            SetTime(-95, wave);
            FindAndPao(pao_operator, {{2, 9}, {5, 9}});
            Delay(100);
            FindAndPao(pao_operator, {{5, 9}});
        } });
    }

    //智能收尾（我也不知道能不能写for）
    endwave(-199, 9);
    endwave(-199, 19);
    endwave(-199, 20);

    //炮消珊瑚，注意，这里时机一定是-128，-150不能使用这个炸点，且必须炸4路，炸3路会漏
    SetTime(-128, 20);
    FindAndPao(pao_operator, {{4, 7.5875}});

    DealBungee();

    //每波检测刷新延迟，如果有延迟且樱桃可用，就放樱桃
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        SetTime(601, wave);
        InsertOperation([=]()
                        {
                        auto seed= GetMainObject()->seedArray();  //种子内存
                        auto YT_seed=GetMainObject()->seedArray() + GetSeedIndex(CHERRY_BOMB);
                        auto zombie = GetMainObject()->zombieArray();
                        //检测是否延迟刷新
                        if (GetMainObject()->refreshCountdown() >601 - 200 + 1 && wave == GetMainObject()->wave()) 
                        {
                        SetNowTime();
                        //检测樱桃是否可用
                        if (YT_seed[CHERRY_BOMB].isUsable())
                        {
                            //樱桃放在（2,9），因为大多数是中场可能延迟刷新，而中场我是选择炸5路，所以智能樱桃放2路
                            Card({{CHERRY_BOMB, 2, 9}});
                        }
                        } });
    }
}