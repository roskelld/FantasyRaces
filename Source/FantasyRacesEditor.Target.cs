// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FantasyRacesEditorTarget : TargetRules
{
	public FantasyRacesEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "FantasyRaces" } );
	}
}
