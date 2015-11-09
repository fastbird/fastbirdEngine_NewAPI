#pragma once
#include <map>
#include <string>
#include <iostream>
namespace fastbird{
	template <typename T>
	class Factory{
	public:
		typedef T* (*CreateCallback)();
		bool RegisterProduct(const char* type, CreateCallback callback){
			if (type == 0 || strlen(type) == 0 || callback == 0){
				std::cerr << "Factory:RegisterProduct : Invalid arguments\n";
				return false;
			}
			if (mProducts.find(type) == mProducts.end()){
				mProducts[std::string(type)] = callback;
				return true;
			}
			std::cerr << "Already registered product type : " << type << std::endl;
			return false;
		}
		void UnregisterProduct(const char* type){
			if (type == 0 || strlen(type) == 0){
				std::cerr << "Factory:RegisterProduct : Invalid arguments\n";
				return;
			}
			auto it = mProducts.find(std::string(type));
			if (it != mProducts.end()){
				mProducts.erase(it);
			}
		}
		T* CreateProduct(const char* type);

	private:
		typedef std::map<std::string, CreateCallback> Products;
		Products mProducts;
	};
}