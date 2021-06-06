#ifndef _DRAGON_
#define _DRAGON_

#include<vector>
#include<string>
#include <fstream>

struct Variable
{
	std::string type_name;
	std::string variable_name;
	void* value{};
};

class Dragon
{
public:
	bool read_file();
	~Dragon();
private:
	bool check_syntax_variables(std::string str);
	bool check_isdigit(std::string str);
	bool check_words(std::string str, std::vector<std::string> rhs);
	bool convert_Type_name(std::vector<std::string> rv);
	int condition(std::vector<std::string> rv);
	void loop(std::vector<std::string> rv);
	void loop_big(float tmp1, float tmp2);
	void loop_small(float tmp1, float tmp2);
	void loop_equal(float tmp1, float tmp2);
	void loop_not_equal(float tmp1, float tmp2);
	bool Arithmetic_unit(std::vector<std::string> rv);
	bool print(std::vector<std::string> rv);
	int check_variable(std::string str);
	bool check(std::string str);
	int read_lines(std::string str);

	int condition_flag{ 0 };
	int loop_flag{ 0 };
	std::vector<std::string> Variables;
	std::vector<Variable*>  var;
	std::vector<std::string> Type_name{ "nat","real" };
	std::vector<std::string> words;
	std::vector<std::string> Instructions{ "cond", "loop" };
	std::vector<std::string> stack;
	std::ifstream file_in;
	float tmp1 = 0;
	float tmp2 = 0;
	float result = 0;
	
};

#endif // !_DRAGON_
