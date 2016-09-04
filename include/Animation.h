#pragma once

#include "global.h"

////////////////////////////////////////////////////////////
// This class was altered from the original source
// by Ironbell
// For the original source, see notice below
////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

class Animation {
public:
	Animation(const sf::Time& frameTime) { m_frameTime = frameTime; };
	Animation() { m_frameTime = sf::milliseconds(100); };

	void clearFrames();
	void addFrame(const sf::IntRect& rect);
	void setSpriteSheet(const sf::Texture* texture);
	void setFrameTime(const sf::Time& frameTime);
	void setLooped(bool isLooped);

	const sf::Texture* getSpriteSheet() const;
	std::size_t getSize() const;
	const sf::IntRect& getFrame(std::size_t n) const;
	const sf::Time& getFrameTime() const;
	const sf::Time getAnimationTime() const;
	bool isLooped() const;

private:
	bool m_isLooped = true;
	std::vector<sf::IntRect> m_frames;
	sf::Time m_frameTime;
	const sf::Texture* m_texture = nullptr;
};