#ifndef BOOST_NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102
#define BOOST_NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request/request_base.hpp>
#include <boost/thread/mutex.hpp>
#include <cstring>

namespace boost { namespace network { namespace http {

request_base::~request_base() {
  // default implementation, only required for linking.
}

struct request_storage_base_pimpl {
  request_storage_base_pimpl(size_t chunk_size);
  void append(char const *data, size_t size);
  size_t read(char *destination, size_t offset, size_t size);
  void flatten(std::string &destination);
  void clear();
  request_storage_base_pimpl clone() const;
  ~request_storage_base_pimpl();

 private:
  size_t chunk_size_;
  typedef std::vector<std::pair<char *, size_t> > chunks_vector;
  chunks_vector chunks_;
  mutex chunk_mutex;
};

request_storage_base::request_storage_base(size_t chunk_size)
: pimpl_(new (std::nothrow) request_storage_base_pimpl(chunk_size))
{}

request_storage_base::~request_storage_base() {
  delete pimpl_;
}

void request_storage_base::append(char const *data, size_t size) {
  pimpl_->append(data, size);
}

size_t request_storage_base::read(char *destination, size_t offset, size_t size) {
  return pimpl_->read(destination, offset, size);
}

void request_storage_base::flatten(std::string &destination) {
  pimpl_->flatten(destination);
}

void request_storage_base::clear() {
  pimpl_->clear();
}

request_storage_base_pimpl::request_storage_base_pimpl(size_t chunk_size)
: chunk_size_(chunk_size)
, chunks_()
{
  // do nothing here.
}

void request_storage_base_pimpl::append(char const *data, size_t size) {
  if (chunks_.empty()) {
    chunks_.push_back(std::make_pair(
        new (std::nothrow) char[chunk_size_], 0));
  }
  std::pair<char *, size_t> *chunk = &chunks_.back();
  size_t remaining = chunk_size_ - chunk->second;
  while (remaining < size) {
    std::memcpy(chunk->first + chunk->second, data, size - remaining);
    chunk->second += size - remaining;
    data += remaining;
    size -= remaining;
    chunks_.push_back(std::make_pair(
        new (std::nothrow) char[chunk_size_], 0));
    chunk = &chunks_.back();
    remaining = chunk_size_ - chunk->second;
  }
  if (size > 0) {
    std::memcpy(chunk->first + chunk->second, data, size);
    chunk->second += size;
  }
}

size_t request_storage_base_pimpl::read(char *destination, size_t offset, size_t size) {
  if (chunks_.empty()) return 0;
  // First we find which chunk we're going to read from using the provided
  // offset and some arithmetic to determine the correct one.
  size_t chunk_index = offset / chunk_size_;

  // Then we start copying up to size data either until we've reached the end
  // or we're 
  size_t chunks_count = chunks_.size();
  size_t read_count = 0;
  while (size > 0 && chunk_index < chunks_.size()) {
    size_t bytes_to_read = std::min(chunks_[chunk_index].second, size);
    std::memcpy(destination + read_count, chunks_[chunk_index].first, bytes_to_read);
    read_count += bytes_to_read;
    size -= bytes_to_read;
    ++chunk_index;
  }
  return read_count;
}

void request_storage_base_pimpl::flatten(std::string &destination) {
  chunks_vector::const_iterator chunk_iterator = chunks_.begin();
  for (; chunk_iterator != chunks_.end(); ++chunk_iterator) {
    destination.append(chunk_iterator->first, chunk_iterator->second);
  }
}

void request_storage_base_pimpl::clear() {
  chunks_vector::const_iterator chunk_iterator = chunks_.begin();
  for (; chunk_iterator != chunks_.end(); ++chunk_iterator) {
    delete [] chunk_iterator->first;
  }
  chunks_vector().swap(chunks_);
}

request_storage_base_pimpl::~request_storage_base_pimpl() {
  clear();
}

} /* http */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102 */
