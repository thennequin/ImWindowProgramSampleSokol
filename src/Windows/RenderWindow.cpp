#include "RenderWindow.h"

RenderWindow::RenderWindow( float fClearRed, float fClearGreen, float fClearBlue, float fClearAlpha, int iSampleCount)
	: m_iSampleCount(iSampleCount)
{
	SetTitle("Render");
	m_oCurrentRenderTargetSize = ImVec2(-1.f, -1.f);
	m_hRenderTargetColor = { 0 };
	m_hRenderTargetDepth = { 0 };
	m_hRenderPass = { 0 };

	m_oRenderPassAction = { 0 };
	m_oRenderPassAction.colors[0].action = SG_ACTION_CLEAR;
	m_oRenderPassAction.colors[0].val[0] = fClearRed;
	m_oRenderPassAction.colors[0].val[1] = fClearGreen;
	m_oRenderPassAction.colors[0].val[2] = fClearBlue;
	m_oRenderPassAction.colors[0].val[3] = fClearAlpha;
	m_oRenderPassAction.depth.action = SG_ACTION_CLEAR;
	m_oRenderPassAction.depth.val = 1.f;
	m_oRenderPassAction.stencil.action = SG_ACTION_CLEAR;
	m_oRenderPassAction.stencil.val = 0;
}

RenderWindow::~RenderWindow()
{
	sg_destroy_image(m_hRenderTargetColor);
	sg_destroy_image(m_hRenderTargetDepth);
	sg_destroy_pass(m_hRenderPass);
}

void RenderWindow::OnGui()
{
	ImGui::Button("##RenderArea", ImGui::GetContentRegionAvail());
	ImVec2 oMin = ImGui::GetItemRectMin();
	ImVec2 oMax = ImGui::GetItemRectMax();
	ImVec2 oSize = ImVec2(oMax.x - oMin.x, oMax.y - oMin.y);
		
	if (oSize.x != m_oCurrentRenderTargetSize.x || oSize.y != m_oCurrentRenderTargetSize.y)
	{
		sg_destroy_image(m_hRenderTargetColor);
		sg_destroy_image(m_hRenderTargetDepth);
		sg_destroy_pass(m_hRenderPass);
		m_oCurrentRenderTargetSize = oSize;

		sg_image_desc color_img_desc = { 0 };
		color_img_desc.render_target = true;
		color_img_desc.width = (int)oSize.x;
		color_img_desc.height = (int)oSize.y;
		color_img_desc.min_filter = SG_FILTER_LINEAR;
		color_img_desc.mag_filter = SG_FILTER_LINEAR;
		color_img_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
		color_img_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
		color_img_desc.sample_count = m_iSampleCount;
		m_hRenderTargetColor = sg_make_image(&color_img_desc);

		sg_image_desc depth_img_desc = color_img_desc;
		depth_img_desc.pixel_format = SG_PIXELFORMAT_DEPTHSTENCIL;
		m_hRenderTargetDepth = sg_make_image(&depth_img_desc);

		sg_pass_desc offscreen_pass_desc = { 0 };
		offscreen_pass_desc.color_attachments[0].image = m_hRenderTargetColor;
		offscreen_pass_desc.depth_stencil_attachment.image = m_hRenderTargetDepth;
		m_hRenderPass = sg_make_pass(&offscreen_pass_desc);
	}

	sg_begin_pass(m_hRenderPass, &m_oRenderPassAction);
	OnRender(oSize);
	sg_end_pass();

	ImGui::GetWindowDrawList()->AddImage((void*)m_hRenderTargetColor.id, oMin, oMax);
}