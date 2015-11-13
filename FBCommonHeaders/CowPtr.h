#pragma once
#include <memory>
namespace fastbird{
	template <class T>
	class CowPtr{
	public:
		typedef std::shared_ptr<T> TargetPtr;	

		CowPtr(){}
		explicit CowPtr(T* other) : mP(other){}
		CowPtr(const typename CowPtr<T>::TargetPtr& other) : mP(other.mP){}

		T& operator*(){
			Detach();
			return *mP.get();
		}

		const T& operator*() const{
			return *mP.get();
		}

		T* operator->(){
			Detach();
			return mP.get();
		}

		const T* operator->() const{
			return mP.get();
		}

		operator T*(){
			Detach();
			return mP.get();
		}

		operator const T*() const{
			return mP.get();
		}

		T* get(){
			Detach();
			return mP.get();
		}

		const T* get() const{
			return mP.get();
		}

		const T* const_get() const{
			return mP.get();
		}

		bool operator == (const CowPtr<T>& other) const{
			return mP.get() == other.mP.get();
		}

		bool operator !=(const CowPtr<T>& other) const{
			return mP.get != other.mP.get();
		}

		bool operator!() const{
			return !mP.get();
		}

		CowPtr<T>& operator=(const CowPtr<T>& other){
			if (other.mP != mP){
				mP = other.mP;
			}
			return *this;
		}

		CowPtr<T>& operator=(T* other){
			mP = TargetPtr(other);
			return *this;
		}

	private:
		void detach(){
			T* temp = mP.get();
			if (temp && !mP.unique()){
				mP = TargetPtr(new T(*temp));
			}
		}

	private:
		TargetPtr mP;
	};
}