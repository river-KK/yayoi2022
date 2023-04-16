# include "Title.h"

Title::Title(const InitData& init)
	: IScene{ init } {}

void Title::update()
{
	const auto& data = getData();
	m_startTransition.update(m_startButton.mouseOver());
	m_rankingTransition.update(m_rankingButton.mouseOver());
	m_exitTransition.update(m_exitButton.mouseOver());

	if (m_startButton.mouseOver() || m_rankingButton.mouseOver() || m_exitButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}
	if (m_startButton.leftClicked()|| (KeyEnter.pressed() && m_startButton.mouseOver()))
	{
		// オーディオを再生
		data.audio_start.play();
		// ゲームシーンへ
		changeScene(State::Game);

	}
	else if (m_rankingButton.leftClicked()|| (KeyEnter.pressed() && m_rankingButton.mouseOver()))
	{
		// ランキングシーンへ
		changeScene(State::Ranking);
		data.audio_anthar.setVolume(data.volume);
		data.audio_anthar.play();

	}
	else if (m_exitButton.leftClicked() || (KeyEnter.pressed() && m_exitButton.mouseOver()))
	{
		// 終了
		System::Exit();
	}
	else {
	}
}

void Title::draw() const
{
	const auto& data = getData();
	Rect{ Arg::center(400, 300), 960, 600 }(data.imageTex).draw();

	FontAsset(U"TitleFont")(U"Shooting Game")
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 400, 100 });

	m_startButton.draw(ColorF{ 1.0,0.5});
	m_rankingButton.draw(ColorF{ 1.0,0.5 });
	m_exitButton.draw(ColorF{ 1.0,0.5 });

	m_startButton.draw(ColorF{ 1, m_startTransition.value() }).drawFrame(2,2, ColorF(1), ColorF(0.2, 0.6, 0.2));
	m_rankingButton.draw(ColorF{ 1, m_rankingTransition.value() }).drawFrame(2, 2, ColorF(1) , ColorF(0.2, 0.6, 0.2));
	m_exitButton.draw(ColorF{ 1, m_exitTransition.value() }).drawFrame(2, 2,ColorF(1), ColorF(0.2, 0.6, 0.2));

	FontAsset(U"Menu")(U"PLAY").drawAt(m_startButton.center(), ColorF{ 0.2, 0.6, 0.2 });
	FontAsset(U"Menu")(U"RANKING").drawAt(m_rankingButton.center(), ColorF{ 0.2, 0.6, 0.2 });
	FontAsset(U"Menu")(U"EXIT").drawAt(m_exitButton.center(), ColorF{ 0.2, 0.6, 0.2 });
}
