#ifndef _CACHEPP_SERIALCACHE_H
#define _CACHEPP_SERIALCACHE_H

#include <map>
#include <memory>

#include "src/cacheinterface.h"
#include "src/globals.h"

namespace cachepp {
	/**
	 * this is a subclass of the base Cache template and is NOT thread-safe
	 */
	template <typename D, typename T>
	class SerialCache : public CacheInterface<std::map<identifier, std::shared_ptr<T>>, D, T> {
		public:
			SerialCache(identifier size);

			virtual void acquire(const std::shared_ptr<T>& arg, D aux = D()) final;
			virtual void release(const std::shared_ptr<T>& arg) final;
			virtual void clear() final;
			virtual void remove(const std::shared_ptr<T>& arg) final;

		protected:
			virtual bool in(const std::shared_ptr<T>& arg) final;
			virtual void allocate(const std::shared_ptr<T>& arg) final;
			virtual std::shared_ptr<T> select() final;
			virtual size_t heuristic(const std::shared_ptr<T>& arg) = 0;
	};
}

/**
 * include template instantiation
 *	cf. http://bit.ly/MQKzaI
 */
#include "src/templates/serialcache.template"

#endif
