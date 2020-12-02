#pragma once
#include <map>
#include <vector>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment(lib,"assimp.lib")

//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;

	int BoneNum;
	std::string BoneName[4];		//本来はインデックスで管理するべき
	float BoneWeight[4];
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationModel;
	aiMatrix4x4 OffsetMatrix;
	aiMatrix4x4 AnimationMatrix;
};


class CAnimationModel
{
private:
	const aiScene* m_Scene = NULL;
	std::map<std::string, const aiScene*> m_Animetion;

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	//テクスチャー読み込み
	std::map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;
	std::map<const std::string, BONE>m_Bone;

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

public:
	void Draw();
	void Load(const char* FileName);
	void LoadAnimetion(const char* FileName,const char* Name);
	void Unload();
	void Update(const char* AnimationName, const char* AnimetionName2, float BlendRate, int Flame);
};
