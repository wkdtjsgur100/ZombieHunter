#include"MyContactListener.h"

MyContactListener::MyContactListener()
	:contacts()
{

}
MyContactListener::~MyContactListener()
{}

void MyContactListener::BeginContact(b2Contact* contact)
{
	MyContact mc = {contact->GetFixtureA() , contact->GetFixtureB()};
	contacts.push_back(mc);
	
}

void MyContactListener::EndContact(b2Contact* contact)
{
	MyContact mc = {contact->GetFixtureA() , contact->GetFixtureB()};
	std::vector<MyContact>::iterator pos;
		pos = std::find(contacts.begin(), contacts.end(), mc);
		if (pos != contacts.end()) {
				contacts.erase(pos);
		}


}
void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}
void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}




