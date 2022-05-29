#pragma once

// This file contains the ID-s for used assets / resources.
// You should append everytime a new asset is introduced.

namespace res
{
	enum class Texture
	{
		PlayerIdle,
		PlayerRun,
		PlayerJump,
		PlayerClimb,
		MenuBackgroundMountain,
		MenuBackgroundForest,
		PauseBackground,
		ParallaxMountain1,
		ParallaxMountain2,
		ParallaxMountain3,
		ParallaxMountain4,
		ParallaxMountain5,
		ParallaxForest1,
		ParallaxForest2,
		ParallaxForest3,
		ParallaxForest4,
		ParallaxForest5
	};

	enum class Font
	{
		Roboto,
		Pixel,
		Upheaval,
		Fipps
	};

	enum class Music
	{
		PeacefulGameplay,
		LudumDare1,
		LudumDare2,
		LudumDare3,
		LudumDare4,
		LudumDare5,
		LudumDare6,
		LudumDare7,
		LudumDare8,
		LudumDare9,
		LudumDare10
	};

	enum class Sound
	{
		Jump1,
		Jump2,
		FootStepGrass
	};

	enum class Map
	{
		TestMap
	};
}
