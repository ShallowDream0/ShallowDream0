#include <avz.h>
#include <shallowdream.h>
#include <dance_cheat.h>
#include <avz_more.h>
bool safe = true;
using namespace AvZ;
TickRunner C_a_user;
AvZ::PaoOperator dixianpao;
AvZ::PaoOperator qianzhipao;
using namespace cresc;
TickRunner C_b_user;

//智能樱桃处理延迟
void SmartDoom()
{
    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        auto zombie_type = GetMainObject()->zombieTypeList();
        SetTime(1, wave);
        InsertOperation([=]()
                        {
        int GGCount_up = 0;
        int GGCount_down = 0;
        auto zombie = GetMainObject()->zombieArray();
        for (int index = 0; index < GetMainObject()->zombieTotal(); ++index)
        {
            if (zombie[index].type() == GIGA_GARGANTUAR && zombie[index].isExist() && !zombie[index].isDead() && (zombie[index].row() == 0 || zombie[index].row() == 1 || zombie[index].row() == 2)) {
                GGCount_up = GGCount_up + 1;
            }
        }
        for (int index = 0; index < GetMainObject()->zombieTotal(); ++index)
        {
            if (zombie[index].type() == GIGA_GARGANTUAR && zombie[index].isExist() && !zombie[index].isDead() && (zombie[index].row() == 2 || zombie[index].row() == 3 || zombie[index].row() == 4)) {
                GGCount_down = GGCount_down + 1;
            }
        }
        SetTime(600, wave);
        InsertOperation([=]()
                    {
        if (GGCount_up < GGCount_down && zombie_type[GIGA_GARGANTUAR] && GetMainObject()->refreshCountdown() > 200)
        {
            SetNowTime();
            Card({{FLOWER_POT, 4, 9}, {CHERRY_BOMB, 4, 9}});
        }
        
        else if (GGCount_up >= GGCount_down && zombie_type[GIGA_GARGANTUAR] && GetMainObject()->refreshCountdown() > 200)
        {
            SetNowTime();
            Card({{FLOWER_POT, 2, 9}, {CHERRY_BOMB, 2, 9}});
        }
                    }); });
    }
}

bool IsTTExist(int row, int hp)
{
    bool result = false;
    auto zombie = GetMainObject()->zombieArray();
    for (int index = 0; index < GetMainObject()->zombieTotal(); ++index)
    {
        if (zombie[index].type() == BUCKETHEAD_ZOMBIE && zombie[index].isExist() && !zombie[index].isDead() && zombie[index].row() == row - 1 && zombie[index].hp() >= hp && zombie[index].state() != 2)
        {
            result = true;
        }
    }
    return result;
}

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

//依据血量放垫材,10cs后铲垫，防止撑杆蹭炮
void SmartCard1()
{
    for (int row : {1, 2, 3, 4, 5})
    {
        if (IsZBExist(GIGA_GARGANTUAR, row, 700, 0))
        {
            for (int index : {7, 8})
            {
                SafeCard(index, row, 8, true);
            }
        }
    }
}

//依据血量放垫材,10cs后铲垫，防止撑杆蹭炮
void SmartCard2()
{
    for (int row : {1, 2, 3, 4, 5})
    {
        if (IsZBExist(GIGA_GARGANTUAR, row, 700, 0))
        {
            for (int index : {7})
            {
                SafeCard(index, row, 8, true);
            }
        }
    }
}

void EndWave()
{
    for (auto wave : {9, 19, 20})
    {
        SetTime(530 - 373, wave);
        InsertOperation([=]()
                        {
            if (GetZombieNumber(GIGA_GARGANTUAR, wave))
            {
                SetNowTime();
                dixianpao.pao({{2, 9}});
                qianzhipao.pao({{4, 9}});
                Delay(400);
                dixianpao.recoverPao({{2, 9}});
                qianzhipao.recoverPao({{4, 9}});
                Delay(400);
                dixianpao.recoverPao({{2, 9}});
                qianzhipao.recoverPao({{4, 9}});
            }
            else if (GetZombieNumber(GIGA_GARGANTUAR, wave - 1))
            {
                SetNowTime();
                dixianpao.pao({{2, 9}});
                qianzhipao.pao({{4, 9}});
                Delay(400);
                dixianpao.recoverPao({{2, 8.5}});
                qianzhipao.recoverPao({{4, 8.5}});
            }

            else 
            {
                SetNowTime();
                dixianpao.pao({{2, 9}});
                qianzhipao.pao({{4, 9}});
                SetTime(1100, wave);
                InsertOperation([=]()
                {
                    if ((IsBYExist(1, 0) || IsBYExist(2, 0) || IsBYExist(3, 0) || IsBYExist(4, 0) || IsBYExist(5, 0) || IsTTExist(1, 0) || IsTTExist(2, 0) || IsTTExist(3, 0) || IsTTExist(4, 0) || IsTTExist(5, 0)) && wave == GetMainObject()->wave())
                    {
                        SetNowTime();
                        dixianpao.recoverPao({{2, 8.5}});
                        qianzhipao.recoverPao({{4, 8.5}});
                    }
                });
            } });
    }
}
void Script()
{
    //连续运行(摁0停止运行，但是再次摁0不会重新启动)
    OpenMultipleEffective('0', AvZ::MAIN_UI_OR_FIGHT_UI);

    //自动获取炮序
    SetTime(-599, 1);
    dixianpao.resetPaoList({{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}});
    qianzhipao.resetPaoList({{1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}});

    //十倍速测试
    SetGameSpeed(10);

    //跳帧
    //直接跳过整个游戏
    SkipTick([=]()
             { return true; });

    //帧运行按血量放垫材
    C_a_user.pushFunc(SmartCard1);
    C_b_user.pushFunc(SmartCard2);

    SelectCards({ICE_SHROOM,
                 CHERRY_BOMB,
                 COFFEE_BEAN,
                 DOOM_SHROOM,
                 JALAPENO,
                 PUMPKIN,
                 M_FLOWER_POT,
                 FLOWER_POT,
                 FUME_SHROOM,
                 PUFF_SHROOM});

    for (int wave : {1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        SetTime(530 - 373, wave);
        dixianpao.pao({{2, 9}});
        qianzhipao.pao({{4, 9}});
    }

    EndWave();

    SetTime(-199, 18);
    C_a_user.pause();
    C_b_user.goOn();
    SetTime(600, 20);
    C_a_user.goOn();
    C_b_user.pause();

    SetTime(-1 - 298, 20);
    Card({{FLOWER_POT, 3, 8}, {ICE_SHROOM, 3, 8}, {COFFEE_BEAN, 3, 8}});
    Delay(400);
    Shovel(3, 8);
    SetTime(-599, 1);
    Shovel({{1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}});
    SmartDoom();
    DanceCheat(FAST);
}