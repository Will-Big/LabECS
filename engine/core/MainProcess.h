#pragma once

class CORE_EXPORT MainProcess
{
public:
	MainProcess(HINSTANCE hInst, std::string_view title, uint32_t width, uint32_t height, bool isLauncher);
	virtual ~MainProcess();

	void Initialize();
	void Loop();

protected:
	virtual void Update();
	virtual void Render();

	float time_ = 0.f;

private:
	HWND hwnd_;
	std::string title_;
	uint32_t width_;
	uint32_t height_;
};
