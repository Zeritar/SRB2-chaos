#include "console.h"
#include "z_zenya.h"
#include "doomdef.h"
#include "g_game.h"
#include "p_local.h"
#include "d_netcmd.h"
#include "d_clisrv.h"

uint16_t oldemeraldcount;
player_t *player;
INT32 lastEvent;
shieldtype_t shieldTypes[10];
sfxenum_t shieldSFX[10];

typedef enum
{
    LAST_SUPER = 1,
    LAST_ARMAGEDDON,
    LAST_GRAVITY,
    LAST_RANDPOWER,
    LAST_BOMB,
    LAST_NAPALM,
    LAST_QUAKE,
    LAST_SUPERGRAVITY,
    LAST_GROWUP,
    LAST_SHRINK,
} eventTypes;

mobj_t *SpawnPlayerProjectile(fixed_t x, fixed_t y, fixed_t z, mobjtype_t type)
{
    mobj_t *mo;
    mo = P_SpawnMobj(x, y, z, type);
    S_StartSound(mo, mo->info->seesound);
    return mo;
}

// Here in case something needs to be initialized.
void ChaosInit(void)
{
    shieldTypes[0] = SH_PITY;
    shieldTypes[1] = SH_PINK;
    shieldTypes[2] = SH_WHIRLWIND;
    shieldTypes[3] = SH_FORCE + 1;
    shieldTypes[4] = SH_ARMAGEDDON;
    shieldTypes[5] = SH_ATTRACT;
    shieldTypes[6] = SH_ELEMENTAL;
    shieldTypes[7] = SH_BUBBLEWRAP;
    shieldTypes[8] = SH_FLAMEAURA;
    shieldTypes[9] = SH_THUNDERCOIN;

    shieldSFX[0] = sfx_shield;
    shieldSFX[1] = sfx_shield;
    shieldSFX[2] = sfx_wirlsg;
    shieldSFX[3] = sfx_forcsg;
    shieldSFX[4] = sfx_armasg;
    shieldSFX[5] = sfx_attrsg;
    shieldSFX[6] = sfx_elemsg;
    shieldSFX[7] = sfx_s3k3f;
    shieldSFX[8] = sfx_s3k3e;
    shieldSFX[9] = sfx_s3k41;
    CONS_Printf("Initialized Chaos Edition stuff.\n");
}

void GetConsolePlayer(void)
{
    player = &players[consoleplayer];
}

// Checking current level time to see if we've reached the timelimit if there is one
void TimeCheck(INT16 curmap, INT16 curtime)
{
    // No shenanigans when not playing k thx
    if (curmap == titlemap)
        return;

    if (curtime > 2 && player->exiting)
    {
        UndoRandomEvent();
        return;
    }

    // For now let's stay out of NiGHTs. Might do something else there at a later date.
    if (curmap == 50 ||
        curmap == 51 ||
        curmap == 52 ||
        curmap == 53 ||
        curmap == 54 ||
        curmap == 55 ||
        curmap == 56
        )
        return;

    // Get console player for pointer
    if (curtime == 1)
        GetConsolePlayer();

    // Start the fun!
    if (curtime > (1*TICRATE))
    {
        if ((curtime % (8*TICRATE)) == 0*TICRATE)
        {
            UndoRandomEvent();
        }
        else if ((curtime % (8*TICRATE)) == 4*TICRATE)
        {
            DoRandomEvent();
        }

        // Double time?
        /*if ((curtime % (4*TICRATE)) == 0)
        {
            DoRandomEvent();
        }*/

        EventThinker(curtime);
    }
}

void DoRandomEvent(void)
{
    INT32 rand = P_RandomRange(1, 10);

    // Double time?
    /*if (lastEvent > 0)
        {
            UndoRandomEvent();
        }*/

    switch(rand)
    {
        case 1:
            lastEvent = LAST_SUPER;
            DoSuper();
            break;
        case 2:
            lastEvent = LAST_ARMAGEDDON;
            DoArmageddonBlast();
            break;
        case 3:
            lastEvent = LAST_GRAVITY;
            DoFlipGravity();
            break;
        case 4:
            lastEvent = LAST_BOMB;
            DoThrowBomb();
            break;
        case 5:
            lastEvent = LAST_RANDPOWER;
            DoRandomPowerup();
            break;
        case 6:
            lastEvent = LAST_NAPALM;
            DoThrowNapalm();
            break;
        case 7:
            lastEvent = LAST_QUAKE;
            DoEarthquake();
            break;
        case 8:
            lastEvent = LAST_SUPERGRAVITY;
            DoSuperGravity();
            break;
        case 9:
            lastEvent = LAST_GROWUP;
            DoGrowUp();
            break;
        case 10:
            lastEvent = LAST_SHRINK;
            DoShrink();
            break;
        default:
            break;
    }
}

