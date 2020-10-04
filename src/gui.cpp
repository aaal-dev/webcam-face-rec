#include "gui.hpp"

#include "gui/nanovg_gl.h"

namespace app {

Log* Gui::logger = nullptr;
StateCon* Gui::stater = nullptr;

Gui::Gui() {
	logger = Log::getLog();
	stater = StateCon::getStateCon();
	width = stater->framebuffer_width;
	height = stater->framebuffer_height;
	sideMenu_hovered = false;
	sideMenu_focused = false;
	sideMenu_time = 1;
}

Gui::~Gui() {}

bool Gui::initialize_gui() {
	vg = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
	if (vg == NULL) {
		logger->logGLInfo("Could not init nanoVG\n");
		return false;
	}
	logger->logGLInfo("initialize gui by nanoVG\n");
	return true;
}

void Gui::configure_gui() {
	nvgCreateFont(vg, "gost-a", "../../data/fonts/GOST_AU.TTF");
}

bool Gui::draw_button(	NVGcontext* vg, int preicon, const char* text, float pos_x, float pos_y, float width, float height, NVGcolor colour) {
	NVGpaint bg;
	char icon[8];
	float cornerRadius = 4.0f;
	float tw = 0, iw = 0;
	bool hovered = false;
	bool pressed = false;
	
	if ((stater->mouse_pos_x > pos_x && stater->mouse_pos_x < pos_x + width) && 
	    (stater->mouse_pos_y > pos_y && stater->mouse_pos_y < pos_y + height)
	) 
		hovered = true;
	
	if (hovered && stater->mouseButton[stater->MOUSE_BUTTON_LEFT])
		pressed = true;
	
	if (hovered)
		if (pressed) 
			bg = nvgLinearGradient(vg, pos_x, pos_y, pos_x, pos_y + height, nvgRGBA(0,0,0,isBlack(colour)?16:32), nvgRGBA(63,63,63,isBlack(colour)?16:32));
		else
			bg = nvgLinearGradient(vg, pos_x, pos_y, pos_x, pos_y + height, nvgRGBA(127,127,127,isBlack(colour)?16:32), nvgRGBA(0,0,0,isBlack(colour)?16:32));
	else
		bg = nvgLinearGradient(vg, pos_x, pos_y, pos_x, pos_y + height, nvgRGBA(255,255,255,isBlack(colour)?16:32), nvgRGBA(0,0,0,isBlack(colour)?16:32));
	
	nvgBeginPath(vg);
	nvgRoundedRect(vg, pos_x + 1, pos_y + 1, width - 2, height - 2, cornerRadius - 1);
	if (!isBlack(colour)) {
		nvgFillColor(vg, colour);
		nvgFill(vg);
	}
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, pos_x + 0.5f, pos_y + 0.5f, width - 1,height - 1, cornerRadius - 0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);

	nvgFontSize(vg, 17.0f);
	nvgFontFace(vg, "gost-a");
	tw = nvgTextBounds(vg, 0,0, text, NULL, NULL);
	if (preicon != 0) {
		nvgFontSize(vg, height * 1.3f);
		nvgFontFace(vg, "icons");
		iw = nvgTextBounds(vg, 0,0, cpToUTF8(preicon,icon), NULL, NULL);
		iw += height * 0.15f;
	}

	if (preicon != 0) {
		nvgFontSize(vg, height * 1.3f);
		nvgFontFace(vg, "icons");
		nvgFillColor(vg, nvgRGBA(255,255,255,96));
		nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
		nvgText(vg, pos_x + width * 0.5f - tw * 0.5f - iw * 0.75f, pos_y + height * 0.5f, cpToUTF8(preicon,icon), NULL);
	}

	nvgFontSize(vg, 17.0f);
	nvgFontFace(vg, "gost-a");
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgFillColor(vg, nvgRGBA(0,0,0,160));
	nvgText(vg, pos_x + width * 0.5f - tw * 0.5f + iw * 0.25f, pos_y + height * 0.5f - 1, text, NULL);
	nvgFillColor(vg, nvgRGBA(255,255,255,160));
	nvgText(vg, pos_x + width * 0.5f - tw * 0.5f + iw * 0.25f, pos_y + height * 0.5f, text, NULL);
	return pressed;
}

