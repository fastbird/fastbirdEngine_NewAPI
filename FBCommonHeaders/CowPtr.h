#pragma once
#include <memory>
namespace fastbird{
	template <class T>
	class CowPtr{
	public:
		typedef std::shared_ptr<T> TargetPtr;	

		CowPtr(){}
		/// Do not pass a pointer managed by other auto ptr functions like the one 
		/// you get from std::shared_ptr<T>::get().
		explicit CowPtr(T* other) : mP(other){}
		CowPtr(const typename CowPtr<T>::TargetPtr& other) : mP(other){}
		
		CowPtr<T>& operator=(const typename CowPtr<T>& other){
			if (other.mP != mP){
				mP = other.mP;
			}
			return *this;
		}

		CowPtr<T>& operator=(T* other){
			if (mP.get() != other){
				mP = TargetPtr(other);
			}
			return *this;
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

		T& operator*(){
			detach();
			return *mP.get();
		}

		const T& operator*() const{
			return *mP.get();
		}

		T* operator->(){
			detach();
			return mP.get();
		}

		const T* operator->() const{
			return mP.get();
		}

		operator T*(){
			detach();
			return mP.get();
		}

		operator const T*() const{
			return mP.get();
		}

		T* get(){
			detach();
			return mP.get();
		}

		const T* get() const{
			return mP.get();
		}

		const T* const_get() const{
			return mP.get();
		}

		TargetPtr data() const{
			return mP;
		}

		void reset(){
			mP.reset();
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