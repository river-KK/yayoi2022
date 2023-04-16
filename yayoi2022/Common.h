# pragma once
# include <Siv3D.hpp>

// シーンの名前
enum class State
{
	Title,
	Game,
	Ranking,
};

// 共有するデータ
struct GameData
{
	// 直前のゲームのスコア
	Optional<int32> lastGameScore;

	// ハイスコア
	Array<int32> highScores = { 150, 145, 100, 10, 0 };
	const Texture imageTex{ U"image/wind.png" ,TextureDesc::Mipped };
	// 音声ファイルを読み込んで Audio を作成
	const Audio audio_start{ U"image/monhan.wav" };
	// 音声ファイルを読み込んで Audio を作成
	const Audio audio_anthar{ U"example/shot.mp3" };
	double volume = 0.1;

};

using App = SceneManager<State, GameData>;
