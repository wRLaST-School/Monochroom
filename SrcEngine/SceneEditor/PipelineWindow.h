#pragma once
#include <SpRootSignature.h>

class PipelineWindow
{
private:
	enum class TabState
	{
		Pipeline,
		RootSignature,
		Shader,
	};

private:
	bool mIsOpen;
	TabState tabState;

private:
	void PipelineTabStateDraw();
	void RootSignatureTabStateDraw();
	void ShaderTabStateDraw();

public:
	PipelineWindow();
	void DrawWindow();
	static void SDraw();

public:
	// セッター
	void SetisOpen(bool isOpen);

public:
	// ゲッター
	bool GetisOpen();
	static PipelineWindow* GetInstance();

private:
	// singleton
	~PipelineWindow() = default;
	PipelineWindow(const PipelineWindow&) = delete;
	PipelineWindow(PipelineWindow&&) = delete;
	PipelineWindow& operator=(const PipelineWindow&) = delete;
	PipelineWindow& operator=(PipelineWindow&&) = delete;

};

