#include <boost/asio.hpp>
#include <iostream>

int main() {
  // I/Oサービスオブジェクトを作成
  boost::asio::io_service io;

  // シリアルポートオブジェクトを作成
  boost::asio::serial_port serial(io);

  // シリアルポートを開く
  serial.open("/dev/ttyUSB0");

  // ボーレートを設定
  serial.set_option(boost::asio::serial_port_base::baud_rate(9600));

  // データを送信
  // boost::asio::write(serial, boost::asio::buffer("Hello, World!\n"));

  // データを受信
  char buf[128];
  size_t len = boost::asio::read(serial, boost::asio::buffer(buf));
  std::cout.write(buf, len);
  std::cout << std::endl;

  return 0;
}