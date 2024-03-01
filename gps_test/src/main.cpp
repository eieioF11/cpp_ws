#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>

int main() {
  // I/Oサービスオブジェクトを作成
  boost::asio::io_service io;

  // シリアルポートオブジェクトを作成
  boost::asio::serial_port serial(io);

  // シリアルポートを開く
  serial.open("/dev/ttyUSB0");

  // ボーレートを設定
  serial.set_option(boost::asio::serial_port_base::baud_rate(9600));

  while (1) {
    // データを受信
    char buf[128];
    size_t len = boost::asio::read(serial, boost::asio::buffer(buf));
    // std::cout.write(buf, len);
    // std::cout << std::endl;
    if (len != 0) {
      std::string message(buf);
      std::vector<std::string> v;
      boost::algorithm::split(v, message, boost::is_any_of("\r\n"));
      //std::cout << " " << std::endl;
      for (const std::string& s : v) {
        if (s[0] == '$') {
          std::vector<std::string> data;
          boost::algorithm::split(data, s, boost::is_any_of(","));
          if(data.size() < 2) continue;
          if (data[0] == "$GPGGA") {
            std::cout << data[0] << " " << data.size() << std::endl;
            std::cout << s << std::endl;
          }
          if (data[0] == "$GPRMC") {
            std::cout << data[0] << " " << data.size() << std::endl;
            std::cout << s << std::endl;
          }
        }
      }
    }
  }
  return 0;
}