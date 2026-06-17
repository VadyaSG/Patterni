#include <iostream>
#include <fstream>
#include <string>

class LogCommand
{
public:
	virtual ~LogCommand() = default;
	virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand :public LogCommand
{
public:
	void print(const std::string& message) override
	{
		std::cout << message << std::endl;
	}
};

class FileLogCommand : public LogCommand
{
private: 
	std::string file_path;
public:
	explicit FileLogCommand(const std::string& path) :file_path(path) {}

	void print(const std::string& message) override
	{
		std::ofstream file(file_path, std::ios::app);
		
		if (file.is_open())
		{
			file << message << std::endl;
			file.close();

			std::cout << "Log added in file (log.txt)";
		}
		else
		{
			std::cerr << "Can't open file" << file_path << std::endl;
		}
	}

};

void print(LogCommand& command)
{
	std::string message = "TEST MSG FOR LOG";
	command.print(message);
}

void print(LogCommand& command, const std::string& message)
{
	command.print(message);
}

int main()
{
	ConsoleLogCommand console_log_command;
	FileLogCommand file_log_command("log.txt");

	std::cout << "1.Console: " << std::endl;
	print(console_log_command, "console msg");

	std::cout << "2.File: " << std::endl;
	print(file_log_command, "file msg");

	return 0;
}