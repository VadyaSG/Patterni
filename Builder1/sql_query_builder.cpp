#include "sql_query_builder.h"

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumn(const std::string& column)
{
	_columns.push_back(column);
	return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddFrom(const std::string& from)
{
	_from = from;
	return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(const std::string& key, const std::string& val)
{
	if (!key.empty() && !val.empty())
	{
		_where.push_back(key + "=" + val);
	}
	return *this;
}

std::string SqlSelectQueryBuilder::BuildQuery() const
{
    std::stringstream query;
    query << "SELECT ";

    if (_columns.empty())
    {
        query << "*";
    }
    else
    {
        for (size_t i = 0; i < _columns.size(); ++i)
        {
            query << _columns[i];
            if (i < _columns.size() - 1)
            {
                query << ", ";
            }
        }
    }

    if (!_from.empty())
    {
        query << " FROM " << _from;
    }

    if (!_where.empty())
    {
        query << " WHERE ";
        for (size_t i = 0; i < _where.size(); ++i)
        {
            query << _where[i];
            if (i < _where.size() - 1)
            {
                query << " AND ";
            }
        }
    }

    query << ";";
    return query.str();
}


