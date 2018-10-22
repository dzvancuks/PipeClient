#pragma once

#include <string>

class ClassA
{
public:
	ClassA();
	~ClassA();

	int get_integer_member() const
	{
		return integer_member_;
	}
	void set_integer_member(int value)
	{
		integer_member_ = value;
	}

	std::string get_string_member() const
	{
		return string_member_;
	}
	void set_string_member(const std::string& value)
	{
		// TODO replace with std::string and deep serialization
		snprintf(string_member_, sizeof string_member_, "%s", value.c_str());
	}

private:
	int integer_member_;
	char string_member_[100]; // TODO replace with std::string and deep serialization
};

