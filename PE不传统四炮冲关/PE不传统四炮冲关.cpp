#include <avz.h>
#include <shallowdream.h>
#include <PlantFixerPlus.h>
#include <avz_more.h>
#include <avz_more_autoplay.h>
#include <TickPlanter.h>

using namespace AvZ;
using namespace cresc;
bool safe = true;
TickRunner C_a_user;
PlantFixerPlus GLOOM_SHROOM_fixer;
PlantFixerPlus FUME_SHROOM_fixer;
PlantFixerPlus LILY_PAD_fixer;
PlantFixerPlus coffee_fixer;
TickRunner smart_blover;
TickRunner fixer_manager;
TickRunner fixer_manager2;
TickRunner PUMPKIN_fixer;

void ManageFixer(PlantFixerPlus &fixer)
{
    struct TimeRange
    {
        int wave;
        int time_lower_bound;
        int time_upper_bound;
    };
    // 启用自动修补的时间段
    const std::vector<TimeRange> enable_time_ranges = {{1, -200, 2500}, {2, -200, 2500}, {3, -200, 2500}, {4, -200, 2500}, {5, -200, 2500}, {6, -200, 2500}, {7, -200, 2500}, {8, -200, 2500}, {9, -200, 0}, {10, 401, 2500}, {11, -200, 2500}, {12, -200, 2500}, {13, -200, 2500}, {14, -200, 2500}, {15, -200, 2500}, {16, -200, 2500}, {17, -200, 2500}, {18, -200, 2500}, {19, -200, 0}, {20, 401, 2500}};

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

void ManageFixer2(PlantFixerPlus &fixer)
{
    struct TimeRange
    {
        int wave;
        int time_lower_bound;
        int time_upper_bound;
    };
    // 启用自动修补的时间段
    const std::vector<TimeRange> enable_time_ranges = {{1, -200, 2500}, {2, -200, 2500}, {3, -200, 2500}, {4, -200, 2500}, {5, -200, 2500}, {6, -200, 2500}, {7, -200, 2500}, {8, -200, 2500}, {9, -200, 0}, {10, 401, 2500}, {11, -200, 2500}, {12, -200, 2500}, {13, -200, 2500}, {14, -200, 2500}, {15, -200, 2500}, {16, -200, 2500}, {17, -200, 2500}, {18, -200, 2500}, {19, -200, 0}, {20, 401, 2500}};

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

//检测僵尸状态，type表示僵尸种类，row表示僵尸出现的行数，abs表示僵尸出现的纵坐标（六行场地为 列数 * 80， 五行场地为 列数 * 100），hp表示僵尸剩余血量
bool IsZBExist(ZombieType type, int row, int abs, int hp)
{
    bool result = false;
    auto zombie = GetMainObject()->zombieArray();
    for (int index = 0; index < GetMainObject()->zombieTotal(); ++index)
    {
        if (zombie[index].type() == type && zombie[index].isExist() && !zombie[index].isDead() && zombie[index].row() == row - 1 && zombie[index].abscissa() < abs && zombie[index].hp() >= hp && zombie[index].state() != 70)
        {
            result = true;
        }
    }
    return result;
}

// 智能用卡，当没有冰车，且当满足条件时种下植物
// index表示植物在卡槽中的位置，范围[1, 10]
// row表示需要种下的行数
// col表示需要种在的列数
// judge默认为true
void SafeCard(int index, int row, int col, bool judge)
{

    if (IsZBExist(ZOMBONI, row, 80 * col, 0))
    {
        safe = false;
    }
    else
    {
        safe = true;
    }

    if (judge)
    {
        if (safe)
        {
            auto seed = GetMainObject()->seedArray();
            if (seed[index - 1].isUsable())
            {
                if (GetPlantIndex(row, col) == -1)
                {
                    SetInsertOperation(false);
                    Card(index, row, col);
                    SetInsertOperation(true);
                }
            }
        }
    }
    else
    {
        auto seed = GetMainObject()->seedArray();
        if (seed[index - 1].isUsable())
        {
            if (GetPlantIndex(row, col) == -1)
            {
                SetInsertOperation(false);
                Card(index, row, col);
                SetInsertOperation(true);
            }
        }
    }
}

void SmartCard1() //依据血量放垫材
{
    for (int row : {1, 6})
    {
        if (IsZBExist(GIGA_GARGANTUAR, row, 500, 200) || IsZBExist(FOOTBALL_ZOMBIE, row, 500, 0))
        {
            for (int index : {7, 8, 9, 10})
            {
                SafeCard(index, row, 6, true);
            }
        }
    }
}

void YCGSolve()
{
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19})
    {
        SetTime(1, wave);
        InsertOperation([=]()
                        {
            int c = CountReadyPao();
            if (c > 2)
            {
                SetNowTime();
                pao_operator.pao({{2, 9}, {5, 9}});
            }

            else if (c == 2)
            {
                if (IsUsable(JALAPENO) && IsUsable(CHERRY_BOMB))
                {
                    SetNowTime();
                    pao_operator.pao({{2, 9}});
                    SetTime(60, wave);
                    Card({{JALAPENO, 5, 7}});
                }

                else 
                {
                    SetNowTime();
                    pao_operator.pao({{2, 9}, {5, 9}});
                }
            }

            else if (c == 1)
            {
                if (IsUsable(CHERRY_BOMB) && !IsUsable(JALAPENO))
                {
                    SetNowTime();
                    pao_operator.pao({{2, 9}});
                    SetTime(60, wave);
                    Card({{CHERRY_BOMB, 5, 9}});
                }

                else
                {
                    SetNowTime();
                    InsertOperation([=]()
                    {
                        if ((GetPlantIndex(3, 6, 14) > 0 || GetPlantIndex(3, 6, 63) > 0 || GetPlantIndex(4, 6, 14) > 0 || GetPlantIndex(4, 6, 63) > 0) && (!GetZombieTypeList()[BUNGEE_ZOMBIE] || (wave != 9 && wave != 19)) && IsUsable(COFFEE_BEAN))
                        {
                            SetNowTime(); 
                            ice_filler.coffee();
                        }
                    });
                }
            }

            else
            {
                SetNowTime();        
                InsertOperation([=]()
                {
                    if ((GetPlantIndex(3, 6, 14) > 0 || GetPlantIndex(3, 6, 63) > 0 || GetPlantIndex(4, 6, 14) > 0 || GetPlantIndex(4, 6, 63) > 0) && (!GetZombieTypeList()[BUNGEE_ZOMBIE] || (wave != 9 && wave != 19)) && IsUsable(COFFEE_BEAN))
                    {
                        SetNowTime(); 
                        ice_filler.coffee();
                    }
                });
            } });
    }
}

