#include "ImwWindow.h"
#include "sokol_gfx.h"

class RenderWindow : public ImWindow::ImwWindow
{
protected:
	const int m_iSampleCount;
public:
	RenderWindow(float fClearRed = 0.25f, float fClearGreen = 0.25f, float fClearBlue = 0.25f, float fClearAlpha = 1.0f, int iSampleCount = 1);
	~RenderWindow();

	virtual void		OnRender(const ImVec2& oSize) = 0;

	virtual void		OnGui();

	sg_image			m_hRenderTargetColor;
	sg_image			m_hRenderTargetDepth;
	sg_pass				m_hRenderPass;
	sg_pass_action		m_oRenderPassAction;
	ImVec2				m_oCurrentRenderTargetSize;

	sg_draw_state		m_oDrawState;
};