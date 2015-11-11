#include "stdafx.h"
#include "InputConsumer.h"
#include "InputManager.h"

using namespace fastbird;

class InputConsumer::InputConsumerImpl{
public:
	InputManagerWeakPtr mInputManager;
	Priority mLevel;
	InputConsumer* mOwner;

	InputConsumerImpl(InputConsumer* owner)
		: mOwner(owner){

	}

	~InputConsumerImpl() {
		auto inputManager = mInputManager.lock();
		if (inputManager){
			inputManager->UnregisterInputConsumer(mLevel, mOwner);
		}
	}

	void OnRegistered(InputManagerPtr inputManager, Priority level){
		mInputManager = inputManager;
		mLevel = level;
	}
	
};
InputConsumer::InputConsumer()
: mImpl(new InputConsumerImpl(this)){

}

InputConsumer::~InputConsumer(){
	delete mImpl;
}

void InputConsumer::OnInputConsumerRegistered(InputManager* inputManager, InputConsumer::Priority level){
	mImpl->OnRegistered(inputManager, level);
}