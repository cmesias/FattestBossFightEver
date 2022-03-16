/*
 * Button.h
 *
 *  Created on: Mar 11, 2022
 *      Author: healt
 */

#ifndef BUTTON_H_
#define BUTTON_H_


#include "TextNFont.h"
#include "../Helper.h"

struct Button: public TextNFont, public Helper {

public:
	float x, y;
	float w, h;
	bool hover;
	bool pressed;
	std::string textField;
	std::string resultText;

	void Init(std::string textField, std::string resultText);

	void Load();

	void Free();

	void Update(float mex, float mey, float x, float y, bool leftClick);

	std::string CheckMouseReleased(float mex, float mey);

	void Render(SDL_Renderer *gRenderer);
};

#endif /* BUTTON_H_ */
