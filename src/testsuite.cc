#include <algorithm>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "libs/exceptionpp/exception.h"

#include "src/testsuite.h"

bool is_dup(char l, char r) { return(((l == '\t') || (l == '\n')) && (l == r)); }

cachepp::TestResult::TestResult() : size(0) {}

size_t cachepp::TestResult::get_size() { return(this->size); }

void cachepp::TestResult::push_back(size_t n_acquire, size_t cache_size, size_t total_data, double read_rate, double miss_rate, double line_size, double total_runtime, bool is_parallel, size_t n_threads) {
	this->n_acquire.push_back(n_acquire);
	this->total_data.push_back(total_data);
	this->read_rate.push_back(read_rate);
	this->miss_rate.push_back(miss_rate);
	this->line_size.push_back(line_size);
	this->cache_size.push_back(cache_size);
	this->total_runtime.push_back(total_runtime);
	this->is_parallel.push_back(is_parallel);
	this->n_threads.push_back(n_threads);
	this->size++;
}

double cachepp::TestResult::get_read_rate(size_t index) { return(this->read_rate.at(index) * 100); }
double cachepp::TestResult::get_miss_rate(size_t index) { return(this->miss_rate.at(index) * 100); }
double cachepp::TestResult::get_throughput(size_t index) { return(this->total_data.at(index) / this->total_runtime.at(index)); }
double cachepp::TestResult::get_latency(size_t index) { return(this->n_acquire.at(index) / this->total_runtime.at(index)); }
double cachepp::TestResult::get_line_size(size_t index) { return(this->line_size.at(index)); }
size_t cachepp::TestResult::get_cache_size(size_t index) { return(this->cache_size.at(index)); }
bool cachepp::TestResult::get_is_parallel(size_t index) { return(this->is_parallel.at(index)); }
size_t  cachepp::TestResult::get_n_threads(size_t index) { return(this->n_threads.at(index)); }

std::string cachepp::TestResult::to_string(bool is_tsv) {
	size_t pad = 12;
	std::string sep = " | ";

	if(this->get_size() == 0) {
		throw(exceptionpp::InvalidOperation("cachepp::TestResult::to_string", "reporting zero trials"));
	}

	std::stringstream buffer;
	buffer << std::setw(pad) << "trial" << sep << std::setw(pad) << "cache size" << sep << std::setw(pad) << "read (%)" << sep << std::setw(pad) << "miss (%)" << sep << std::setw(pad) << "tput (B/ms)" << sep << std::setw(pad) << "lat (ms)" << sep << std::setw(pad) << "line (B)" << sep << std::setw(pad) << "parallel" << sep << std::setw(pad) << "n_threads" << std::endl;
	buffer << std::string(buffer.str().length(), '=') << std::endl;
	for(size_t index = 0; index < this->get_size(); ++index) {

		buffer << std::setw(pad) << index + 1 << sep << std::setw(pad) << this->get_cache_size(index) << sep << std::setw(pad) << this->get_read_rate(index) << sep << std::setw(pad) << this->get_miss_rate(index) << sep << std::setw(pad) << this->get_throughput(index) << sep << std::setw(pad) << this->get_latency(index) << sep << std::setw(pad) << this->get_line_size(index) << sep << std::setw(pad) << this->get_is_parallel(index) << sep << std::setw(pad);

		if(this->get_is_parallel(index)) {
			buffer << this->get_n_threads(index);
		} else {
			buffer << "--";
		}
		buffer << std::endl;
	}

	std::string ret = buffer.str();
	// format to tabs
	if(is_tsv) {
		std::replace(ret.begin(), ret.end(), '|', '\t');
		std::replace(ret.begin(), ret.end(), ' ', '\t');
		// cf. http://bit.ly/1p6ATc7
		ret.erase(std::remove(ret.begin(), ret.end(), '='), ret.end());

		// cf. http://bit.ly/1ywrQSZ
		std::string::iterator end = std::unique(ret.begin(), ret.end(), is_dup);
		ret.erase(end, ret.end());

		// cf. http://bit.ly/1oN9U5R
		std::stringstream s;
		s << std::endl << "\t";
		size_t pos = 0;
		while((pos = ret.find(s.str())) != std::string::npos) {
			ret.erase(pos + 1, 1);
		}

		ret = ret.substr(1);
	}
	return(ret);
}

std::ostream& operator<< (std::ostream& os, cachepp::TestResult& obj) {
	os << obj.to_string(false);
	return(os);
}
