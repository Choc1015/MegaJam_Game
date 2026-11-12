// Copyright Epic Games, Inc. All Rights Reserved.

#include "WhatWasCutGameMode.h"
#include "Cut/CutHUD.h"

AWhatWasCutGameMode::AWhatWasCutGameMode()
{
	HUDClass = ACutHUD::StaticClass();
}
