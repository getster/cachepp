#ifndef _CACHEPP_CACHE_TPP
#define _CACHEPP_CACHE_TPP

#include <limits>
#include <type_traits>

#include "libs/exceptionpp/exception.h"

#include "src/cache.h"

/**
 * Cache
 */

template <typename T> cachepp::Cache<T>::Cache(cachepp::identifier size) : size(size) {
	if(!std::is_base_of<cachepp::Line, T>::value) {
		throw(exceptionpp::InvalidOperation("cachepp::Cache<T>::Cache", "given typename T is not inherited from cachepp::Line"));
	}
}

template <typename T> void cachepp::Cache<T>::acquire(const std::shared_ptr<T>& arg) {
	this->l.lock();
	if(!this->in(arg)) {
		this->allocate(arg);
	}
	this->l.unlock();
}

template <typename T> void cachepp::Cache<T>::clear() {
	this->l.lock();
	for(typename std::map<cachepp::identifier, std::shared_ptr<T>>::iterator it = this->cache.begin(); it != this->cache.end();) {
		this->cache.erase(it->first);
		it->second->unload();
		it++;
	}
	this->l.unlock();
}

/**
 * tests for membership in the cache
 */
template <typename T> bool cachepp::Cache<T>::in(const std::shared_ptr<T>& arg) {
	this->l.lock();
	bool succ = false;
	try {
		this->cache.at(arg->get_identifier());
		succ = true;
	} catch(const std::out_of_range& e) {
	}
	this->l.unlock();
	return(succ);
}

/**
 * the function which actually loads the data into the cache
 */
template <typename T> void cachepp::Cache<T>::allocate(const std::shared_ptr<T>& arg) {
	this->l.lock();
	if(this->cache.size() >= this->size) {
		std::shared_ptr<T> target = this->select();
		this->cache.erase(target->get_identifier());
		target->unload();
	}
	arg->load();
	this->cache.insert(std::pair<cachepp::identifier, std::shared_ptr<T>> (arg->get_identifier(), arg));
	this->l.unlock();
}

/**
 * selects a cache line for eviction
 *
 * default implementation may need to be overridden in derived classes
 */
template <typename T> std::shared_ptr<T> cachepp::Cache<T>::select() {
	cachepp::identifier heuristic = 0;
	cachepp::identifier target = 0;
	for(typename std::map<cachepp::identifier, std::shared_ptr<T>>::iterator it = this->cache.begin(); it != this->cache.end(); ++it) {
		cachepp::identifier  h = this->heuristic(it->second);
		if(h <= heuristic) {
			heuristic = h;
			target = it->first;
		}
	}
	if(target == 0) {
		throw(exceptionpp::RuntimeError("cachepp::Cache::select", "cannot find a target to evict"));
	}
	return(this->cache.at(target));
}

/**
 * SimpleNChanceCache
 */

template <typename T> cachepp::SimpleNChanceCache<T>::SimpleNChanceCache(cachepp::identifier size) : cachepp::Cache<T>::Cache(size) {
	for(cachepp::identifier i = 0; i < this->size; ++i) {
		this->access_data.push_back(0);
	}
}

template <typename T> void cachepp::SimpleNChanceCache<T>::access(const std::shared_ptr<T>& arg) {
	if(!this->in(arg)) {
		throw(exceptionpp::RuntimeError("cachepp::SimpleNChanceCache::access", "attempting to access a cache line which is not loaded"));
	}
	this->access_data.at(this->hash(arg))++;
}

/**
 * takes in an object and returns an index to the auxiliary data
 */
template <typename T> cachepp::identifier cachepp::SimpleNChanceCache<T>::hash(const std::shared_ptr<T>& arg) {
	return(arg->get_identifier() % this->size);
}

template <typename T> size_t cachepp::SimpleNChanceCache<T>::heuristic(const std::shared_ptr<T>& arg) {
	cachepp::identifier h = this->access_data.at(this->hash(arg));
	this->access_data.at(this->hash(arg)) = h == 0 ? 0 : h - 1;
	return(h);
}

#endif
