#pragma once
#include <MyMath.h>
#include<Model.h>
#include<WorldTransform.h>
class Enemy  {
public:
	void Initialize(
	    Model* model, uint32_t textureHandle_,
		 float x, float y, float kCharacterSpeedx,
	    float kCharacterSpeedy);

	void Update();

	void Draw(ViewProjection& viewProjection_);

	void OnCollision();

	const WorldTransform& GetWorldTransform() { return worldTransform_; };
	const bool& GetIsDeath() { return isDeath; };

	void SetIsDeath(bool isDeath_) {
		isDeath = isDeath_;
	}
	
	bool isDeath = false;

private:

	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	float kCharacterSpeedX_ = 0;
	float kCharacterSpeedY_ = 0;
	Vector3 move = {0, 0, 0};
	
	WorldTransform worldTransform_;

};
