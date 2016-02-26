#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"

AllyWalkingBehavior::AllyWalkingBehavior(Enemy* enemy) :
	EnemyMovingBehavior(enemy),
	MovingBehavior(enemy),
	WalkingBehavior(enemy) {
}

void AllyWalkingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	WalkingBehavior::checkCollisions(nextPosition);
	// if the enemy collidesX but can't jump and is ally and idle, teleports to its owner.
	EnemyState state = m_enemy->getEnemyState();
	if (state != EnemyState::Chasing && !m_jumps && m_mainChar->getMovingBehavior()->isGrounded() && dist(m_mainChar->getPosition(), m_enemy->getPosition()) > 400.f) {
		sf::Vector2f newPos(m_mainChar->getPosition().x, m_mainChar->getPosition().y + m_mainChar->getBoundingBox()->height - m_enemy->getBoundingBox()->height);
		WorldCollisionQueryRecord rec;
		rec.ignoreDynamicTiles = m_ignoreDynamicTiles;
		rec.boundingBox = *m_enemy->getBoundingBox();
		rec.boundingBox.left = newPos.x;
		rec.boundingBox.top = newPos.y;
		if (m_enemy->getLevel()->collides(rec)) return;
		m_enemy->setPosition(newPos);
		m_isCollisionTiltSuppressed = true;
	}
}

void AllyWalkingBehavior::execHandleMovementInput() {
	// movement AI
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;
	if (hasTarget && (m_enemy->getEnemyState() == EnemyState::Chasing || m_enemy->getEnemyState() == EnemyState::Recovering)) {
		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_movingDirectionX = 1;
		}
		else {
			m_movingDirectionX = 0;
		}
	}
	else if (hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {
		m_movingDirectionX = (targetCenter.x < center.x) ? 1 : -1;
	}
	else if (m_enemy->getEnemyState() == EnemyState::Idle || m_enemy->getEnemyState() == EnemyState::Waiting) {

		sf::Vector2f mainCharCenter = m_mainChar->getCenter();

		float approachingDistance = (m_enemy->getState() == GameObjectState::Walking) ? (m_approachingDistance - 10.f) : m_approachingDistance;

		if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > approachingDistance) {
			m_movingDirectionX = 1;
		}
		else {
			m_movingDirectionX = 0;
		}
	}

	if (m_jumps && m_isGrounded) {
		m_enemy->setVelocityY(-m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
		m_jumps = false;
	}
}