bool Gui::draw_button_centered(	NVGcontext* vg, int preicon, const char* text, float pos_y, float width, float height, NVGcolor colour) {
	float pos_x = ((float)this->width / 2) - (width / 2);
	return draw_button(vg, preicon, text, pos_x, pos_y, width, height, colour);
}

void Gui::draw_plane(NVGcontext* vg, float pos_x, float pos_y, float width, float height, NVGcolor colour) {
	nvgBeginPath(vg);
	nvgRect(vg, pos_x, pos_y, width, height);
	nvgFillColor(vg, colour);
	nvgFill(vg);

}

void Gui::drawMainMenu() {
	float windowRatio = (float)width / (float)height;
	nvgBeginFrame(vg, width, height, windowRatio);
	nvgSave(vg);
	if (draw_button_centered(vg, 0, "Start", height - 200, 200, 32, nvgRGBA(0,0,0,0)))
		stater->setAppState(stater->FULL_FRAME);
	if (draw_button_centered(vg, 0, "Options", height - 160, 200, 32, nvgRGBA(0,0,0,0)))
		stater->setAppState(stater->OPTIONS_MENU);
	if (draw_button_centered(vg, 0, "Credits", height - 120, 200, 32, nvgRGBA(0,0,0,0)))
		stater->setAppState(stater->CREDITS);
	if (draw_button_centered(vg, 0, "Exit", height - 80, 200, 32, nvgRGBA(0,0,0,0)))
		stater->setAppState(stater->EXIT_APP);
	nvgRestore(vg);
	nvgEndFrame(vg);
}

void Gui::drawOptionMenu() {
	float windowRatio = (float)width / (float)height;
	nvgBeginFrame(vg, width, height, windowRatio);
	nvgSave(vg);
	if (draw_button_centered(vg, 0, "Return", 20, 200, 32, nvgRGBA(0,0,0,0)))
		stater->setAppState(stater->MAIN_MENU);
	nvgRestore(vg);
	nvgEndFrame(vg);
}

void Gui::drawCreditsMenu() {
	float windowRatio = (float)width / (float)height;
	nvgBeginFrame(vg, width, height, windowRatio);
	nvgSave(vg);
	if (draw_button_centered(vg, 0, "Return", 20, 200, 32, nvgRGBA(0,0,0,0)))
		stater->setAppState(stater->MAIN_MENU);
	nvgRestore(vg);
	nvgEndFrame(vg);
}

void Gui::drawSideMenu() {
	float windowRatio = (float)this->width / (float)this->height;
	float margin = 20;
	float width = 200;
	float pos_x = 0 - width * sideMenu_time;
	float pos_y = 0;
	
	if (!sideMenu_hovered) {
		if (sideMenu_time < 1)
			sideMenu_time += 0.1;
		if (stater->mouse_pos_x > 0 && stater->mouse_pos_x < 50) {
			sideMenu_hovered = true;
		}
	} else {
		if (sideMenu_time > 0)
			sideMenu_time -= 0.1;
		if (stater->mouse_pos_x > pos_x + width + 50 * sideMenu_time) {
			sideMenu_hovered = false;
		}
	}
	
	nvgBeginFrame(vg, this->width, this->height, windowRatio);
	//nvgSave(vg);
	draw_plane(vg, pos_x, pos_y, width, this->height, nvgRGBA(127,127,127,127));
	if (draw_button(vg, 0, "Return", pos_x + margin, pos_y + margin, width - 2 * margin, 32, nvgRGBA(0,0,0,0))){
		stater->setAppState(stater->MAIN_MENU);
		sideMenu_hovered = false;
		sideMenu_time = 1;
	}
	//nvgRestore(vg);
	nvgEndFrame(vg);
}

void Gui::cleanup() {
	nvgDeleteGL3(vg);
}

int Gui::isBlack(NVGcolor col) {
	if( col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f )
	{
		return 1;
	}
	return 0;
}

char* Gui::cpToUTF8(int cp, char* str) {
	int n = 0;
	if (cp < 0x80) n = 1;
	else if (cp < 0x800) n = 2;
	else if (cp < 0x10000) n = 3;
	else if (cp < 0x200000) n = 4;
	else if (cp < 0x4000000) n = 5;
	else if (cp <= 0x7fffffff) n = 6;
	str[n] = '\0';
	switch (n) {
	case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
	case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
	case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
	case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
	case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
	case 1: str[0] = cp;
	}
	return str;
}

}
