#include "pch.h"
#include "Graphics.h"

std::unique_ptr<Graphics> Graphics::m_graphics = nullptr;

// DirectX Graphics�N���X�̃C���X�^���X���擾����
Graphics* const Graphics::GetInstance()
{
	if (m_graphics == nullptr)
	{
		// DirectX Graphics�N���X�̃C���X�^���X�𐶐�����
		m_graphics.reset(new Graphics());
	}
	// DirectX Graphics�N���X�̃C���X�^���X��Ԃ�
	return m_graphics.get();
}

// �R���X�g���N�^
Graphics::Graphics()
	:
	m_deviceResources(nullptr),			// �f�o�C�X���\�[�X
	m_commonStates(nullptr),				// �R�����X�e�[�g
	m_spriteBatch(nullptr),					// �X�v���C�g�o�b�`
	m_spriteFont(nullptr),						// �X�v���C�g�t�H���g
	m_basicEffect(nullptr),					// �x�[�V�b�N�G�t�F�N�g
	m_primitiveBatch(nullptr),				// �v���~�e�B�u�o�b�`
	m_rasterrizerState(nullptr),				// ���X�^���C�U�[�X�e�[�g
	m_effectFactory(nullptr),				// �G�t�F�N�g�t�@�N�g��
	m_inputLayout(nullptr),					// ���̓��C�A�E�g
	m_screenW(0),								// �X�N���[����
	m_screenH(0),									// �X�N���[����
	m_view{},											// �r���[�s��
	m_projection{},								// �ˉe�s��
	m_device(nullptr),							// �f�o�C�X
	m_context(nullptr)							// �f�o�C�X�R���e�L�X�g
{
}

// �f�X�g���N�^
Graphics::~Graphics()
{
}

// ����������
void Graphics::Initialize(DX::DeviceResources* deviceResources, const int& width, const int& height)
{
	// �X�N���[���T�C�Y��ݒ肷��
	SetScreenSize(width, height);
	// �f�o�C�X���\�[�X��ݒ肷��
	m_deviceResources = deviceResources;
	// �f�o�C�X���擾����
	m_device = m_deviceResources->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾����
	m_context = m_deviceResources->GetD3DDeviceContext();

	// �R�����X�e�[�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(m_device);
	// �X�v���C�g�o�b�`�𐶐�����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_context);
	// �x�[�V�b�N�G�t�F�N�g�𐶐�����
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_device);
	// �X�v���C�g�t�H���g�𐶐�����
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device, L"resources\\font\\SegoeUI_18.spritefont");
	// �v���~�e�B�u�o�b�`�𐶐�����
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_context);
	// ���̓��C�A�E�g�𐶐�����
	m_basicEffect->SetVertexColorEnabled(true);
	// �e�N�X�`���𖳌��ɂ���
	m_basicEffect->SetTextureEnabled(false);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	// ���̓��C�A�E�g�𐶐�����
	m_device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
	CD3D11_RASTERIZER_DESC rasterizerStateDesc(
		D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
		D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, FALSE, TRUE);
	// ���X�^���C�Y�X�e�[�g�𐶐�����
	m_device->CreateRasterizerState(&rasterizerStateDesc, m_rasterrizerState.ReleaseAndGetAddressOf());
	// �G�t�F�N�g�t�@�N�g���𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device);
	// ���\�[�X�f�B���N�g����ݒ肷��
	//m_fx->SetDirectory(L"resources\\cmo");
}

// �������`�悷��
void Graphics::DrawString(const float& x, const float& y, const wchar_t* str)
{
	// �������`�悷��
	m_spriteFont->DrawString(m_spriteBatch.get(), str, DirectX::SimpleMath::Vector2(x, y));
}

// �v���~�e�B�u�`����J�n����
void Graphics::DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	m_context->OMSetBlendState(m_commonStates->Opaque(), nullptr, 0xFFFFFFFF);
	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0);
	m_context->RSSetState(m_commonStates->CullNone());
	//m_context->RSSetState(m_rasterrizeState.Get());

	// �r���[�s���ݒ肷��
	m_basicEffect->SetView(view);
	// �v���W�F�N�V�����s���ݒ肷��
	m_basicEffect->SetProjection(projection);
	// ���[���h�s���ݒ肷��
	m_basicEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);

	// ���_�J���[��L���ɂ���
	m_basicEffect->SetVertexColorEnabled(true);
	// �e�N�X�`����L���ɂ���
	m_basicEffect->SetTextureEnabled(false);
	// ���̓��C�A�E�g��ݒ肷��
	m_basicEffect->Apply(m_context);
	// ���̓��C�A�E�g��ݒ肷��
	m_context->IASetInputLayout(m_inputLayout.Get());
	// �v���~�e�B�u�`����J�n����
	m_primitiveBatch->Begin();
}

