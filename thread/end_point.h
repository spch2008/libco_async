/**
* @file   end_point.h
* @author sunpengcheng(sunpengcheng@foxmail.com)
* @date   2017-06-04 10:26:11
* @brief
**/

#ifndef _END_POINT_H_
#define _END_POINT_H_

#include <string>

class EndPoint {
 public:
  EndPoint(const std::string& ip, const std::string& port);
  bool CreateListenSocket();
  int GetListenFd();

 private:
   std::string ip_;
   std::string port_;
   int socket_fd_;
};

#endif
