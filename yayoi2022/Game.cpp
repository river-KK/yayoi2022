# include "Game.h"

Game::Game(const InitData& init)
	: IScene{ init }
{
};

void Game::update()
{
	//敵の数の増加
	enemymath = score / 100;
	if (enemymath <= 1) {
		enemymath = 1;
	}

	// ゲームオーバー判定
	bool gameover = false;
	
	const double deltaTime = Scene::DeltaTime();
	enemyAccumulator += deltaTime;
	playerShotTimer = Min((playerShotTimer + deltaTime), playerShotCoolTime);
	enemyShotTimer += deltaTime;
	while (enemySpawnTime <= enemyAccumulator)
	{

		enemyAccumulator -= enemySpawnTime;
		enemySpawnTime = Max(enemySpawnTime * 0.98, 0.25);
		for (int i = 0; i < enemymath; i++) {
			enemies << GenerateEnemy();
		}
		
	}
	

	//-------------------
	//
	// 移動
	//

	// 自機の移動


	const Vec2 move = Vec2{ ((KeyRight.pressed()|| KeyD.pressed()) - (KeyLeft.pressed()|| KeyA.pressed())), ((KeyDown.pressed()|| KeyS.pressed()) - (KeyUp.pressed()|| KeyW.pressed())) }
	.setLength(deltaTime * playerSpeed * (KeyShift.pressed() ? 0.65 : 1.0));
	playerPos.moveBy(move).clamp(Scene::Rect());
	

	

	// 自機ショットの発射
	if (playerShotCoolTime <= playerShotTimer)
	{
		if (KeyShift.pressed()) {
			playerShotTimer -= playerShotCoolTime/1.75;
		}
		else {
			playerShotTimer -= playerShotCoolTime;
		}
		playerBullets << playerPos.movedBy(0, -10);
	}

	// 自機ショットの移動
	for (auto& playerBullet : playerBullets)
	{
		playerBullet.y += (deltaTime * -playerBulletSpeed);
	}
	// 画面外に出た自機ショットは消滅
	playerBullets.remove_if([](const Vec2& b) { return (b.y < -40); });

	// 敵の移動
	for (auto& enemy : enemies)
	{
		enemy.y += (deltaTime * enemySpeed);
	}
	// 画面外に出た敵は消滅
	enemies.remove_if([&](const Vec2& e)
	{
		if (700 < e.y)
		{
			life--;
			return true;
		}
		else
		{
			return false;
		}
	});

	// 敵ショットの発射
	if (enemyShotCoolTime <= enemyShotTimer)
	{
		enemyShotTimer -= enemyShotCoolTime;
		for (const auto& enemy : enemies)
		{
			enemyBullets << enemy;
		}
	}

	// 敵ショットの移動
	for (auto& enemyBullet : enemyBullets)
	{
		enemyBullet.y += deltaTime * enemyBulletSpeed;
	}
	// 画面外に出た自機ショットは消滅
	enemyBullets.remove_if([](const Vec2& b) {return (700 < b.y); });

	//-------------------
	//
	// 攻撃判定
	//

	// 敵 vs 自機ショット
	for (auto itEnemy = enemies.begin(); itEnemy != enemies.end();)
	{
		const Circle enemyCircle{ *itEnemy, 40 };
		bool skip = false;

		for (auto itBullet = playerBullets.begin(); itBullet != playerBullets.end();)
		{
			if (enemyCircle.intersects(*itBullet))
			{
				// 爆発エフェクトを追加
				effect.add([pos = *itEnemy](double t)
				{
				const double t2 = (1.0 - t);
				Circle{ pos, 10 + t * 70 }.drawFrame(20 * t2, AlphaF(t2 * 0.5));
				return (t < 1.0);
				});

				itEnemy = enemies.erase(itEnemy);
				playerBullets.erase(itBullet);
				++score;
				skip = true;
				break;
			}

			++itBullet;
		}

		if (skip)
		{
			continue;
		}

		++itEnemy;
	}

	// 敵ショット vs 自機
	for (const auto& enemyBullet : enemyBullets)
	{
		// 敵ショットが playerPos の 20 ピクセル以内に接近したら
		if (enemyBullet.distanceFrom(playerPos) <= 10&&isHit==false)
		{
			isHit = true;
			life--;
			break;
		}
	}

	// 無敵時間をセット
	if (isHit) {
		Accumulator += deltaTime;
		if (invincible <= Accumulator)
		{
			Accumulator -= invincible;
			isHit = false;
		}
	}
	
	//ゲームオーバー
	if (life <= 0) {
		gameover = true;
	}

	// ゲームオーバーならリセット
	if (gameover)
	{
		// ランキング画面へ
		changeScene(State::Ranking);
		getData().lastGameScore = score;
	}

	
}
void Game::draw() const
{
	const auto& data = getData();
	Rect{ Arg::center(400, 300), 960, 600 }(data.imageTex).draw();

	// 自機の描画

	//無敵時間の描写
	if (isHit) {
		if (Periodic::Square0_1(0.1)) {
			playerTexture.flipped().resized(50).drawAt(playerPos);
			Circle{ playerPos, 9 }.draw(Palette::White).drawFrame(1, 1, ColorF(Palette::Deeppink));
		}
	}
	else {
		playerTexture.flipped().resized(50).drawAt(playerPos);
		Circle{ playerPos, 9 }.draw(Palette::White).drawFrame(1, 1, ColorF(Palette::Seagreen));
	}
	

	// 自機ショットの描画
	for (const auto& playerBullet : playerBullets)
	{
		Circle{ playerBullet, 6 }.draw(Palette::White).drawFrame(2, ColorF(0.2, 0.6, 0.2));
	}

	// 敵の描画
	for (const auto& enemy : enemies)
	{
		enemyTexture.flipped().resized(50).drawAt(enemy);
	}

	// 敵ショットの描画
	for (const auto& enemyBullet : enemyBullets)
	{
		Circle{ enemyBullet, 4 }.draw(Palette::White).drawFrame(2,Palette::Orange);
	}
	effect.update();
	// スコアの描画
	font(U"{} [{}]"_fmt(score, data.highScores[0])).draw(Arg::bottomRight(780, 580));
	// ライフの描画
	font(U"{}{}"_fmt(U"LIFE ",life)).draw(Arg::bottomLeft(20, 40));
}

