#include "CSV_row.h"

string const& CSV_row::operator[](size_t index) const
{
	return m_data[index];
}
size_t CSV_row::size() const
{
	return m_data.size();
}
void CSV_row::read_next_row(istream& str)
{
	string line;
	getline(str, line);

	stringstream lineStream(line);
	string cell;

	m_data.clear();
	while (getline(lineStream, cell, ','))
	{
		m_data.push_back(cell);
	}
}
