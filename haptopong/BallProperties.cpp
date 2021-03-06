#include "pch.h"
#include "BallProperties.h"

BallProperties::BallProperties(void) :
	m_friction(0.1),
	m_elasticity(0.036),
	m_restitution(0.9),
	m_weight(0.0027),
	m_radius(0.03),
	m_area(0.0013),
	m_volume(0.000034),
	m_gravity(9.8),
    m_linDamping(0.001),
    m_angDamping(0.9)
{
}


BallProperties::~BallProperties(void)
{
}
