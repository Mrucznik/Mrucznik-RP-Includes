#if defined _ACSBM_included
	#endinput
#endif
#define _ACSBM_included

#include <a_samp>
#include <crashdetect>
#include <log-plugin>
//#pragma tabsize 0

new kaska[MAX_PLAYERS];
new Logger:dajkaseLog;

forward Ac_OnGameModeInit();
public Ac_OnGameModeInit()//anty cheat
{
	dajkaseLog = CreateLog("logi/dajkase");
	SetTimer("AntyCheatSystemByMrucznik", 1000, 1);
	return 1;
}

forward Ac_OnPlayerConnect(playerid);
public Ac_OnPlayerConnect(playerid)
{
	kaska[playerid]=0;	
	return 1;
}

forward AntyCheatSystemByMrucznik();
public AntyCheatSystemByMrucznik()
{
    for(new i = 0, p = GetMaxPlayers(); i < p; i++)
    {
		if(GetPlayerMoney(i) != kaska[i])
		{
			if(IsPlayerConnected(i))
			{
				ResetPlayerMoney(i);
				GivePlayerMoney(i,kaska[i]);
			}
		}
	}
	return 1;
}

forward DajKase(playerid,pieniadze);
public DajKase(playerid,pieniadze)
{
	new logstring[256], nick[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nick, sizeof(nick));
	format(logstring, sizeof(logstring), "%s dostal %d$", nick, pieniadze);

	kaska[playerid]+=pieniadze;
	GivePlayerMoney(playerid,pieniadze);
	
	if(pieniadze < 0)
	{
		Log(dajkaseLog, ERROR, logstring);
	}
	else
	{
		Log(dajkaseLog, INFO, logstring);
	}
	return 1;
}

forward ZabierzKase(playerid, money);
public ZabierzKase(playerid, money)
{
	new logstring[256], nick[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nick, sizeof(nick));
	format(logstring, sizeof(logstring), "%s zabrano %d$", nick, money);

	kaska[playerid] -= money;
	GivePlayerMoney(playerid, -money);
	
	if(money < 0)
	{
		Log(dajkaseLog, ERROR, logstring);
	}
	else
	{
		Log(dajkaseLog, INFO, logstring);
	}
	return 1;
}

forward UstawKase(playerid, money);
public UstawKase(playerid, money)
{
	new logstring[256], nick[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nick, sizeof(nick));
	format(logstring, sizeof(logstring), "%s ustawiono %d$", nick, money);

	kaska[playerid] = money;
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, money);
	
	Log(dajkaseLog, INFO, logstring);
	return 1;
}

forward ResetujKase(playerid);
public ResetujKase(playerid)
{
	kaska[playerid]=0;
	ResetPlayerMoney(playerid);
	return 1;
}