#include "common.h"

#include "IniFile.h"

#include "CarCtrl.h"
#include "FileMgr.h"
#include "main.h"
#include "Population.h"

float CIniFile::PedNumberMultiplier = 0.6f;
float CIniFile::CarNumberMultiplier = 0.8f;

void CIniFile::LoadIniFile()
{
	CFileMgr::SetDir("");
	int f = CFileMgr::OpenFile("gta3.ini", "r");
	if (f){
		CFileMgr::ReadLine(f, gString, 200);
		sscanf(gString, "%f", &PedNumberMultiplier);
		PedNumberMultiplier = Min(3.0f, Max(0.5f, PedNumberMultiplier));
		CFileMgr::ReadLine(f, gString, 200);
		sscanf(gString, "%f", &CarNumberMultiplier);
		CarNumberMultiplier = Min(3.0f, Max(0.5f, CarNumberMultiplier));
		CFileMgr::CloseFile(f);
	}
	CPopulation::MaxNumberOfPedsInUse = 25.0f * PedNumberMultiplier;
	CPopulation::MaxNumberOfPedsInUseInterior = 40.0f * PedNumberMultiplier;
	CCarCtrl::MaxNumberOfCarsInUse = 30.0f * CarNumberMultiplier;
}