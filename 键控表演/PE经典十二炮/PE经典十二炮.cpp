#include <avz.h>

using namespace AvZ;

void Script()
{
    SetZombies({HY_32, BY_23, BC_12, XC_15, CG_3, KG_17, TT_18, GL_7, HT_14, FT_21, WW_8});
    SelectCards({
        CHERRY_BOMB,    //樱桃
        ICE_SHROOM,     //寒冰菇
        M_ICE_SHROOM,   //模仿冰
        COFFEE_BEAN,    //咖啡豆
        PUFF_SHROOM,    //小喷菇
        FLOWER_POT,     //花盆
        SUN_SHROOM,     //阳光菇
        SCAREDY_SHROOM, //胆小菇
        SQUASH,         //倭瓜
        PUMPKIN,        //南瓜头
    });

    pao_operator.autoGetPaoList();

    for (auto wave : {1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18})
    {
        SetTime(341 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
    }

    for (auto wave : {9, 19})
    {
        SetTime(341 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(100);
        pao_operator.recoverPao({{2, 9}, {5, 9}});
        Delay(1);
        pao_operator.recoverPao({{2, 9}, {5, 9}});
    }

    for (auto wave : {10})
    {
        SetTime(341 - 3 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        SetTime(599 - 100);
        Card({{CHERRY_BOMB, 2, 9}});
    }

    for (auto wave : {20})
    {
        SetTime(-128, wave);
        pao_operator.pao({{4, 7.5875}});
        SetTime(341 - 3 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(100);
        pao_operator.recoverPao({{2, 9}, {5, 9}});
        Delay(1);
        pao_operator.recoverPao({{2, 9}, {5, 9}});
    }
}