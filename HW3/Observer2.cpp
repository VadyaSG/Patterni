#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class Observer
{
public:
	virtual ~Observer() = default;
	virtual void onWarning(const  std::string& message) {}
	virtual void onError(const std::string& message) {}
	virtual void ofFatalError(const std::string& message) {}
};

class Observable
{
private:
	std::vector<Observer*> observers;

public:
	void add_observer(Observer* observer)
	{
		if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end())
		{
			observers.push_back(observer);
		}
	}

	void remove_observer(Observer* observer)
	{
		auto  it = std::find(observers.begin(), observers.end(), observer);
		if (it != observers.end())
		{
			observers.erase(it);
		}
	}

	void warning(const std::string& message) const 
	{
		for (Observer* obs : observers) 
		{
			if (obs) obs->onWarning(message);
		}
	}

	void error(const std::string& msg)const
	{
		for (Observer* observer : observers)
		{
			if (observer)
			{
				observer->onError(msg);
			}
		}
	}

	void fatal_error(const std::string& msg)const
	{
		for (Observer* observer : observers)
		{
			if (observer)
			{
				observer->ofFatalError(msg);
			}
		}
	}

	size_t get_observer_count()const
	{
		return  observers.size();
	}
};

class WarningObserver :public Observer
{
public:
	void onWarning(const std::string& msg)override
	{
		std::cout << "[WARNING]" << msg << std::endl;
	}
};

class ErrorObserver :public Observer
{
private:
	std::string file_path;

public:
	explicit ErrorObserver(const std::string& path) :file_path(path){}

	void onError(const std::string& msg)override
	{
		std::ofstream file(file_path, std::ios::app);
		if (file.is_open())
		{
			file << "[ERROR] " << msg << std::endl;
			file.close();
		}
		else
		{
			std::cerr << "cant open file";
		}
	}
};

class FatalErrorObserver : public Observer
{
private:
	std::string file_path;

public:
	explicit FatalErrorObserver(const std::string& path) :file_path(path) {}

	void ofFatalError(const std::string& msg)override
	{
		std::cout << "[FATAL ERROR] " << msg << std::endl;

		std::ofstream file(file_path, std::ios::app);
		if (file.is_open())
		{
			file << "[FATAL ERROR]" << msg << std::endl;
			file.close();
		}
		else
		{
			std::cerr << "can't open file" << std::endl;
		}
	}
};

int main()
{
	Observable logger;
	WarningObserver warning_observer;
	ErrorObserver error_observer("Errors.txt");
	FatalErrorObserver fatal_observer("Fatal.txt");

	logger.add_observer(&warning_observer);
	logger.add_observer(&error_observer);
	logger.add_observer(&fatal_observer);

	logger.warning("warning test");
	logger.error("error test");
	logger.fatal_error("fatal err test");

}