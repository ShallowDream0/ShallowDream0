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
    SetTime(-599, 1);
    ice_filler.start({{4, 9}});

    for (auto wave : {1, 4, 7, 11, 14, 17})
    {
        SetTime(-14, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        Delay(107);
        pao_operator.pao({{1, 7.7}, {5, 7.7}});
    }

    for (auto wave : {2, 5, 8, 12, 15, 18})
    {
        SetTime(270 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        SetTime(270 - 298, wave);
        ice_filler.coffee();
    }

    for (auto wave : {3, 6, 9, 10, 13, 16, 19})
    {
        SetTime(270 - 373, wave);
        pao_operator.pao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});
        Delay(110);
        pao_operator.pao({{1, 9}, {5, 8.8}});
    }

    SetTime(-128, 20);
    pao_operator.pao({{4, 7.5875}});
    SetTime(270 - 373, 20);
    pao_operator.pao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});
    Delay(110);
    pao_operator.pao({{1, 9}, {5, 9}});
}