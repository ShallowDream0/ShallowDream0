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
PlantFixerPlus planter_fixer_plus;
PlantFixerPlus coffee_fixer;
TickRunner smart_blover;
TickRunner tick_planter;

bool IsZBExist(ZombieType type, int row, int abs, int hp) //判定僵尸
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

void SafeCard(int index, int row, int col, bool judge) //智能用卡
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
            for (int index : {8, 9, 10})
            {
                SafeCard(index, row, 6, true);
            }
        }
    }
}

//有丑关解法
void YCG()
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
                    if ((GetPlantIndex(3, 6, 14) > 0 || GetPlantIndex(3, 6, 63) > 0 || GetPlantIndex(4, 6, 14) > 0 || GetPlantIndex(4, 6, 63) > 0) && (!GetZombieTypeList()[BUNGEE_ZOMBIE] || (wave != 9 && wave != 19)) && IsUsable(COFFEE_BEAN))
                    {
                        SetNowTime(); 
                        ice_filler.coffee();
                    }

                    else if (IsUsable(JALAPENO) && IsUsable(CHERRY_BOMB))
                    {
                        SetTime(60, wave);
                        Card(CHERRY_BOMB, {{2, 8}, {1, 8}, {2, 7}, {1, 7}});
                        Card(JALAPENO, {{5, 7}});
                    }
                });
            } });
    }

    for (auto wave : {9, 19, 20})
    {
        SetTime(500, wave);
        pao_operator.recoverPao({{2, 9}, {5, 9}});
    }
}

//无丑关解法
void WCG()
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

                        else if (IsUsable(JALAPENO) && IsUsable(CHERRY_BOMB))
                        {
                            SetTime(60, wave);
                            Card(CHERRY_BOMB, {{2, 9}, {1, 9}, {2, 8}, {1, 8}, {2, 7}, {1, 7}});
                            Card(JALAPENO, {{5, 7}});
                        }
                    });
                } });
    }

    for (auto wave : {9, 19, 20})
    {
        SetTime(500, wave);
        pao_operator.recoverPao({{2, 9}, {5, 9}});
        SetTime(1500, wave);
        InsertOperation([=]()
                        {
            if (IsHYExist(1, 600) && IsHYExist(6, 600))
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 8}, {5, 8}});
            }

            else if (IsHYExist(6, 600))
            {
                SetNowTime();
                pao_operator.recoverPao({{5, 8}});
            }

            else if (IsHYExist(1, 600))
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 8}});
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
                                    ice_filler.coffee();
                                }

                                else if (IsUsable(JALAPENO) && IsUsable(CHERRY_BOMB))
                                {
                                    SetTime(60, wave);
                                    Card(CHERRY_BOMB, {{2, 8}, {1, 8}, {2, 7}, {1, 7}});
                                    Card(JALAPENO, {{5, 7}});
                                }
                                });
                }
                } });
    };
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

    //八倍速冲关
    SetGameSpeed(8);

    //手动设置游戏速度
    GameSpeed();

    ice_filler.start({{3, 6}, {4, 6}});

    ////智能修补位于{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 7}, {5, 7}的曾哥，血量阈值为90，不使用模仿种子
    LILY_PAD_fixer.start(LILY_PAD, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 6}, {5, 6}}, 0.01, false);

    //帧运行按血量放垫材
    C_a_user.pushFunc(SmartCard1);

    //智能修补位于{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 7}, {5, 7}的曾哥，血量阈值为90，不使用模仿种子
    GLOOM_SHROOM_fixer.start(GLOOM_SHROOM, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 6}, {5, 6}}, 0.01, false);

    //智能修补位于{1, 6}, {6, 6}的大喷，血量阈值为90，不使用模仿种子
    FUME_SHROOM_fixer.start(FUME_SHROOM, {{1, 5}, {6, 5}}, 0.01, false);

    InsertOperation([=]()
                    { smart_blover.pushFunc(SanYeCao1); });

    coffee_fixer.start(COFFEE_BEAN, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 6}, {5, 6}, {1, 5}, {6, 5}}, 0.01, false);

    //智能修补南瓜
    SetTime(-599, 1);
    plant_fixer.start(PUMPKIN, {{1, 1}, {2, 1}, {5, 1}, {6, 1}, {5, 2}, {5, 3}, {5, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {4, 9}}, 2000);

    if (GetZombieTypeList()[GIGA_GARGANTUAR] && !GetZombieTypeList()[BALLOON_ZOMBIE] && !GetZombieTypeList()[ZOMBONI])
    {
        SelectCards({
            ICE_SHROOM,
            M_ICE_SHROOM,
            COFFEE_BEAN,
            CHERRY_BOMB,
            JALAPENO,
            LILY_PAD,
            PUMPKIN,
            PUFF_SHROOM,
            SCAREDY_SHROOM,
            FUME_SHROOM,
        });
    }

    else if (GetZombieTypeList()[GIGA_GARGANTUAR] && GetZombieTypeList()[BALLOON_ZOMBIE] && GetZombieTypeList()[ZOMBONI])
    {
        SelectCards({
            ICE_SHROOM,
            M_ICE_SHROOM,
            COFFEE_BEAN,
            CHERRY_BOMB,
            JALAPENO,
            GLOOM_SHROOM,
            PUMPKIN,
            FUME_SHROOM,
            BLOVER,
            LILY_PAD,
        });
    }
    else if (GetZombieTypeList()[GIGA_GARGANTUAR] && !GetZombieTypeList()[BALLOON_ZOMBIE])
    {
        SelectCards({ICE_SHROOM,
                     M_ICE_SHROOM,
                     COFFEE_BEAN,
                     CHERRY_BOMB,
                     JALAPENO,
                     GLOOM_SHROOM,
                     PUMPKIN,
                     PUFF_SHROOM,
                     SUN_SHROOM,
                     FUME_SHROOM});
    }
    else if (GetZombieTypeList()[GIGA_GARGANTUAR])
    {
        SelectCards({
            ICE_SHROOM,
            M_ICE_SHROOM,
            COFFEE_BEAN,
            CHERRY_BOMB,
            JALAPENO,
            GLOOM_SHROOM,
            PUMPKIN,
            PUFF_SHROOM,
            FUME_SHROOM,
            BLOVER,
        });
    }

    else
    {
        SelectCards({
            ICE_SHROOM,
            M_ICE_SHROOM,
            COFFEE_BEAN,
            CHERRY_BOMB,
            JALAPENO,
            GLOOM_SHROOM,
            PUMPKIN,
            LILY_PAD,
            FUME_SHROOM,
            BLOVER,
        });
    }
    if (GetZombieTypeList()[JACK_IN_THE_BOX_ZOMBIE])
    {
        YCG();
    }
    else
    {
        WCG();
    }
    FlagWave();
}