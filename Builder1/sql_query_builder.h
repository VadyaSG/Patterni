#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<cassert>

class SqlSelectQueryBuilder
{
private:
    std::vector<std::string> _columns;
    std::string _from;
    std::vector<std::string> _where;

public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column);
    SqlSelectQueryBuilder& AddFrom(const std::string& from);
    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& val);
    std::string BuildQuery() const;
};