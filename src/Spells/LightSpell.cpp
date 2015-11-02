#include "Spells/LightSpell.h"

LightSpell::LightSpell() : Spell() {
}

void LightSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_lightObject = new LightObject(LightBean(sf::Vector2f(), bean.range));
	loadParticleSystem();
}

const sf::Vector2f LightSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(0.f, -50.f);
}

void LightSpell::setDisposed() {
	Spell::setDisposed();
	m_lightObject->setDisposed();
}

void LightSpell::setScreen(Screen* screen) {
	Spell::setScreen(screen);
	screen->addObject(m_lightObject);
}

void LightSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
	if (m_lightObject != nullptr) m_lightObject->setPosition(pos);
	updateParticleSystemPosition();
}

void LightSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void LightSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	m_ps->update(frameTime);
}

void LightSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(50, g_resourceManager->getTexture(ResourceID::Texture_Particle_blob2)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.0f / 2.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->size = sf::Vector2f(getBoundingBox()->width / 2.f, 0.f);
	m_pointGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 15.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 1.f;
	sizeGen->maxEndSize = 5.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 204, 100);
	colGen->maxStartCol = sf::Color(255, 255, 255, 150);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartVel = 5.f;
	velGen->maxStartVel = 10.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.3f;
	timeGen->maxTime = 0.3f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void LightSpell::updateParticleSystemPosition() {
	if (m_pointGenerator == nullptr) return;
	m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}

