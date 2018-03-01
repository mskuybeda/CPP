#ifndef CSV_ROW_H
#define CSV_ROW_H

#include <vector>
#include <sstream>

using namespace std;

class CSV_row
{
public:
	string const& operator[](size_t index) const;
	size_t size() const;
	void read_next_row(istream& str);

private:
	vector<string> m_data;
};

inline istream& operator >> (istream& str, CSV_row& data)
{
	data.read_next_row(str);
	return str;
}

#endif // !CSV_ROW_H