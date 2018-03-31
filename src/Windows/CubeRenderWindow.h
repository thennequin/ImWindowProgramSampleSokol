#include "RenderWindow.h"

class CubeRenderWindow : public RenderWindow
{
public:
						CubeRenderWindow();
	virtual				~CubeRenderWindow();
	
	virtual void		OnRender(const ImVec2& oSize);
protected:
	sg_buffer			m_hVertexBuffer;
	sg_buffer			m_hIndexBuffer;
	sg_shader			m_hShader;
	sg_pipeline			m_hPipeline;

	ImVec2				m_oRotate;
	float				m_fDistance;
};