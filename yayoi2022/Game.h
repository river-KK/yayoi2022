#pragma once
# pragma once
# include "Common.h"

// ゲームシーン
class Game : public App::Scene
{
public:

	Game(const InitData& init);

	void update() override;
	void draw() const override;

private:
	Vec2 GenerateEnemy()
	{
		return RandomVec2({ 50, 750 }, -20);
	}
	const Font font{ 30 };

	// 自機テクスチャ
	const Texture playerTexture{ U"🌩"_emoji };
	// 敵テクスチャ
	const Texture enemyTexture{ U"👾"_emoji };

	// 自機
	Vec2 playerPos{ 400, 500 };
	// 敵
	Array<Vec2> enemies = { GenerateEnemy() };

	// 自機ショット
	Array<Vec2> playerBullets;
	// 敵ショット
	Array<Vec2> enemyBullets;

	//自機のライフ
	int life = 3;
	// 無敵時間のカウンタ
	double invincible = 1.5;
	double Accumulator = 0.0;
	// ヒットしたかのフラグ　障害物と当たったらtrueになる
	bool isHit = false;
	// 自機のスピード
	double playerSpeed = 550.0;
	// 自機ショットのスピード
	double playerBulletSpeed = 500.0;


	// 敵のスピード
	double enemySpeed = 100.0;
	// 敵ショットのスピード
	double enemyBulletSpeed = 300.0;
	// 敵の発生間隔の初期値（秒）
	double initialEnemySpawnTime = 0.5;
	// 敵の発生間隔（秒）
	double enemySpawnTime = initialEnemySpawnTime;
	// 敵の発生の蓄積時間（秒）
	double enemyAccumulator = 0.0;
	//敵の発生数
	int enemymath=1;

	// 自機ショットのクールタイム（秒）
	double playerShotCoolTime = 0.1;
	// 自機ショットのクールタイムタイマー（秒）
	double playerShotTimer = 0.0;

	// 敵ショットのクールタイム（秒）
	double enemyShotCoolTime = 1.10;
	// 敵ショットのクールタイムタイマー（秒）
	double enemyShotTimer = 0.0;

	Effect effect;

	// 現在のスコア
	int32 score = 0;
};