//无丑关解法
void WCGSolve()
{
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19})
    {
        SetTime(1, wave);
        InsertOperation([=]()
                        {
            int c = CountReadyPao();
            if (c >= 2)
            {
                SetNowTime();
                pao_operator.pao({{2, 9}, {5, 9}});
            }

            else
            {
                SetNowTime();
                InsertOperation([=]()
                {
                    if ((GetPlantIndex(3, 6, 14) > 0 || GetPlantIndex(3, 6, 63) > 0 || GetPlantIndex(4, 6, 14) > 0 || GetPlantIndex(4, 6, 63) > 0) && IsUsable(COFFEE_BEAN) && (wave != 9 && wave != 19))
                    {
                        SetNowTime();
                        ice_filler.coffee();
                    }
                });
            } });
    }
}

//统一旗帜波
void FlagWave()
{
    for (auto wave : {10, 20})
    {
        SetTime(1, wave);
        InsertOperation([=]()
                        {
            if (GetZombieTypeList()[BUNGEE_ZOMBIE])
            {
                SetTime(400 - 298, wave);
                ice_filler.coffee();
            }

            else if (GetZombieTypeList()[JACK_IN_THE_BOX_ZOMBIE])
            {
                SetTime(1, wave);
                InsertOperation([=]()
                {
                    int c = CountReadyPao();
                    if (c > 2)
                    {
                        SetNowTime();
                        pao_operator.pao({{2, 9}, {5, 9}});
                    }

                    else if (c == 2)
                    {
                        if (IsUsable(JALAPENO) && IsUsable(CHERRY_BOMB))
                        {
                            SetNowTime();
                            pao_operator.pao({{2, 9}});
                            SetTime(300, wave);
                            Card({{JALAPENO, 5, 7}});
                        }

                        else 
                        {
                            SetNowTime();
                            pao_operator.pao({{2, 9}, {5, 9}});
                        }
                    }

                    else if (c == 1)
                    {
                        if (IsUsable(CHERRY_BOMB) && !IsUsable(JALAPENO))
                        {
                            SetNowTime();
                            pao_operator.pao({{2, 9}});
                            SetTime(300, wave);
                            Card({{CHERRY_BOMB, 4, 9}});
                        }

                        else
                        {
                            SetNowTime();
                            InsertOperation([=]()
                            {
                                if ((GetPlantIndex(3, 6, 14) > 0 || GetPlantIndex(3, 6, 63) > 0 || GetPlantIndex(4, 6, 14) > 0 || GetPlantIndex(4, 6, 63) > 0) && IsUsable(COFFEE_BEAN))
                                {
                                    SetTime(400 - 298, wave); 
                                    ice_filler.coffee();
                                }
                            });
                        }
                    }

                    else
                    {
                        SetNowTime();        
                        InsertOperation([=]()
                        {
                            if ((GetPlantIndex(3, 6, 14) > 0 || GetPlantIndex(3, 6, 63) > 0 || GetPlantIndex(4, 6, 14) > 0 || GetPlantIndex(4, 6, 63) > 0) && IsUsable(COFFEE_BEAN))
                            {
                                SetTime(400 - 298, wave);
                                ice_filler.coffee();
                            }
                        });
                    } });
            }

            else
            {
                int c = CountReadyPao();
                if (c >= 2)
                {
                    SetNowTime();
                    pao_operator.pao({{2, 9}, {5, 9}});
                }
                else
                {
                    SetNowTime();        
                    InsertOperation([=]()
                    {
                        if ((GetPlantIndex(3, 6, 14) > 0 || GetPlantIndex(3, 6, 63) > 0 || GetPlantIndex(4, 6, 14) > 0 || GetPlantIndex(4, 6, 63) > 0) && IsUsable(COFFEE_BEAN))
                        {
                            SetTime(400 - 298, wave);
                            ice_filler.coffee();
                        }
                    });
                } 
            } });
    }
}

