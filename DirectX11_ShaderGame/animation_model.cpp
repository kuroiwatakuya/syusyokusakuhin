#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "animation_model.h"

void CAnimationModel::Draw()
{
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_Scene->mMeshes[m];

		aiMaterial* material = m_Scene->mMaterials[mesh->mMaterialIndex];

		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[path.data]);		//名前で管理するものはmapが便利

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		//インデックスバッファ設定
		CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//ポリゴン描画
		CRenderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void CAnimationModel::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_Scene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_Scene);

	m_VertexBuffer = new ID3D11Buffer * [m_Scene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_Scene->mNumMeshes];

	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_Scene->mNumMeshes];

	CreateBone(m_Scene->mRootNode);

	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_Scene->mMeshes[m];

		//頂点バッファ作成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[1][v].y);
				vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

			delete[] vertex;
		}

		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			//頂点バッファ作成
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces*3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}

		//変形後頂点データ初期化
		{
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				DEFORM_VERTEX deformVertex;
				deformVertex.Position = mesh->mVertices[v];
				deformVertex.Normal = mesh->mNormals[v];
				deformVertex.BoneNum = 0;

				//骨最大4つまで設定
				for (unsigned int b = 0; b < 4; b++)
				{
					deformVertex.BoneName[b] = "";
					deformVertex.BoneWeight[b] = 0.0f;
				}

				m_DeformVertex[m].push_back(deformVertex);
			}


			//ボーンデータ初期化
			for (unsigned int b = 0; b < mesh->mNumBones; b++)
			{
				aiBone* bone = mesh->mBones[b];

				m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

				//変形後頂点にボーンデータ格納
				for (unsigned int w = 0; w < bone->mNumWeights; w++)
				{
					aiVertexWeight weight = bone->mWeights[w];

					//ボーン関連付け
					int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

					m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
					m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
					m_DeformVertex[m][weight.mVertexId].BoneNum++;

					//頂点当たりのボーンが4つ以上だったらエラー
					assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
				}
			}
		}
	}
	{
		for (unsigned int m = 0; m < m_Scene->mNumMaterials; m++)
		{
			aiString path;

			if (m_Scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				if (path.data[0] == '*')
				{
					//FBXファイル内から読み込み
					if (m_Texture[path.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&path.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							CRenderer::GetDevice(),
							(const unsigned char*)m_Scene->mTextures[id]->pcData,
							m_Scene->mTextures[id]->mWidth,	//幅のところに画像のbyte数が入っている(高さには0が入っている。)
							NULL,
							NULL,
							&texture,
							NULL);

						//複数のデータを保存することができる
						m_Texture[path.data] = texture;
					}
				}
				else
				{
					//外部ファイルから読み込み
				}
			}
			else
			{
				m_Texture[path.data] = NULL;
			}
		}
	}
}

//アニメーションのロード
void CAnimationModel::LoadAnimetion(const char* FileName, const char* Name)
{
	m_Animetion[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);	//mapは添え字に文字列を設定できる
	assert(m_Animetion[Name]);
}

void CAnimationModel::Unload()
{
	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	delete[] m_DeformVertex;

	for (std::pair<const std::string, ID3D11ShaderResourceView*>pair : m_Texture)
	{
		pair.second->Release();
	}
	
	aiReleaseImport(m_Scene);
}

