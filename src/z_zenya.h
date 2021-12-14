#include "g_game.h"

void TimeCheck(INT16 curmap, INT16 curtime);
void ChaosInit(void);
void GetConsolePlayer(void);
void DoRandomEvent(void);
void UndoRandomEvent(void);
void EventThinker(INT16 curtime);
void DoFlipGravity(void);
void UndoFlipGravity(void);
void DoSuper(void);
void UndoSuper(void);
void DoArmageddonBlast(void);
void DoRandomPowerup(void);
void DoThrowBomb(void);
void DoThrowNapalm(void);
void DoEarthquake(void);
void UndoEarthquake(void);
void DoSuperGravity(void);
void UndoSuperGravity(void);
void DoGrowUp(void);
void UndoGrowUp(void);
void DoShrink(void);
void UndoShrink(void);
void DoDiscoMode(void);
void UndoDiscoMode(void);
void ResetPlayer(void);
void BackupPlayer(void);

mobj_t *SpawnPlayerProjectile(fixed_t x, fixed_t y, fixed_t z, mobjtype_t type);