void SanYeCao1()
{
    auto zombie_array = GetMainObject()->zombieArray();
    auto seed_array = GetMainObject()->seedArray();
    bool is_clover_usable = seed_array[GetSeedIndex(27)].isUsable();
    if (is_clover_usable)
    {
        for (int index = 0; index < GetMainObject()->zombieTotal(); ++index)
        {
            if (zombie_array[index].isExist() && zombie_array[index].type() == QQ_16 && !zombie_array[index].isDead() && zombie_array[index].abscissa() <= 2 * 80)
            {
                {
                    //确保操作成功插入进去
                    InsertGuard ig(false);
                    //使用三叶草，坐标可改
                    Card(SYC_27, {{2, 7}, {5, 7}, {1, 6}, {6, 6}});
                }
                break;
            }
        }
    }
}

void Script()
{
    //连续运行(摁0停止运行，但是再次摁0不会重新启动)
    OpenMultipleEffective('0', AvZ::MAIN_UI_OR_FIGHT_UI);

    //自动获取炮序
    pao_operator.autoGetPaoList();

    //设置不检测卡片中是否有曾哥和荷叶
    GLOOM_SHROOM_fixer.setIsCheckCards(false);
    LILY_PAD_fixer.setIsCheckCards(false);

    //跳帧
    //直接跳过整个游戏
    /*SkipTick([=]()
             { return true; });
             */

    SetGameSpeed(2);

    //手动设置游戏速度
    GameSpeed();

    //在（3，6）和（4, 6）存冰（固定的两个存冰位）
    ice_filler.start({{3, 6}, {4, 6}});

    ////智能修补位于{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 7}, {5, 7}的曾哥，血量阈值为90，不使用模仿种子
    LILY_PAD_fixer.start(LILY_PAD, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}}, 0.01, false);

    //帧运行按血量放垫材
    C_a_user.pushFunc(SmartCard1);

    //智能修补位于{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 7}, {5, 7}的曾哥，血量阈值为90，不使用模仿种子
    GLOOM_SHROOM_fixer.start(GLOOM_SHROOM, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 6}, {5, 6}, {5, 5}, {2, 5}}, 0.01, false);

    //智能修补位于{1, 6}, {6, 6}的大喷，血量阈值为90，不使用模仿种子
    FUME_SHROOM_fixer.start(FUME_SHROOM, {{1, 5}, {6, 5}}, 0.01, false);

    plant_fixer_plus.start(PUMPKIN, {{2, 6}, {5, 6}}, 1000, false);

    InsertOperation([=]()
                    { smart_blover.pushFunc(SanYeCao1); });

    coffee_fixer.start(COFFEE_BEAN, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 6}, {5, 6}, {1, 5}, {6, 5}}, 0.01, false);

    //智能修补南瓜
    SetTime(-599, 1);
    plant_fixer.start(PUMPKIN, {{1, 1}, {2, 1}, {5, 1}, {6, 1}, {5, 2}, {5, 3}, {5, 4}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {4, 6}, {4, 7}, {4, 8}, {4, 9}}, 2000);

    //有红眼，没丑没气球，不用樱辣，要带垫材，不用带曾套件
    if (GetZombieTypeList()[GIGA_GARGANTUAR] && !GetZombieTypeList()[JACK_IN_THE_BOX_ZOMBIE] && !GetZombieTypeList()[BALLOON_ZOMBIE])
    {
        SelectCards({ICE_SHROOM,
                     M_ICE_SHROOM,
                     COFFEE_BEAN,
                     FUME_SHROOM,
                     LILY_PAD,
                     PUMPKIN,
                     PUFF_SHROOM,
                     SCAREDY_SHROOM,
                     SUN_SHROOM,
                     SUNFLOWER});
    }

    //有红眼，小丑和气球，带樱辣、三叶草和曾套件
    else if (GetZombieTypeList()[GIGA_GARGANTUAR] && GetZombieTypeList()[JACK_IN_THE_BOX_ZOMBIE] && GetZombieTypeList()[BALLOON_ZOMBIE])
    {
        SelectCards({ICE_SHROOM,
                     M_ICE_SHROOM,
                     COFFEE_BEAN,
                     CHERRY_BOMB,
                     JALAPENO,
                     PUMPKIN,
                     GLOOM_SHROOM,
                     FUME_SHROOM,
                     BLOVER,
                     LILY_PAD});
    }

    //有红眼，有小丑，没气球，不带三叶草，带樱辣垫材和曾套件
    else if (GetZombieTypeList()[GIGA_GARGANTUAR] && GetZombieTypeList()[JACK_IN_THE_BOX_ZOMBIE] && !GetZombieTypeList()[BALLOON_ZOMBIE])
    {
        SelectCards({ICE_SHROOM,
                     M_ICE_SHROOM,
                     COFFEE_BEAN,
                     CHERRY_BOMB,
                     JALAPENO,
                     PUMPKIN,
                     PUFF_SHROOM,
                     FUME_SHROOM,
                     LILY_PAD,
                     GLOOM_SHROOM});
    }

    //有红眼，有气球，没小丑，不带樱辣，带垫材和三叶草和曾套件
    else if (GetZombieTypeList()[GIGA_GARGANTUAR] && GetZombieTypeList()[BALLOON_ZOMBIE] && !GetZombieTypeList()[JACK_IN_THE_BOX_ZOMBIE])
    {
        SelectCards({
            ICE_SHROOM,
            M_ICE_SHROOM,
            COFFEE_BEAN,
            GLOOM_SHROOM,
            PUMPKIN,
            BLOVER,
            PUFF_SHROOM,
            SUN_SHROOM,
            SCAREDY_SHROOM,
            FUME_SHROOM,
        });
    }

    //如果没有红眼，有小丑，没气球，带垫材，樱辣，三曾套件
    else if (GetZombieTypeList()[JACK_IN_THE_BOX_ZOMBIE] && !GetZombieTypeList()[BALLOON_ZOMBIE])
    {
        SelectCards({ICE_SHROOM,
                     M_ICE_SHROOM,
                     COFFEE_BEAN,
                     CHERRY_BOMB,
                     JALAPENO,
                     PUMPKIN,
                     GLOOM_SHROOM,
                     LILY_PAD,
                     FUME_SHROOM,
                     PUFF_SHROOM});
    }

    //如果没红眼，没小丑，不用带曾套件，不用带樱辣，带三叶草和垫材
    else if (!TypeExist(JACK_IN_THE_BOX_ZOMBIE))
    {
        SelectCards({ICE_SHROOM,
                     M_ICE_SHROOM,
                     COFFEE_BEAN,
                     BLOVER,
                     PUMPKIN,
                     FUME_SHROOM,
                     PUFF_SHROOM,
                     SUN_SHROOM,
                     SUNFLOWER,
                     SCAREDY_SHROOM});
    }

    else
    {
        SelectCards({ICE_SHROOM,
                     M_ICE_SHROOM,
                     COFFEE_BEAN,
                     CHERRY_BOMB,
                     JALAPENO,
                     PUMPKIN,
                     FUME_SHROOM,
                     BLOVER,
                     GLOOM_SHROOM,
                     LILY_PAD});
    }

    if (GetZombieTypeList()[JACK_IN_THE_BOX_ZOMBIE])
    {
        YCGSolve();
    }
    else
    {
        WCGSolve();
    }
    FlagWave();

    //红眼关中场一对炮杀伤红眼，方便收尾
    if (GetZombieTypeList()[GIGA_GARGANTUAR])
    {
        for (int wave : {9, 19, 20})
        {
            SetTime(500, wave);
            pao_operator.recoverPao({{2, 9}, {5, 9}});
        }
    }

    //无冰车红眼在（2,6）和（5,6）偷南瓜
    if (!GetZombieTypeList()[GIGA_GARGANTUAR] && !GetZombieTypeList()[ZOMBONI])
    {
        SetTime(-599, 1);
        plant_fixer_plus.goOn();
    }

    //若有红眼或冰车，铲除偷的南瓜
    if (GetZombieTypeList()[GIGA_GARGANTUAR] || GetZombieTypeList()[ZOMBONI])
    {
        SetTime(-599, 1);
        plant_fixer_plus.pause();
        InsertOperation([=]()
                        {
            if ((GetPlantIndex(2, 6, 30) > 0))
            {
                SetNowTime(); 
                Shovel(2, 6, true);
            }
            if ((GetPlantIndex(5, 6, 30) > 0))
            {
                SetNowTime();
                Shovel(5, 6, true);
            } });
    }

    //智能控制修补时机，在收尾波不修补大喷
    InsertOperation([=]()
                    { fixer_manager.pushFunc([]()
                                             { ManageFixer(FUME_SHROOM_fixer); }); });
    //智能控制修补时机，在收尾波不修补曾哥
    InsertOperation([=]()
                    { fixer_manager2.pushFunc([]()
                                              { ManageFixer2(GLOOM_SHROOM_fixer); }); });
}