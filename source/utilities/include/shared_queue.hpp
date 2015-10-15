/*
 * queue.hpp
 *
 *  Created on: 14 ott 2015
 *      Author: Marco
 */

#ifndef SOURCE_UTILITIES_INCLUDE_SHARED_QUEUE_HPP_
#define SOURCE_UTILITIES_INCLUDE_SHARED_QUEUE_HPP_

#include <deque>
#include <condition_variable>
#include <mutex>
#include <utilities/include/atend.hpp>

namespace utilities {

	namespace impl {
		struct nihil {};
	}

	template<typename T, typename L = impl::nihil, L* _l = nullptr, void (L::*push)(T&) = nullptr, void (L::*pop)(T&) = nullptr >
	class shared_queue {
		std::deque<T> data;
		std::mutex lk;
		std::condition_variable cv;
		shared_queue() = default;
	public:

		void enqueue(T& obj) {
			std::lock_guard<std::mutex> guard(lk);
			data.push_back(obj);
			if(_l && push) _l->*push(obj);
		}

		T dequeue(void){
			std::unique_lock<std::mutex> guard(lk);
			on_return<> ret([data](){
				if(_l && push) _l->*pop(data.front());
				data.pop_front();
			});

			cv.wait(guard, [&data](){return !data.empty();});
			return data.front();
		}

		inline bool empty() const {
			return data.empty();
		}

		static inline shared_queue<T, push, pop>& operator()() {
			static shared_queue<T, push, pop> ist;
			return ist;
		}


	};
}


#endif /* SOURCE_UTILITIES_INCLUDE_SHARED_QUEUE_HPP_ */
