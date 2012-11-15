// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102
#define NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102

#include <network/protocol/http/request/request_base.hpp>
#include <thread>
#include <cstring>

namespace network { namespace http {

request_base::~request_base() {
  // default implementation, only required for linking.
}

struct request_storage_base_pimpl {
  explicit request_storage_base_pimpl(size_t chunk_size);
  request_storage_base_pimpl *clone() const;
  void append(char const *data, size_t size);
  size_t read(std::string &destination, size_t offset, size_t size) const;
  void flatten(std::string &destination) const;
  void clear();
  bool equals(request_storage_base_pimpl const &other) const;
  void swap(request_storage_base_pimpl &other);
  ~request_storage_base_pimpl();

 private:
  size_t chunk_size_;
  typedef std::vector<std::pair<char *, size_t> > chunks_vector;
  chunks_vector chunks_;
  mutable std::mutex chunk_mutex_;

  request_storage_base_pimpl(request_storage_base_pimpl const &other);
};

request_storage_base::request_storage_base(size_t chunk_size)
: pimpl_(new (std::nothrow) request_storage_base_pimpl(chunk_size))
{}

request_storage_base::request_storage_base(request_storage_base const &other)
: pimpl_(other.pimpl_->clone())
{}

request_storage_base::~request_storage_base() {
  delete pimpl_;
}

void request_storage_base::append(char const *data, size_t size) {
  pimpl_->append(data, size);
}

size_t request_storage_base::read(std::string &destination, size_t offset, size_t size) const {
  return pimpl_->read(destination, offset, size);
}

void request_storage_base::flatten(std::string &destination) const {
  pimpl_->flatten(destination);
}

void request_storage_base::clear() {
  pimpl_->clear();
}

bool request_storage_base::equals(request_storage_base const &other) const {
  return pimpl_->equals(*other.pimpl_);
}

void request_storage_base::swap(request_storage_base &other) {
  return other.pimpl_->swap(*pimpl_);
}

request_storage_base_pimpl::request_storage_base_pimpl(size_t chunk_size)
: chunk_size_(chunk_size)
, chunks_()
{
  // do nothing here.
}

request_storage_base_pimpl::request_storage_base_pimpl(request_storage_base_pimpl const &other)
: chunk_size_(other.chunk_size_)
, chunks_(0) {
  std::lock_guard<std::mutex> scoped_lock(other.chunk_mutex_);
  chunks_.reserve(other.chunks_.size());
  chunks_vector::const_iterator it = other.chunks_.begin();
  for (; it != other.chunks_.end(); ++it) {
    chunks_vector::value_type pair =
        std::make_pair(
            new (std::nothrow) char[other.chunk_size_],
            it->second);
    std::memcpy(pair.first, it->first, it->second);
    chunks_.push_back(pair);
  }
}

request_storage_base_pimpl * request_storage_base_pimpl::clone() const {
  return new(std::nothrow) request_storage_base_pimpl(*this);
}
 
void request_storage_base_pimpl::append(char const *data, size_t size) {
  std::lock_guard<std::mutex> scoped_lock(chunk_mutex_);
  if (chunks_.empty()) {
    chunks_.push_back(std::make_pair(
        new (std::nothrow) char[chunk_size_], 0));
  }
  std::pair<char *, size_t> *chunk = &chunks_.back();
  BOOST_ASSERT(chunk_size_ >= chunk->second);
  size_t remaining = chunk_size_ - chunk->second;
  while (remaining < size) {
    size_t bytes_to_write = std::min(size - remaining, chunk_size_);
    std::memcpy(chunk->first + chunk->second, data, bytes_to_write);
    chunk->second += bytes_to_write;
    BOOST_ASSERT(chunk->second <= chunk_size_);
    data += bytes_to_write;
    size -= bytes_to_write;
    chunks_.push_back(std::make_pair(
        new (std::nothrow) char[chunk_size_], 0));
    chunk = &chunks_.back();
    remaining = chunk_size_;
  }
  if (size > 0) {
    std::memcpy(chunk->first + chunk->second, data, size);
    chunk->second += size;
  }
}

size_t request_storage_base_pimpl::read(std::string &destination, size_t offset, size_t size) const {
  std::lock_guard<std::mutex> scoped_lock(chunk_mutex_);
  if (chunks_.empty()) return 0;
  // First we find which chunk we're going to read from using the provided
  // offset and some arithmetic to determine the correct one.
  size_t chunk_index = offset / chunk_size_;
  offset = offset % chunk_size_;

  // Then we start copying up to size data either until we've reached the end
  size_t chunks_count = chunks_.size();
  size_t read_count = 0;
  while (size > 0 && chunk_index < chunks_count) {
    size_t bytes_to_read = std::min(chunks_[chunk_index].second, size);
    destination.append(chunks_[chunk_index].first+offset,
                       chunks_[chunk_index].first+offset+bytes_to_read);
    read_count += bytes_to_read;
    size -= bytes_to_read;
    offset = 0;
    ++chunk_index;
  }
  return read_count;
}

void request_storage_base_pimpl::flatten(std::string &destination) const {
  std::lock_guard<std::mutex> scpoped_lock(chunk_mutex_);
  chunks_vector::const_iterator chunk_iterator = chunks_.begin();
  for (; chunk_iterator != chunks_.end(); ++chunk_iterator) {
    destination.append(chunk_iterator->first, chunk_iterator->second);
  }
}

void request_storage_base_pimpl::clear() {
  std::lock_guard<std::mutex> scoped_lock(chunk_mutex_);
  chunks_vector::const_iterator chunk_iterator = chunks_.begin();
  for (; chunk_iterator != chunks_.end(); ++chunk_iterator) {
    delete [] chunk_iterator->first;
  }
  chunks_vector().swap(chunks_);
}

bool request_storage_base_pimpl::equals(request_storage_base_pimpl const &other) const {
  std::lock(other.chunk_mutex_, this->chunk_mutex_);
  if (other.chunk_size_ != chunk_size_ || other.chunks_.size() != chunks_.size()) {
    other.chunk_mutex_.unlock();
    this->chunk_mutex_.unlock();
    return false;
  }
  chunks_vector::const_iterator chunk_iterator = chunks_.begin();
  chunks_vector::const_iterator other_iterator = other.chunks_.begin();
  for (; chunk_iterator != chunks_.begin() && other_iterator != other.chunks_.end();
         ++chunk_iterator, ++other_iterator) {
    if (chunk_iterator->second != other_iterator->second ||
        strncmp(chunk_iterator->first, other_iterator->first, chunk_iterator->second)) {
      other.chunk_mutex_.unlock();
      this->chunk_mutex_.unlock();
      return false;
    }
  }
  other.chunk_mutex_.unlock();
  this->chunk_mutex_.unlock();
  return true;
}

void request_storage_base_pimpl::swap(request_storage_base_pimpl &other) {
  std::lock(other.chunk_mutex_, this->chunk_mutex_);
  std::swap(chunk_size_, other.chunk_size_);
  std::swap(chunks_, other.chunks_);
  other.chunk_mutex_.unlock();
  this->chunk_mutex_.unlock();
}

request_storage_base_pimpl::~request_storage_base_pimpl() {
  clear();
}


}  // namespace http
}  // namespace network

#endif /* NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102 */
