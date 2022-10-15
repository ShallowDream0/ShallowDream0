#include <avz.h>
#include <shallowdream.h>

bool safe = true;
using namespace AvZ;
TickRunner C_a_user;

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
        pao_operator.pao({{2, 9}, {4, 9}});
        Delay(400);
        InsertOperation([=]()
                        {
            if ((IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsBYExist(1, 0) || IsBYExist(2, 0) || IsBYExist(3, 0) || IsBYExist(4, 0) || IsBYExist(5, 0) || IsHYExist(1, 0) || IsHYExist(2, 0) || IsHYExist(3, 0) || IsHYExist(4, 0) || IsHYExist(5, 0)) && wave == GetMainObject()->wave())
            {
                SetNowTime();
                pao_operator.recoverPao({{2, 8.5}, {4, 8.5}});
                SetTime(1500, wave);
                InsertOperation([=]()
                {
                    if ((IsMSExist(1, 0) || IsMSExist(2, 0) || IsMSExist(3, 0) || IsMSExist(4, 0) || IsMSExist(5, 0) || IsBYExist(1, 0) || IsBYExist(2, 0) || IsBYExist(3, 0) || IsBYExist(4, 0) || IsBYExist(5, 0) || IsHYExist(1, 0) || IsHYExist(2, 0) || IsHYExist(3, 0) || IsHYExist(4, 0) || IsHYExist(5, 0)) && wave == GetMainObject()->wave())
                    {
                        SetNowTime();
                        pao_operator.recoverPao({{2, 9}, {4, 9}});
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
    pao_operator.autoGetPaoList();

    //十倍速测试
    SetGameSpeed(10);

    //跳帧
    //直接跳过整个游戏
    SkipTick([=]()
             { return true; });

    //帧运行按血量放垫材
    C_a_user.pushFunc(SmartCard1);

    SelectCards({ICE_SHROOM,
                 SQUASH,
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
        pao_operator.pao({{2, 9}, {4, 9}});
    }

    EndWave();

    SetTime(-1 - 100, 20);
    Card({{FLOWER_POT, 3, 8}, {ICE_SHROOM, 3, 8}});
    Delay(110);
    Shovel(3, 8);
    SetTime(-199, 10);
    Shovel({{1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}});
    SetTime(-199, 20);
    Shovel({{1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}});
}