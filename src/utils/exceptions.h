#ifndef UTILS_EXCEPTIONS_H
#define UTILS_EXCEPTIONS_H

#include <exception>
#include <sstream>
#include <string>

class BaseException : public std::exception
{
public:
	template <typename... Args> BaseException(const Args&... args)
	{
		std::stringstream ss;
		init(ss, args...);
	}

	virtual const char* what() const noexcept override
	{
		return _msg.c_str();
	}

private:
	void init(std::stringstream& ss)
	{
		_msg = ss.str();
	}

	template <typename T, typename... Args> void init(std::stringstream& ss, const T& data, const Args&... args)
	{
		ss << data;
		init(ss, args...);
	}

	std::string _msg;
};

class InvalidInputFileException : public BaseException
{
public:
	InvalidInputFileException(const std::string& fileName) :
		BaseException("Invalid input file '", fileName, "' provided.")
	{}
};

class UnsupportedDataLinkLayerException : public BaseException
{
public:
	UnsupportedDataLinkLayerException() :
		BaseException("Unsupported link-layer protocol. Ethernet expected.")
	{}
};

class UnsupportedIpProtocolException : public BaseException
{
public:
	UnsupportedIpProtocolException() :
		BaseException("Unsupported IP protocol exception. IPv4 expected.")
	{}
};

class UnsupportedTransportProtocolException : public BaseException
{
public:
	UnsupportedTransportProtocolException() :
		BaseException("Unsupported transport protocol exception. TCP expected.")
	{}
};

#endif
