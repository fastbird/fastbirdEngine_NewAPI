#pragma once
#include <map>
#include <vector>
#include <iostream>
#include "Helpers.h"
namespace fastbird{
	template <typename Observer>
	class Observable{
	public:
		typedef int ObserverEventType;

	protected:
		std::map< ObserverEventType, std::vector<Observer> > mObservers_;

	public:
		void AddObserver(ObserverEventType type, Observer observer){
			auto& observers = mObservers_[type];
			if (!ValueExistsInVector(observers, observer)){
				observers.push_back(observer);
			}
			else{
				std::cerr << "Observable : (error) observer is already registered.";
			}
		}

		bool RemoveObserver(ObserverEventType type, Observer observer){
			auto& observers = mObservers_[type];
			DeleteValuesInVector(observers, observer);
			return observersDeletedAny;
		}

		virtual void OnObserverAdded(Observer observer) {}		
	};
}