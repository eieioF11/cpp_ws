#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "../include/f11robo_msg/f11robo_msg.hpp"

int main() {
  // I/Oサービスオブジェクトを作成
  boost::asio::io_service io;

  // シリアルポートオブジェクトを作成
  boost::asio::serial_port serial(io);

  // シリアルポートを開く
  serial.open("/dev/ttyUSB0");

  // ボーレートを設定
  serial.set_option(boost::asio::serial_port_base::baud_rate(115200));

  f11robo::command_msg_t command_msg;
  f11robo::sensor_msg_t sensor_msg;

  double x, y, theta;
  while (1) {
    command_msg.liner_x.data   = 0.0;
    command_msg.angular_z.data = 1.0;
    // データを送信
    boost::asio::write(serial, boost::asio::buffer({f11robo::HEADER}));
    boost::asio::write(serial, boost::asio::buffer(command_msg.get_data()));
    boost::asio::write(serial, boost::asio::buffer({f11robo::END}));
    // データを受信
    uint8_t buf[1], data[33];
    size_t len = boost::asio::read(serial, boost::asio::buffer(buf));
    if (len != 0 && buf[0] == f11robo::HEADER) {
      len = boost::asio::read(serial, boost::asio::buffer(data));
      std::cout << "data_len: " << len << std::endl;
      for (int i = 0; i < 33; i++) {
        sensor_msg.set(i, data[i]);
        // std::cout << "data: " << (int)data[i] << std::endl;
      }

      double w_l     = static_cast<double>(sensor_msg.velocity.left_wheel.data);
      double w_r     = static_cast<double>(sensor_msg.velocity.right_wheel.data);
      double rx      = f11robo::param::R * ((w_r + w_l) / 2.0);
      double ry      = f11robo::param::R * ((w_r + w_l) / 2.0);
      double angular = (f11robo::param::R / (2.0 * f11robo::param::L)) * (w_r - w_l);
      std::cout << "rx: " << rx << " ry:" << ry << std::endl;
      std::cout << "angular:" << angular << std::endl;
      double dt = 0.01;
      x += rx * std::cos(angular) * dt;
      y += ry * std::sin(angular) * dt;
      theta += angular * dt;
      std::cout << "x: " << x << " y:" << y << " theta:" << theta << std::endl;
      // debug
      std::cout << "sensor_msg.velocity.right_wheel: " << sensor_msg.velocity.right_wheel.data << std::endl;
      std::cout << "sensor_msg.velocity.left_wheel: " << sensor_msg.velocity.left_wheel.data << std::endl;
      std::cout << "sensor_msg.rpy.roll: " << sensor_msg.rpy.roll.data << std::endl;
      std::cout << "sensor_msg.rpy.pitch: " << sensor_msg.rpy.pitch.data << std::endl;
      std::cout << "sensor_msg.rpy.yaw: " << sensor_msg.rpy.yaw.data << std::endl;
      std::cout << "sensor_msg.sensor_data.light:";
      for (int i = 0; i < 6; i++)
        std::cout << " " << (int)sensor_msg.sensor_data.light[i];
      std::cout << std::endl;
      for (int i = 0; i < 2; i++)
        std::cout << "sensor_msg.sensor_data.sw[" << i << "]: " << sensor_msg.sensor_data.sw[i] << std::endl;
      std::cout << "sensor_msg.ems: " << sensor_msg.ems << std::endl;
      std::cout << "sensor_msg.battery_voltage: " << sensor_msg.battery_voltage.data << std::endl;
      len = boost::asio::read(serial, boost::asio::buffer(buf));
      std::cout << "end: " << (buf[0] == f11robo::END) << std::endl << std::endl;
    }
  }
  return 0;
}