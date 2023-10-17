#include "Player.h"
#include <cassert>
#include <imgui.h>
#include <iostream>
#include <algorithm>
#include <PrimitiveDrawer.h>

Player::Player() {

}

Player::~Player() {
	delete sprite_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandleAttackRange) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	textureHandleAttackRange_ = textureHandleAttackRange;

	sprite_ = Sprite::Create(textureHandleAttackRange_, {0, 0});

	input_ = Input::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 15;
}

void Player::Update() { 

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Translation", reinterpret_cast<float*>(&worldTransform_.translation_), -15, 15);
	ImGui::End();

	//今どこの辺にいるか
	if (worldTransform_.translation_.y <= -15) {
		nowPlayerSide = up;
	} else if (worldTransform_.translation_.y >= 15) {
		nowPlayerSide = down;
	} else if (worldTransform_.translation_.x <= -15) {
		nowPlayerSide = left;
	} else if (worldTransform_.translation_.x >= 15) {
		nowPlayerSide = right;
	}

	//横移動
	if (worldTransform_.translation_.y >= 15 || worldTransform_.translation_.y <= -15) {
		if (input_->PushKey(DIK_A)) {
			worldTransform_.translation_.x -= kPlayerSpeed;
		}
		if (input_->PushKey(DIK_D)) {
			worldTransform_.translation_.x += kPlayerSpeed;
		}
	}

	//縦移動
	if (worldTransform_.translation_.x >= 15 || worldTransform_.translation_.x <= -15) {
		if (input_->PushKey(DIK_W)) {
			worldTransform_.translation_.y += kPlayerSpeed;
		}
		if (input_->PushKey(DIK_S)) {
			worldTransform_.translation_.y -= kPlayerSpeed;
		}
	}
	
	//チャージ処理
	if (input_->PushKey(DIK_SPACE)) {
		playerChageFrame++;
		isPlayerChage = true;
	}

	//ボタンを離したとき
	if (isPlayerChage && !input_->PushKey(DIK_SPACE)) {
		if (playerChageFrame >= 90) {
			    isPlayerAttack = 2;//強攻撃
		} else {
			    isPlayerAttack = 1;//弱攻撃
		}

		switch (nowPlayerSide) {
			case up://上にいたら下に
					worldTransform_.translation_.y = 15;
					break;
			case down://下にいたら上に
					worldTransform_.translation_.y = -15;
					break;
			case left://左にいたら右に
					worldTransform_.translation_.x = 15;
					break;
			case right://右にいたら左に
					worldTransform_.translation_.x = -15;
					break;
		}

		playerAttackTimer = kPlayerAttackFrame; // プレイヤーの攻撃タイマーをセット
		playerChageFrame = 0;//プレイヤーのチャージ中フレームをリセット
		isPlayerChage = false;//プレイヤーチャージフラグをオフに戻す
		attackRangePos = worldTransform_.translation_;
	}

	//攻撃中の処理
	if (isPlayerAttack) {
		playerAttackTimer--;//タイマー
		if (!playerAttackTimer) {
			//タイマーが0になったら攻撃中フラグをオフに
		     isPlayerAttack = 0;
		}
	}
	
	//clampでプレイヤーの移動範囲制限
	worldTransform_.translation_.y = min(max(-15, worldTransform_.translation_.y), 15);
	worldTransform_.translation_.x = min(max(-15, worldTransform_.translation_.x), 15);
	
	//worldTransform更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection &viewprojection) { 

	//プレイヤーモデルの表示
	model_->Draw(worldTransform_, viewprojection, textureHandle_);

}

void Player::CollisionDraw(ViewProjection& viewprojection) {

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewprojection);
	// 当たり判定の表示

	int attackRange{};

	if (isPlayerAttack == 2) {
		attackRange = kAttackRangeStrong;
	} else if (isPlayerAttack == 1) {
		attackRange = kAttackRangeWeak;
	}

	if (isPlayerAttack) {
		switch (nowPlayerSide) {
		case up:
			 PrimitiveDrawer::GetInstance()->DrawLine3d(
			     {attackRangePos.x - attackRange / 2, attackRangePos.y, attackRangePos.z},
			     {attackRangePos.x - attackRange / 2, attackRangePos.y + 30, attackRangePos.z},
			     {1.0f, 0.0f, 0.0f, 1.0f});
			 PrimitiveDrawer::GetInstance()->DrawLine3d(
			     {attackRangePos.x + attackRange / 2, attackRangePos.y, attackRangePos.z},
			     {attackRangePos.x + attackRange / 2, attackRangePos.y + 30, attackRangePos.z},
			     {1.0f, 0.0f, 0.0f, 1.0f});
			 break;
		case down:
			 PrimitiveDrawer::GetInstance()->DrawLine3d(
			     {attackRangePos.x - attackRange / 2, attackRangePos.y, attackRangePos.z},
			     {attackRangePos.x - attackRange / 2, attackRangePos.y - 30, attackRangePos.z},
			     {1.0f, 0.0f, 0.0f, 1.0f});
			 PrimitiveDrawer::GetInstance()->DrawLine3d(
			     {attackRangePos.x + attackRange / 2, attackRangePos.y, attackRangePos.z},
			     {attackRangePos.x + attackRange / 2, attackRangePos.y - 30, attackRangePos.z},
			     {1.0f, 0.0f, 0.0f, 1.0f});
			 break;
		case left:
			 PrimitiveDrawer::GetInstance()->DrawLine3d(
			     {attackRangePos.x, attackRangePos.y - attackRange / 2, attackRangePos.z},
			     {attackRangePos.x + 30, attackRangePos.y - attackRange / 2, attackRangePos.z},
			     {1.0f, 0.0f, 0.0f, 1.0f});
			 PrimitiveDrawer::GetInstance()->DrawLine3d(
			     {attackRangePos.x, attackRangePos.y + attackRange / 2, attackRangePos.z},
			     {attackRangePos.x + 30, attackRangePos.y + attackRange / 2, attackRangePos.z},
			     {1.0f, 0.0f, 0.0f, 1.0f});
			 break;
		case right:
			 PrimitiveDrawer::GetInstance()->DrawLine3d(
			     {attackRangePos.x, attackRangePos.y - attackRange / 2, attackRangePos.z},
			     {attackRangePos.x - 30, attackRangePos.y - attackRange / 2, attackRangePos.z},
			     {1.0f, 0.0f, 0.0f, 1.0f});
			 PrimitiveDrawer::GetInstance()->DrawLine3d(
			     {attackRangePos.x, attackRangePos.y + attackRange / 2, attackRangePos.z},
			     {attackRangePos.x - 30, attackRangePos.y + attackRange / 2, attackRangePos.z},
			     {1.0f, 0.0f, 0.0f, 1.0f});
			 break;
		}
	}

}
