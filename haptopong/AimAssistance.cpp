#include "pch.h"
#include "AimAssistance.h"


AimAssistance::AimAssistance(BallPtr ball, RacketPtr playerRacket, chai3d::cCamera* camera) :
	m_ball(ball),
	m_racket(playerRacket),
	m_camera(camera)
{
}


AimAssistance::~AimAssistance(void)
{
}

void AimAssistance::applyImpulseFromRacket(btManifoldPoint& point)
{
	btVector3 bvel = m_ball->getVelocity();
	btVector3 rvel = Util::Vec(m_racket->getVelocity() * m_racket->getMoveAreaScale());

	float xvel = rvel[0] - bvel[0];
	float yvel = rvel[1] - bvel[1];
	float zvel = rvel[2] - bvel[2];

	//TODO: real forces
	xvel *= (1.0f + point.m_combinedRestitution);	
	bvel[0] += xvel;
	bvel[1] += yvel * 0.4f;
	bvel[2] += zvel * 0.4f;
	m_ball->setVelocity(bvel);

	btVector3 angVel = m_ball->getAngularVelocity();
	angVel[1] += -rvel[2] * 200.0f;
	angVel[2] += rvel[1] * 200.0f;
	//std::cout<< "-Y "<<-rvel[2] * 200.0f<<"\t|Z "<< rvel[1] * 200.0f<<"\n";
	m_ball->setAngularVelocity(angVel);

}
	
void AimAssistance::changeVel(const btVector3& target)
{
	btVector3 dist = target - m_ball->getPosition();

	float v0 = m_ball->getVelocity().x();
	float a = 0.0;	
	float v0_a = v0 / a;
	
	float timeX = dist.x() / v0;
	
	float zPos = m_ball->getVelocity().z() * timeX + -10.0 * timeX * timeX * 0.5f;

	btVector3 vel = m_ball->getVelocity();
	vel[2] -= zPos + 1.0;
	m_ball->setVelocity(vel);

	std::cout 
		<<"Time: " << timeX << "\t"
		<<"v0: " << v0 << "\t"
		<<"distx: " << dist.x() << "\t"
		<<"zpos: " << zPos << "\t"
		<<std::endl;
}
	
btVector3 AimAssistance::adjustedVelocity(const btVector3& velocity, const btVector3& angularVel, float factor)
{

}