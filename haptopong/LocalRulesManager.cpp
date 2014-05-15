#include "pch.h"
#include "LocalRulesManager.h"
#include "PongScene.h"

#include "Hello.h"
#include "Welcome.h"
#include "UpdatePos.h"
#include "BallEvent.h"

LocalRulesManager::LocalRulesManager(GameRulesPtr gameRules, int port) :
	m_gameRules(gameRules),
	m_port(port)
{
	m_buffer = new unsigned char[2048];
}

LocalRulesManager::~LocalRulesManager(void)
{
	delete[] m_buffer;
	enet_host_destroy(m_server);
}

void LocalRulesManager::initialize()
{
	m_address.host = ENET_HOST_ANY;
	m_address.port = m_port;

	m_server = enet_host_create(&m_address,
		32, 2, 0, 0);

	if(m_server != nullptr) 
	{
		std::cout<<"Server listening to port: "<<m_port<<std::endl;
		std::cout<<"Waiting for client to connect..."<<std::endl;
	}
	else
		std::cout<<"Failed to create server"<<std::endl;
}

void LocalRulesManager::onBallHitTable(const Ball& ball, const Table& table)
{

}

void LocalRulesManager::onBallHitRacket(const Ball& ball, const Racket& racket)
{

}

void LocalRulesManager::onBallOut(const Ball& ball)
{

}

void LocalRulesManager::update(const double& timeStep)
{	
	ENetEvent event;

	while(enet_host_service(m_server, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			std::cout<<"Client connected: "<< event.peer->address.host << " : " << event.peer->address.port<<std::endl;
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			updateMessages(event.packet->data, event.packet->dataLength);

			for(auto msg : m_messageQueue)
			{
				switch(msg->getType())
				{
				case C_HELLO:
					{
						unsigned char* buf = m_buffer;

						MessagePtr welcome = MessagePtr(new Welcome("Server"));
						welcome->addToBuffer(buf);

						ENetPacket * packet = enet_packet_create (m_buffer, 
							welcome->getSize(), 
							ENET_PACKET_FLAG_RELIABLE);

						std::cout<<"GOT HELLO "<<msg->getData()<<std::endl;

						enet_host_broadcast (m_server, 0, packet);
						enet_host_flush (m_server);
						m_isWaiting = false;
					}
					break;
				case G_UPDATE_POS:
					m_pongScene->updateOpponentPos(((UpdatePos*)msg.get())->getPosition());
					break;
				case G_BALLEVENT:
					std::cout<<"Ball event: "+((BallEvent*)msg.get())->getEventType();
					break;
				}
			}
			m_messageQueue.clear();

			//std::cout<<"Got data: "<< event.packet->dataLength << std::endl;
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout<<"Client disconnected: "<< event.peer->address.host << " : " << event.peer->address.port<<std::endl;
			break;
		}
	}
}

void LocalRulesManager::updatePlayerPos(const btVector3& position)
{
	if(isWaiting())
		return;

	unsigned char* buf = m_buffer;

	MessagePtr msg = MessagePtr(new UpdatePos(position));
	msg->addToBuffer(buf);

	ENetPacket * packet = enet_packet_create (m_buffer, 
		msg->getSize(), 
		ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);

	enet_host_broadcast (m_server, 0, packet);
	enet_host_flush (m_server);
}