void CAnimationModel::Update(const char* AnimetionName, const char* AnimetionName2,float BlendRate, int Flame)
{
	if(!m_Animetion[AnimetionName]->HasAnimations())
	{
		return;
	}
	if (!m_Animetion[AnimetionName2]->HasAnimations())
	{
		return;
	}

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = m_Animetion[AnimetionName]->mAnimations[0];
	aiAnimation* animation2 = m_Animetion[AnimetionName]->mAnimations[0];

	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim1 = animation1->mChannels[c];
		aiNodeAnim* nodeAnim2 = animation2->mChannels[c];
		BONE* bone = &m_Bone[nodeAnim1->mNodeName.C_Str()];

		int f;
		f = Flame % nodeAnim1->mNumRotationKeys;//簡易実装
		aiQuaternion rot1 = nodeAnim1->mRotationKeys[f].mValue;

		f = Flame % nodeAnim1->mNumPositionKeys;//簡易実装
		aiVector3D pos1 = nodeAnim1->mPositionKeys[f].mValue;

		f = Flame % nodeAnim2->mNumRotationKeys;//簡易実装
		aiQuaternion rot2 = nodeAnim2->mRotationKeys[f].mValue;

		f = Flame % nodeAnim2->mNumPositionKeys;//簡易実装
		aiVector3D pos2 = nodeAnim2->mPositionKeys[f].mValue;

		aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;	//線形補間
		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//再帰的にボーンマトリクスを更新
	UpdateBoneMatrix(m_Scene->mRootNode, aiMatrix4x4());

	//頂点変換(CPUスキニング)
	for (unsigned int m = 0; m < m_Scene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_Scene->mMeshes[m];

		D3D11_MAPPED_SUBRESOURCE ms;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;

			matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
			matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
			matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
			matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

			//ウェイトを考慮してマトリクス算出
			//outMatrix = matrix[0]*deformVertex->BoneWeight[0]
			//				+ matrix[1]*deformVertex->BoneWeight[1]
			//				+ matrix[2]*deformVertex->BoneWeight[2]
			//				+ matrix[3]*deformVertex->BoneWeight[3];
			{
				//a
				outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
					+ matrix[1].a1 * deformVertex->BoneWeight[1]
					+ matrix[2].a1 * deformVertex->BoneWeight[2]
					+ matrix[3].a1 * deformVertex->BoneWeight[3];

				outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
					+ matrix[1].a2 * deformVertex->BoneWeight[1]
					+ matrix[2].a2 * deformVertex->BoneWeight[2]
					+ matrix[3].a2 * deformVertex->BoneWeight[3];

				outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
					+ matrix[1].a3 * deformVertex->BoneWeight[1]
					+ matrix[2].a3 * deformVertex->BoneWeight[2]
					+ matrix[3].a3 * deformVertex->BoneWeight[3];

				outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
					+ matrix[1].a4 * deformVertex->BoneWeight[1]
					+ matrix[2].a4 * deformVertex->BoneWeight[2]
					+ matrix[3].a4 * deformVertex->BoneWeight[3];

				//b
				outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
					+ matrix[1].b1 * deformVertex->BoneWeight[1]
					+ matrix[2].b1 * deformVertex->BoneWeight[2]
					+ matrix[3].b1 * deformVertex->BoneWeight[3];

				outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
					+ matrix[1].b2 * deformVertex->BoneWeight[1]
					+ matrix[2].b2 * deformVertex->BoneWeight[2]
					+ matrix[3].b2 * deformVertex->BoneWeight[3];

				outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
					+ matrix[1].b3 * deformVertex->BoneWeight[1]
					+ matrix[2].b3 * deformVertex->BoneWeight[2]
					+ matrix[3].b3 * deformVertex->BoneWeight[3];

				outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
					+ matrix[1].b4 * deformVertex->BoneWeight[1]
					+ matrix[2].b4 * deformVertex->BoneWeight[2]
					+ matrix[3].b4 * deformVertex->BoneWeight[3];

				//c
				outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
					+ matrix[1].c1 * deformVertex->BoneWeight[1]
					+ matrix[2].c1 * deformVertex->BoneWeight[2]
					+ matrix[3].c1 * deformVertex->BoneWeight[3];

				outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
					+ matrix[1].c2 * deformVertex->BoneWeight[1]
					+ matrix[2].c2 * deformVertex->BoneWeight[2]
					+ matrix[3].c2 * deformVertex->BoneWeight[3];

				outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
					+ matrix[1].c3 * deformVertex->BoneWeight[1]
					+ matrix[2].c3 * deformVertex->BoneWeight[2]
					+ matrix[3].c3 * deformVertex->BoneWeight[3];

				outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
					+ matrix[1].c4 * deformVertex->BoneWeight[1]
					+ matrix[2].c4 * deformVertex->BoneWeight[2]
					+ matrix[3].c4 * deformVertex->BoneWeight[3];

				//d
				outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
					+ matrix[1].d1 * deformVertex->BoneWeight[1]
					+ matrix[2].d1 * deformVertex->BoneWeight[2]
					+ matrix[3].d1 * deformVertex->BoneWeight[3];

				outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
					+ matrix[1].d2 * deformVertex->BoneWeight[1]
					+ matrix[2].d2 * deformVertex->BoneWeight[2]
					+ matrix[3].d2 * deformVertex->BoneWeight[3];

				outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
					+ matrix[1].d3 * deformVertex->BoneWeight[1]
					+ matrix[2].d3 * deformVertex->BoneWeight[2]
					+ matrix[3].d3 * deformVertex->BoneWeight[3];

				outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
					+ matrix[1].d4 * deformVertex->BoneWeight[1]
					+ matrix[2].d4 * deformVertex->BoneWeight[2]
					+ matrix[3].d4 * deformVertex->BoneWeight[3];
			}

			deformVertex->Position = mesh->mVertices[v];
			deformVertex->Position *= outMatrix;

			//法線変換用に移動成分を削除
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			deformVertex->Normal = mesh->mNormals[v];
			deformVertex->Normal *= outMatrix;

			//頂点バッファへの書き込み
			vertex[v].Position.x = deformVertex->Position.x;
			vertex[v].Position.y = deformVertex->Position.y;
			vertex[v].Position.z = deformVertex->Position.z;

			vertex[v].Normal.x = deformVertex->Normal.x;
			vertex[v].Normal.y = deformVertex->Normal.y;
			vertex[v].Normal.z = deformVertex->Normal.z;

			vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
			vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

			vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
	}
}

void CAnimationModel::CreateBone(aiNode* node)
{
	BONE bone;
	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

void CAnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//マトリックスの乗算順番に注意
	aiMatrix4x4 worldMatrix;

	worldMatrix = matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}