#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <imgui.h>

GameScene::GameScene() {}

GameScene::~GameScene() { 

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("white1x1.png");
	textureHandleAttackRange_ = TextureManager::Load("red1x1.png");
	model_.reset(Model::Create());

	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(model_.get(), textureHandle_, textureHandleAttackRange_);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	//敵の初期化
	enemy_[0] = new Enemy;
	enemy_[1] = new Enemy;
	enemy_[2] = new Enemy;
	enemy_[3] = new Enemy;
	enemy_[4] = new Enemy;
	enemy_[5] = new Enemy;

	enemy_[0]->Initialize(model_.get(), textureHandle_, -22, 0, -0.5f, 0.05f);
	enemy_[1]->Initialize(model_.get(), textureHandle_, 10, -10, 0.5f, -0.5f);
	enemy_[2]->Initialize(model_.get(), textureHandle_, -23, -20, -0.5f, -0.5f);
	enemy_[3]->Initialize(model_.get(), textureHandle_, 0, 0, 0.5f, 0.5f);
	enemy_[4]->Initialize(model_.get(), textureHandle_, -30, -10, -0.5f, 0.5f);
	enemy_[5]->Initialize(model_.get(), textureHandle_, 20, 5, 0.5f, -0.5f);
}

void GameScene::Update() {
	//自キャラの更新
	player_->Update();

	//敵の更新
	enemy_[0]->Update();
	enemy_[1]->Update();
	enemy_[2]->Update();
	enemy_[3]->Update();
	enemy_[4]->Update();
	enemy_[5]->Update();

	ImGui::Begin("enemy");
	ImGui::Checkbox("enemy0", &enemy_[0]->isDeath);
	ImGui::Checkbox("enemy1", &enemy_[1]->isDeath);
	ImGui::Checkbox("enemy2", &enemy_[2]->isDeath);
	ImGui::Checkbox("enemy3", &enemy_[3]->isDeath);
	ImGui::Checkbox("enemy4", &enemy_[4]->isDeath);
	ImGui::Checkbox("enemy5", &enemy_[5]->isDeath);
	ImGui::End();

	for (int i = 0; i < 6; i++) {
		if (player_->GetIsPlayerAttack()) {
			int attackRange{};
			
			if (player_->GetIsPlayerAttack() == 2) {
				attackRange = player_->GetAttackRangeStrong();
			} else if (player_->GetIsPlayerAttack() == 1) {
				attackRange = player_->GetAttackRangeWeak();
			}

			switch (player_->GetNowPlayerSide()) {
				case Player::side::up:
					if (enemy_[i]->GetWorldTransform().translation_.x > player_->GetAttackRangePos().x - attackRange / 2 && enemy_[i]->GetWorldTransform().translation_.x < player_->GetAttackRangePos().x + attackRange) {
						if (enemy_[i]->GetWorldTransform().translation_.y > player_->GetAttackRangePos().y && enemy_[i]->GetWorldTransform().translation_.y < player_->GetAttackRangePos().y + 30) {
						    enemy_[i]->SetIsDeath(true);
						}
					}
					break;
				case Player::side::down:
					if (enemy_[i]->GetWorldTransform().translation_.x > player_->GetAttackRangePos().x - attackRange / 2 && enemy_[i]->GetWorldTransform().translation_.x < player_->GetAttackRangePos().x + attackRange) {
						if (enemy_[i]->GetWorldTransform().translation_.y < player_->GetAttackRangePos().y && enemy_[i]->GetWorldTransform().translation_.y > player_->GetAttackRangePos().y - 30) {
						    enemy_[i]->SetIsDeath(true);
						}
					}
					break;
				case Player::side::left:
					if (enemy_[i]->GetWorldTransform().translation_.x > player_->GetAttackRangePos().x && enemy_[i]->GetWorldTransform().translation_.x < player_->GetAttackRangePos().x + 30) {
						if (enemy_[i]->GetWorldTransform().translation_.y > player_->GetAttackRangePos().y - attackRange / 2 && enemy_[i]->GetWorldTransform().translation_.y < player_->GetAttackRangePos().y + attackRange / 2) {
						    enemy_[i]->SetIsDeath(true);
						}
					}
					break;
				case Player::side::right:
					if (enemy_[i]->GetWorldTransform().translation_.x < player_->GetAttackRangePos().x && enemy_[i]->GetWorldTransform().translation_.x > player_->GetAttackRangePos().x - 30) {
						if (enemy_[i]->GetWorldTransform().translation_.y > player_->GetAttackRangePos().y - attackRange / 2 && enemy_[i]->GetWorldTransform().translation_.y < player_->GetAttackRangePos().y + attackRange / 2) {
						    enemy_[i]->SetIsDeath(true);
						}
					}
					break;
			}
		}
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	player_->Draw(viewProjection_);

	enemy_[0]->Draw(viewProjection_);
	enemy_[1]->Draw(viewProjection_);
	enemy_[2]->Draw(viewProjection_);
	enemy_[3]->Draw(viewProjection_);
	enemy_[4]->Draw(viewProjection_);
	enemy_[5]->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	player_->CollisionDraw(viewProjection_);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