void EventThinker(INT16 curtime)
{
    switch (lastEvent)
    {
        case LAST_BOMB:
            if ((curtime % (1*TICRATE/2)) == 0)
                DoThrowBomb();
            break;
        default:
            break;
    }
}

void UndoRandomEvent(void)
{
    switch(lastEvent)
    {
        case LAST_SUPER:
            UndoSuper();
            break;
        case LAST_GRAVITY:
            UndoFlipGravity();
            break;
        case LAST_QUAKE:
            UndoEarthquake();
            break;
        case LAST_SUPERGRAVITY:
            UndoSuperGravity();
            break;
        case LAST_GROWUP:
            UndoGrowUp();
            break;
        case LAST_SHRINK:
            UndoShrink();
            break;
        default:
            break;
    }
    lastEvent = 0;
}

void DoFlipGravity(void)
{
    player->powers[pw_gravityboots] = 9999;
    S_StartSound(player->mo, sfx_s3k74);
}

void UndoFlipGravity(void)
{
    player->powers[pw_gravityboots] = 0;
    S_StartSound(player->mo, sfx_s3k72);
}

void DoArmageddonBlast(void)
{
    P_BlackOw(player);
    player->mo->momz = 45*FRACUNIT;
    player->pflags |= PF_JUMPED;
	P_SetPlayerMobjState(player->mo, S_PLAY_JUMP);
}

void DoSuper(void)
{
    oldemeraldcount = emeralds;
    emeralds = ((EMERALD7)*2)-1;
    if (player->rings < 5)
        player->rings = 5;
        player->charflags |= SF_SUPER;
    player->powers[pw_super] = 1;
    S_StartSound(NULL, sfx_s3kb3);
}

void UndoSuper(void)
{
    player->powers[pw_super] = 0;
    if (player->skin == 1 || player->skin == 3 || player->skin == 4)
        player->charflags ^= SF_SUPER;
    
    emeralds = oldemeraldcount;
    player->mo->color = player->skincolor;
    S_StartSound(player->mo, sfx_kc65);
}

void DoThrowBomb(void)
{
    if (player->pflags & MF2_OBJECTFLIP)
        return;

    fixed_t zoffset;
	mobj_t *missile;
    mobj_t *actor = player->mo;

	zoffset = actor->z + FixedMul(64*FRACUNIT, actor->scale);

    missile = SpawnPlayerProjectile(actor->x, actor->y, zoffset, MT_FBOMB);

    missile->momx = missile->momy = 0;
	missile->momz = missile->info->speed/2;
}

void DoThrowNapalm(void)
{
    if (player->pflags & MF2_OBJECTFLIP)
        return;

    fixed_t zoffset;
	mobj_t *missile;
    mobj_t *actor = player->mo;

	zoffset = actor->z + FixedMul(64*FRACUNIT, actor->scale);

    missile = SpawnPlayerProjectile(actor->x, actor->y, zoffset, MT_CYBRAKDEMON_NAPALM_BOMB_LARGE);

    missile->momx = missile->momy = 0;
	missile->momz = missile->info->speed/2;
}

void DoRandomPowerup(void)
{
    INT32 rand = P_RandomRange(0, 9);
    P_SwitchShield(player, shieldTypes[rand]);
	S_StartSound(player->mo, shieldSFX[rand]);
}

void DoEarthquake(void)
{
    quake.time = 999;
    quake.intensity = 32*FRACUNIT;
    S_StartSound(player->mo, sfx_s3kccl);
}

void UndoEarthquake(void)
{
    quake.time = 0;
    S_StopSoundByNum(sfx_s3kccl);
}

void DoSuperGravity(void)
{
    gravity = gravity * 3;
    S_StartSound(player->mo, sfx_s3kc6l);
}

void UndoSuperGravity(void)
{
    gravity = gravity / 3;
    S_StopSoundByNum(sfx_s3kc6l);
}

void DoGrowUp(void)
{
    player->mo->destscale = 2*FRACUNIT;
    player->normalspeed = player->normalspeed / 2;
    player->actionspd = player->actionspd / 2;
    S_StartSound(player->mo, sfx_kc5a);
}

void UndoGrowUp(void)
{
    player->mo->destscale = FRACUNIT;
    player->normalspeed = player->normalspeed * 2;
    player->actionspd = player->actionspd * 2;
    S_StartSound(player->mo, sfx_kc59);
}

void DoShrink(void)
{
    player->mo->destscale = FRACUNIT/2;
    S_StartSound(player->mo, sfx_s3k72);
}

void UndoShrink(void)
{
    player->mo->destscale = FRACUNIT;
    S_StartSound(player->mo, sfx_s3k75);
}