/**
* @file   end_point.cpp
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 10:26:11
* @brief
**/

#include "end_point.h"

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>

EndPoint::EndPoint(const std::string& ip, const std::string& port)
    : ip_(ip), port_(port), socket_fd_(0) {
}

bool EndPoint::CreateListenSocket() {
  struct addrinfo hint, *result;
  int res, fd, flags;

  memset(&hint, 0, sizeof(struct addrinfo));
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;

  res = getaddrinfo(ip_.c_str(), port_.c_str(), &hint, &result);
  if (res == -1) {
    return false;
  }

  fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (fd == -1) {
    goto err;
  }

  res = bind(fd, result->ai_addr, result->ai_addrlen);
  if (res == -1) {
    goto err;
  }

  flags = fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;             
  flags = fcntl(fd, F_SETFL, flags);
  if (flags == -1) {
    goto err;
  }

  res = listen(fd, 100);
  if (res == -1) {
    goto err;
  }

  socket_fd_ = fd;
  return true;

err:
  free(result);
  return false;
}

int EndPoint::GetListenFd() {
  return socket_fd_;
}
