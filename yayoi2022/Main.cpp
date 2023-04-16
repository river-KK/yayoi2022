# include <Siv3D.hpp>
# include "Common.h"
# include "Title.h"
# include "Game.h"
# include "Ranking.h"

void Main()
{
	Scene::SetLetterbox(Color{ 30, 30, 30 ,255 });
	Scene::SetResizeMode(ResizeMode::Keep);
	//Window::SetFullscreen(true);
	Window::SetStyle(WindowStyle::Sizable);

	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 50, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);
	FontAsset::Register(U"Menu", FontMethod::MSDF, 40, Typeface::Medium);
	FontAsset::Register(U"Ranking", 40, Typeface::Heavy);
	FontAsset::Register(U"GameScore", 30, Typeface::Light);
	AudioAsset::Register(U"Brick", GMInstrument::Woodblock, PianoKey::C5, 0.2s, 0.1s);

	App manager;
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);
	manager.add<Ranking>(State::Ranking);

	// ゲームシーンから開始したい場合はこのコメントを外す
	//manager.init(State::Game);
	
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
