#include "CubeRenderWindow.h"
#include "glm/gtc/matrix_transform.hpp"

typedef struct
{
	float x, y, z;
	float u, v;
} VertexUv;

typedef struct
{
	ImVec2 size;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
}OffscreenParams;

const VertexUv c_pCubeVertices[] = {
	/* pos                 uvs */
	-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
	1.0f,  1.0f, -1.0f,    1.0f, 1.0f,
	-1.0f,  1.0f, -1.0f,   0.0f, 1.0f,

	-1.0f, -1.0f,  1.0f,   0.0f, 0.0f,
	1.0f, -1.0f,  1.0f,    1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,    1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,   0.0f, 1.0f,

	-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,   1.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,   1.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,   0.0f, 1.0f,

	1.0f, -1.0f, -1.0f,    0.0f, 0.0f,
	1.0f,  1.0f, -1.0f,    1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,    1.0f, 1.0f,
	1.0f, -1.0f,  1.0f,    0.0f, 1.0f,

	-1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,   1.0f, 0.0f,
	1.0f, -1.0f,  1.0f,    1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,    0.0f, 1.0f,

	-1.0f,  1.0f, -1.0f,   0.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,   1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,    1.0f, 1.0f,
	1.0f,  1.0f, -1.0f,    0.0f, 1.0f
};
const int c_iCubeVerticesSize = sizeof(c_pCubeVertices);

const uint16_t c_pCubeIndices[] = {
	0, 1, 2,  0, 2, 3,
	6, 5, 4,  7, 6, 4,
	8, 9, 10,  8, 10, 11,
	14, 13, 12,  15, 14, 12,
	16, 17, 18,  16, 18, 19,
	22, 21, 20,  23, 22, 20
};
const int c_iCubeIndicesSize = sizeof(c_pCubeIndices);


CubeRenderWindow::CubeRenderWindow()
	: RenderWindow(0.f, 0.f, 0.f, 1.f, 4)
{
	sg_buffer_desc oVertexBufferDesc = { 0 };
	oVertexBufferDesc.type = SG_BUFFERTYPE_VERTEXBUFFER;
	oVertexBufferDesc.size = c_iCubeVerticesSize;
	oVertexBufferDesc.content = c_pCubeVertices;
	m_hVertexBuffer = sg_make_buffer(&oVertexBufferDesc);

	sg_buffer_desc oIndexBufferDesc = { 0 };
	oIndexBufferDesc.type = SG_BUFFERTYPE_INDEXBUFFER;
	oIndexBufferDesc.size = c_iCubeIndicesSize;
	oIndexBufferDesc.content = c_pCubeIndices;
	m_hIndexBuffer = sg_make_buffer(&oIndexBufferDesc);

	sg_shader_desc oShaderDesc = { 0 };
	oShaderDesc.vs.uniform_blocks[0].size = sizeof(OffscreenParams);
	oShaderDesc.vs.uniform_blocks[0].uniforms[0].name = "size";
	oShaderDesc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_FLOAT2;
	oShaderDesc.vs.uniform_blocks[0].uniforms[1].name = "projection";
	oShaderDesc.vs.uniform_blocks[0].uniforms[1].type = SG_UNIFORMTYPE_MAT4;
	oShaderDesc.vs.uniform_blocks[0].uniforms[2].name = "view";
	oShaderDesc.vs.uniform_blocks[0].uniforms[2].type = SG_UNIFORMTYPE_MAT4;
	oShaderDesc.vs.uniform_blocks[0].uniforms[3].name = "model";
	oShaderDesc.vs.uniform_blocks[0].uniforms[3].type = SG_UNIFORMTYPE_MAT4;

	oShaderDesc.vs.source = R"***(
			#version 330

			uniform vec2 size;
			uniform mat4 projection;
			uniform mat4 view;
			uniform mat4 model;

			in vec4 position;
			in vec2 texcoord0;
			in vec3 normal0;

			out vec2 uv;
			out vec3 normal;

			void main()
			{
			  vec3 camdir = (view * vec4(0,0,1,1)).xyz;
			  mat4 mvp = projection * view * model;
			  gl_Position = mvp * position;
			  uv = texcoord0;
			  normal = normalize(position.xyz);
			  normal.y = -normal.y;
			}
		)***";
	oShaderDesc.fs.source = R"***(
			#version 330

			in vec2 uv;
			in vec3 normal;

			layout(location=0) out vec4 frag_color_0;

			void main()
			{
				frag_color_0 = vec4(uv, 0.0, 1.0);
			}
		)***";
	m_hShader = sg_make_shader(&oShaderDesc);

	sg_pipeline_desc oPipDesc = { 0 };
	oPipDesc.layout.attrs[0].name = "position";
	oPipDesc.layout.attrs[0].offset = offsetof(VertexUv, x);
	oPipDesc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
	oPipDesc.layout.attrs[1].name = "texcoord0";
	oPipDesc.layout.attrs[1].offset = offsetof(VertexUv, u);
	oPipDesc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT2;
	oPipDesc.shader = m_hShader;
	oPipDesc.index_type = SG_INDEXTYPE_UINT16;
	oPipDesc.depth_stencil.depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL;
	oPipDesc.depth_stencil.depth_write_enabled = true;
	oPipDesc.blend.enabled = false;
	oPipDesc.blend.depth_format = SG_PIXELFORMAT_DEPTHSTENCIL;
	oPipDesc.rasterizer.cull_mode = SG_CULLMODE_BACK;
	oPipDesc.rasterizer.sample_count = m_iSampleCount;

	m_hPipeline = sg_make_pipeline(&oPipDesc);

	m_oRotate = ImVec2(0.f, 0.f);
	m_fDistance = 15.0f;
}

