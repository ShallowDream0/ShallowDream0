#include "avz.h"
#include "shallowdream.h"
#include "PlantFixerPlus.h"
#include "dance_cheat.h"

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

//红眼出怪循环：PPD，D拦截
void HYXH()
{
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        SetTime(341 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(110);
        pao_operator.pao({{5, 7.8}});
    }
}

//其他出怪循环：PPD，D瞬杀（炸实）
void QTXH()
{
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        SetTime(341 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(100);
        pao_operator.pao({{5, 9}});
    }
}

//中场处理，若有红眼则PPND（五个运算量炸实），若无红眼则PPD+樱桃
void Half()
{
    if (GetZombieTypeList()[GIGA_GARGANTUAR])
    {
        SetTime(341 - 3 - 299, 10);
        Card(LILY_PAD, {{3, 9}, {4, 9}});
        Card(DOOM_SHROOM, {{3, 9}, {4, 9}});
        Card(COFFEE_BEAN, {{3, 9}, {4, 9}});
        SetTime(341 - 3 - 373, 10);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(110);
        pao_operator.pao({{5, 9}});
    }

    else
    {
        SetTime(341 - 3 - 373, 10);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(110);
        pao_operator.pao({{5, 7.8}});
        SetTime(401, 10);
        InsertOperation([=]()
                        {
                        auto seed= GetMainObject()->seedArray();  //种子内存
                        auto YT_seed=GetMainObject()->seedArray()+GetSeedIndex(CHERRY_BOMB);
                            auto zombie = GetMainObject()->zombieArray();
                            if (GetMainObject()->refreshCountdown() > 200 && 10 == GetMainObject()->wave()) {
                            SetNowTime();
                                if (YT_seed[CHERRY_BOMB].isUsable())//如果樱桃可以用
                                {
                                    Card({{CHERRY_BOMB, 2, 9}});
                                }
            } });
    }
}

//智能收尾函数
void EndWave()
{
    for (auto wave : {9, 19, 20})
    {
        SetTime(341 - 3 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(110);
        pao_operator.pao({{5, 7.8}});
        SetTime(453, wave);
        InsertOperation([=]()
                        {
            if (IsHYExist(1, 3500) || IsHYExist(2, 3500) || IsHYExist(5, 3500) || IsHYExist(6, 3500))//判断有无本波红
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 9}, {5, 9}});
                Delay(108);
                pao_operator.recoverPao({{5, 8}});
                Delay(377);
                pao_operator.recoverPao({{2, 9}, {5, 9}, {2, 9}});
            }

            else if (IsHYExist(1, 1500) || IsHYExist(2, 1500) || IsHYExist(5, 0) || IsHYExist(6, 0))//如果没有本波红，而且有上波红（夹零）
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 9}, {2, 9}, {5, 9}});
            }

            else if (IsBYExist(1, 0) || IsBYExist(2, 0) || IsBYExist(5, 0) || IsBYExist(6, 0))      //判断有无白眼
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 9}, {5, 9}});
                Delay(377);
                InsertOperation([=]()
                {
                    if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || 
                        IsCGExist(1, 0) || IsCGExist(2, 0) || IsCGExist(5, 0) || IsCGExist(6, 0))
                        {
                            SetNowTime();
                            pao_operator.recoverPao({{2, 9}, {5, 9}});
                        }
                });
            }

            else if (wave == GetMainObject()->wave() && GetMainObject()->refreshCountdown() > 200 )
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 9}, {5, 9}});
                Delay(377);
                InsertOperation([=]()
                {
                    if (IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsMSExist(6, 0) || 
                        IsCGExist(1, 0) || IsCGExist(2, 0) || IsCGExist(5, 0) || IsCGExist(6, 0))
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
        M_KERNEL_PULT,
        KERNEL_PULT,
        COB_CANNON,
        DOOM_SHROOM,
        LILY_PAD,
        COFFEE_BEAN,
        CHERRY_BOMB,
        SUN_SHROOM,
        SCAREDY_SHROOM,
        FLOWER_POT,
    });

    pao_operator.autoGetPaoList();

    //智能三叶草，pushfunc是not in queue属性，因此必须在前面写insertoperation
    InsertOperation([=]()
                    { smart_blover.pushFunc(SanYeCao); });

    //八倍速冲关
    SetGameSpeed(8);

    //手动选择游戏速度
    GameSpeed();

    //智能修补位于{1，1}，{2，1}，{3，1}，{4，1}的玉米加农炮，血量阈值为90，同时使用模仿种子
    Cob_Cannon_fixer.start(COB_CANNON, {{1, 1}, {2, 1}, {5, 1}, {6, 1}}, 0.3);

    //智能控制修补时机，在收尾波不修补炮
    InsertOperation([=]()
                    { fixer_manager.pushFunc([]()
                                             { ManageFixer(Cob_Cannon_fixer); }); });

    //跳舞秘籍
    DanceCheat(FAST);

    //连续运行(摁0停止运行，但是再次摁0不会重新启动)
    OpenMultipleEffective('0', AvZ::MAIN_UI_OR_FIGHT_UI);

    if (GetZombieTypeList()[GIGA_GARGANTUAR])
    {
        HYXH();
    }

    else
    {
        QTXH();
    }

    //智能中场
    Half();

    //智能收尾
    EndWave();

    //炮消珊瑚
    PXSH();
}
