#include <iostream>
#include <string>
#include <fstream>

enum class Type
{
	Warning,
	Error,
	Fatal_Error,
	Unknown
};

class LogMessage
{
private:
	Type _type;
	std::string _message;

public:
	LogMessage(Type type, const std::string& msg) :_type(type), _message(msg) {}
	Type type() const { return _type; }
	const std::string& message()const { return _message; }
};

class Handler
{
private:
	std::unique_ptr<Handler>_next;

protected:
	virtual void process(const LogMessage& message) = 0;
	virtual bool can_handle(const LogMessage& message) = 0;

public:
	void set_next(std::unique_ptr<Handler> next)
	{
		_next = std::move(next);
	}

	void handle(const LogMessage& message)
	{
		if (can_handle(message))
		{
			process(message);
		}
		else if (_next)
		{
			_next->handle(message);
		}
		else
		{
			throw std::runtime_error("No handler found for message type");
		}

	}
};

class FatalErrorHandler : public Handler
{
protected:
	bool can_handle(const LogMessage& message)override
	{
		return message.type() == Type::Fatal_Error;
	}

	void process(const LogMessage& message)override
	{
		throw std::runtime_error("Fatal error: " + message.message());
	}
};

class ErrorHandler :public Handler
{
private:
	std::string _file_path;

public:
	explicit ErrorHandler(const std::string& file_path) :_file_path(file_path) {}

protected:
	bool can_handle(const LogMessage& msg) override
	{
		return msg.type() == Type::Error;
	}
	void process(const LogMessage& msg)override
	{
		std::ofstream file(_file_path, std::ios::app);
		if (file.is_open())
		{
			file << "ERROR: " << msg.message() << std::endl;
			file.close();
		}
		else
		{
			throw std::runtime_error("Can't open log file: " + _file_path);
		}
	}

};

class WarningHandler : public Handler 
{
protected:
	bool can_handle(const LogMessage& msg) override
	{
		return msg.type() == Type::Warning;
	}
	void process(const LogMessage& msg) override 
	{
		std::cout << "WARNING: " << msg.message() << std::endl;
	}
};

class UnknownHandler : public Handler
{
protected:
	bool can_handle(const LogMessage& msg)override 
	{
		return msg.type() == Type::Unknown;
	}

	void process(const LogMessage& msg) override 
	{
		throw std::runtime_error("Unknown message: " + msg.message());
	}
};

class LoggerChain
{
private:
	std::unique_ptr<Handler> _head;

public:
	LoggerChain() 
	{
		auto fatalHandler = std::make_unique<FatalErrorHandler>();
		auto errorHandler = std::make_unique<ErrorHandler>("log.txt");
		auto warningHandler = std::make_unique<WarningHandler>();
		auto unknownHandler = std::make_unique<UnknownHandler>();

		warningHandler->set_next(std::move(unknownHandler));
		errorHandler->set_next(std::move(warningHandler));
		fatalHandler->set_next(std::move(errorHandler));

		_head= std::move(fatalHandler);
	}

	void log(const LogMessage& msg) 
	{
		_head->handle(msg);
	}
};

int main()
{
	LoggerChain logger;
	try 
	{
		logger.log(LogMessage(Type::Warning, "Low disk space"));
		logger.log(LogMessage(Type::Error, "Failed to open configuration file"));
		logger.log(LogMessage(Type::Fatal_Error, "Out of memory"));
		logger.log(LogMessage(Type::Unknown, "Something went wrong"));
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	return 0;
}