CubeRenderWindow::~CubeRenderWindow()
{

}

void CubeRenderWindow::OnRender(const ImVec2& oSize)
{
	if (ImGui::IsItemHovered())
	{
		float fWheel = ImGui::GetIO().MouseWheel;
		if (fWheel != 0.f)
		{
			m_fDistance -= fWheel * (ImGui::GetIO().KeyShift ? 0.01f : 0.25f);
			if (m_fDistance < 0.01f)
				m_fDistance = 0.01f;
		}
	}

	if (ImGui::IsItemActive() && ImGui::IsMouseDragging())
	{
		ImVec2 oDelta = ImGui::GetMouseDragDelta();
		m_oRotate.y += glm::radians(oDelta.x);
		m_oRotate.x -= glm::radians(oDelta.y);
		ImGui::ResetMouseDragDelta();
	}
	sg_draw_state oDrawState = { 0 };
	oDrawState.pipeline = m_hPipeline;
	oDrawState.vertex_buffers[0] = m_hVertexBuffer;
	oDrawState.index_buffer = m_hIndexBuffer;
	sg_apply_draw_state(&oDrawState);
	OffscreenParams oParams;
	oParams.size = oSize;
	glm::mat4 mProjection = glm::perspective(glm::radians(90.f), oSize.x / oSize.y, 0.1f, 100.f);
	glm::mat4 mView = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	mView = glm::rotate(mView, m_oRotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	mView = glm::rotate(mView, m_oRotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 vLookAt(0.f);
	glm::vec3 vEye;
	vEye.x = vLookAt.x + (m_fDistance * cos(m_oRotate.x) * cos(m_oRotate.y));
	vEye.z = vLookAt.z + (m_fDistance * cos(m_oRotate.x) * sin(m_oRotate.y));
	vEye.y = vLookAt.y + (m_fDistance * sin(m_oRotate.x));
	mView = glm::lookAt(vEye, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 mModel = glm::scale(glm::mat4(1.0f), glm::vec3(5.f));
	oParams.projection = mProjection;
	oParams.view = mView;
	oParams.model = mModel;
	sg_apply_uniform_block(SG_SHADERSTAGE_VS, 0, &oParams, sizeof(OffscreenParams));
	sg_draw(0, c_iCubeIndicesSize / sizeof(c_pCubeIndices[0]), 1);
}