// �v���~�e�B�u�`����I������
void Graphics::DrawPrimitiveEnd()
{
	// �v���~�e�B�u�`����I������
	m_primitiveBatch->End();
}

// ������`�悷��
void Graphics::DrawLine(
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& vector,
	const DirectX::FXMVECTOR& m_color
)
{
	// ���_�J���[��ݒ肷��
	DirectX::VertexPositionColor vertex[2] =
	{
		{ DirectX::SimpleMath::Vector3(position.x, 0.0f, position.y), m_color },
		{ DirectX::SimpleMath::Vector3(position.x + vector.x, 0.0f, position.y + vector.y), m_color },
	};
	// ������`�悷��
	m_primitiveBatch->DrawLine(vertex[0], vertex[1]);
}

// �x�N�g����`�悷��
void Graphics::DrawVector(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color)
{
	using namespace DirectX::SimpleMath;

	// �ʒu��3D�ɕϊ�����
	Vector3 position3 = Vector3(position.x, 0.0f, position.y);
	// �x�N�g����3D�ɕϊ����� 
	Vector3 vector3 = Vector3(vector.x, 0.0f, vector.y);
	// ���̃x�N�g����ݒ肷��T�C�Y��ݒ肷��
	Vector3 arrow3 = -Vector3(vector.x, 0.0f, vector.y);
	// ���K������
	arrow3.Normalize();
	// ���̒���
	arrow3 *= 3.0f;

	float cos = cosf(DirectX::XMConvertToRadians(20.0f));
	float sin = sinf(DirectX::XMConvertToRadians(20.0f));
	// �E��
	Vector3 arrowR(arrow3.x * cos - arrow3.z * sin, 0.0f, arrow3.x * sin + arrow3.z * cos);
	// ����
	Vector3 arrowL(arrow3.x * cos + arrow3.z * sin, 0.0f, -arrow3.x * sin + arrow3.z * cos);
	// ����`�悷��
	DrawLine(DirectX::SimpleMath::Vector2(position3.x + vector3.x, position3.z + vector3.z), Vector2(arrowR.x, arrowR.z), color);
	// ����`�悷��
	DrawLine(DirectX::SimpleMath::Vector2(position3.x + vector3.x, position3.z + vector3.z), Vector2(arrowL.x, arrowL.z), color);
	// ������`�悷��
	DrawLine(position, vector, color);
}

// �~��`�悷��
void Graphics::DrawCircle(
	const DirectX::SimpleMath::Vector2& center,
	const float& radius,
	const DirectX::FXMVECTOR& m_color,
	const int& split
)
{
	using namespace DirectX::SimpleMath;
	// �p�x������������
	float angle = 0.0f;
	// ���S�_��XZ���ʂɕϊ�����
	Vector3 center3 = Vector3(center.x, 0.0f, center.y);
	// �n�_��錾����
	Vector3 position1 = DirectX::SimpleMath::Vector3(cosf(angle), 0.0f, sinf(angle)) * radius + center3;
	for (int index = 0; index < split; index++)
	{
		// �n�_��ݒ肷��
		Vector3 position0 = position1;
		// �p�x���v�Z����
		angle += DirectX::XM_2PI / (float)split;
		// ���_���v�Z����
		position1 = DirectX::SimpleMath::Vector3(cosf(angle), 0.0f, sinf(angle)) * radius + center3;
		// �~��`�悷��
		DrawLine(Vector2(position0.x, position0.z), Vector2((position1 - position0).x, (position1 - position0).z), m_color);
	}
}

// ���f����`�悷��
void Graphics::DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world, const bool& depthBuffer)
{
	if (depthBuffer)
	{
		// ���f����`�悷��
		model->Draw(m_context, *m_commonStates.get(), world, m_view, m_projection);
	}
	else
	{
		// ���f����`�悷��
		model->Draw(
			m_context,
			*m_commonStates.get(),
			world,
			m_view,
			m_projection,
			false,
			[&]() { 	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0); }
		);
	}
}

// �A�j���[�V�������f����`�悷��
void Graphics::DrawModel(
	const DirectX::Model* model,
	const DX::AnimationSDKMESH* animationSDKMESH,
	const DirectX::ModelBone::TransformArray* transformArray,
	const DirectX::SimpleMath::Matrix& world
)
{
	// �{�[���z��̃T�C�Y���擾����
	size_t bones = model->bones.size();
	// �A�j���[�V�����Ƀ��f���A�{�[�����A�{�[����K�p����
	animationSDKMESH->Apply(*model, bones, transformArray->get());
	// �R�����X�e�[�g���擾����
	DirectX::CommonStates* commonState = Graphics::GetInstance()->GetCommonStates();
	// �A�j���[�V������`�悷��
	model->DrawSkinned(m_context, *commonState, bones, transformArray->get(), world, m_view, m_projection);
}

