#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include <Input.h>
#include <MyMath.h>
#include <Sprite.h>
class Player {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandleAttackRange);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewprojection);

	/// <summary>
	/// 描画
	/// </summary>
	void CollisionDraw(ViewProjection& viewprojection);
	
	const float kPlayerSpeed = (float)0.2; // プレイヤーのスピード規定値
	int playerChageFrame = 0;          // プレイヤーがチャージしているフレーム数
	bool isPlayerChage = false;        // プレイヤーがチャージしているか
	int isPlayerAttack = 0;            // プレイヤーが攻撃中か
	const int kPlayerAttackFrame = 60; // プレイヤーの攻撃時間規定値
	int playerAttackTimer = 0; // プレイヤーが攻撃しているフレーム数タイマー
	const int kAttackRangeWeak = 6;    // 弱攻撃の範囲
	const int kAttackRangeStrong = 10; // 強攻撃の範囲

	enum side { up, down, left, right };
	int nowPlayerSide = up; // 現在プレイヤーがどこの辺にいるか

	Vector3 attackRangePos{};


private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandleAttackRange_ = 0;

	// スプライト
	Sprite* sprite_ = nullptr;

	Input* input_ = nullptr;

	// 3Dモデル
	Model* model_ = nullptr;

};
