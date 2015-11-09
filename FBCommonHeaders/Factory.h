#pragma once
#include <map>
#include <string>
#include <iostream>
#include <memory>
namespace fastbird{
	template <typename T>
	class Factory{
	public:
		typedef std::shared_ptr<T> (*CreateCallback)();
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
		std::shared_ptr<T> CreateProduct(const char* type){
			if (type == 0 || strlen(type) == 0){
				std::cerr << "Factory:CreateProduct : Invalid arguments\n";
				return 0;
			}
			auto it = mProducts.find(type);
			if (it != mProducts.end()){
				return it->second();
			}
			std::cerr << "Factory:CreateProduct : Not registered type.\n";
			return 0;
		}

	private:
		typedef std::map<std::string, CreateCallback> Products;
		Products mProducts;
	};
}