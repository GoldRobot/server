#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <fstream>
#include <string>

#define ADRESS_IP "127.0.0.1"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using namespace boost::asio;

void client_process(ip::tcp::socket *target_socket, int session_id)
{
	cout << "Connection done" << endl;
	string data;
	//data = "Session ID: " + std::to_string(session_id);
	data = std::to_string(session_id);
	data = "abvgd\r\n";
	bool cycle_work = true;
	bool sent_once = false;
	bool check_sent_once = false;
	int i_data = 0;
	const string delimiter = "\r\n";
	while (cycle_work)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
		try
		{
			if (!sent_once && check_sent_once || !check_sent_once)
			{
				data = "login username password";
				cout << "Sending: " << data << endl;
				target_socket->send(buffer(data + delimiter));
			}
		}
		catch (boost::system::system_error e)
		{
			return; //unnoknw error
		}
		//boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
		//cycle_work = false;
	}
}
//asdfasdf
int main()
{
	std::ofstream file;
	file.open("log.txt");
	file.clear();
	io_service service_io_stream; //������ ������ IO
	ip::tcp::endpoint target_point(ip::address::from_string(ADRESS_IP), 42042);
	ip::tcp::socket target_socket(service_io_stream);
	try
	{
		target_socket.connect(target_point);
	}
	catch (boost::system::system_error e)
	{
		cout << "Connection failed (" << e.code().value() << ")." << endl;
		file << "Connection failed (" << e.code().value() << " -- " << e.code().message() << ")." << endl;
		//system("pause");
		file.close();
		//system("pause");
		return -1;
	}
	int session_id = 0;
	/*
	try
	{
		target_socket.read_some(buffer(&session_id, sizeof(session_id)));
	}
	catch (boost::system::system_error e)
	{
		if (e.code().value() == 2)
		{
			cout << "\nConnection aborted." << endl;
			e.~system_error();
			//system("pause");
			return 0;
		}
		else
		{
			cout << "\nUnnoknw exeption." << endl;
			e.~system_error();
			//system("pause");
			return -1;
		}
	}
	*/
	boost::thread talk_thread = boost::thread(boost::bind(client_process, &target_socket, session_id));
	talk_thread.join();
	cout << "Session ID: " << session_id << endl;
	cout << "Connected." << endl;
	//system("pause");
	target_socket.close();
	cout << "Disconnected." << endl;
	file.close();
	return 0;

}

